#include "explosionbombermansystem.hpp"
#include "tilemapbombermancomponent.hpp"
#include "constants.hpp"
#include "bombconfigbombermancomponent.hpp"
#include "positioncomponent.hpp"
#include "flagcomponent.hpp"
#include "timerbombermancomponent.hpp"
#include "tilemapbombermancomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include "tableau2d.hpp"
#include "engine.hpp"
#include "vector2D.hpp"
#include <cassert>

ExplosionBombermanSystem::ExplosionBombermanSystem()
{
        if(! bAddComponentToSystem(ecs::POSITION_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout POSITION_COMPONENT.\n";
        }
        if(! bAddComponentToSystem(BOMBER_FLAG_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout BOMBER_FLAG_COMPONENT.\n";
        }
        if(! bAddComponentToSystem(BOMBER_TIMER_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout BOMBER_TIMER_COMPONENT.\n";
        }
}

void ExplosionBombermanSystem::execSystem()
{
    System::execSystem();
    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
    for(; it != mVectNumEntity.end() ; ++it)
    {
        FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
                (*it, BOMBER_FLAG_COMPONENT);

        assert(flagComponent && "flagComponent is null\n");
        if(flagComponent->muiNumFlag != FLAG_EXPLOSION)
        {
            continue;
        }

        TimerBombermanComponent* timerComp = stairwayToComponentManager() .
                    searchComponentByType <TimerBombermanComponent> (*it, BOMBER_TIMER_COMPONENT);
        assert(timerComp && "timerComp == NULL\n");

        if(! timerComp->mLaunched)
        {
            timerComp->mBombClock.restart();
            timerComp->mLaunched = true;
        }
        else
        {
            unsigned int timeElapsed = timerComp->mBombClock.getElapsedTime().asMilliseconds();
            if(timeElapsed >= mTimeExplosion)
            {
                mptrSystemManager->getptrEngine()->bRmEntity(*it);
//                std::cout << *it << "End explode!!!\n";
            }
        }
    }
}

void ExplosionBombermanSystem::displaySystem() const
{}

void ExplosionBombermanSystem::makeBombExplode(unsigned int numEntityBomb)
{
    unsigned int caseX, caseY;
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType < ecs::PositionComponent >
            (numEntityBomb, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    MoteurGraphique::static_getPositionsCurrentCase(*posComponent, caseX, caseY, false);

    BombConfigBombermanComponent *bombConfComponent = stairwayToComponentManager().searchComponentByType <BombConfigBombermanComponent>
            (numEntityBomb, BOMBER_BOMB_CONFIG_COMPONENT);
    assert(bombConfComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");

    PlayerConfigBombermanComponent *playerConfComponent = stairwayToComponentManager().searchComponentByType<PlayerConfigBombermanComponent>
            (bombConfComponent->mNumPlayerEntity, BOMBER_PLAYER_CONFIG_COMPONENT);
    assert(playerConfComponent && "BombBombermanSystem::lauchBomb playerConfComponent is null\n");
    createExplosions(caseX, caseY,  playerConfComponent->mRadiusExplosion);
}

bool ExplosionBombermanSystem::createExplosions(unsigned int caseX, unsigned int caseY, unsigned int explosionRadius)
{
    unsigned int minX = caseX - 1, maxX = caseX + 1, minY = caseY - 1, maxY = caseY + 1;
    unsigned int vertSizeUp = 0, vertSizeDown = 0, horizSizeLeft = 0, horizSizeRight = 0;
    bool maxXOk = false, minXOk = false, maxYOk = false, minYOk = false;

    unsigned int entityLevel = Niveau::getNumEntityLevel();
    TilemapBombermanComponent *levelTileComponent = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>
            (entityLevel, BOMBER_TILEMAP_COMPONENT);
    assert(levelTileComponent && "tileComponent is null\n");

    const Tableau2D &tabNiveau = levelTileComponent->mTabTilemap;

    if(tabNiveau.getLargeur() <= caseY || tabNiveau.getLongueur() <= caseX)
    {
        return false;
    }
    for(unsigned int i = 0; i < explosionRadius; ++i)
    {
        if(!maxXOk)
        {
            if(tabNiveau.getValAt(maxX, caseY) != TILE_EMPTY)
            {
                --maxX;
                maxXOk = true;
            }
            else
            {
                ++horizSizeRight;
                ++maxX;
            }
        }
        if(!minXOk)
        {
            if(tabNiveau.getValAt(minX, caseY) != TILE_EMPTY)
            {
                ++minX;
                minXOk = true;
            }
            else
            {
                ++horizSizeLeft;
                --minX;
            }
        }
        if(!maxYOk)
        {
            if(tabNiveau.getValAt(caseX, maxY) != TILE_EMPTY)
            {
                --maxY;
                maxYOk = true;
            }
            else
            {
                ++vertSizeDown;
                ++maxY;
            }
        }
        if(!minYOk)
        {
            if(tabNiveau.getValAt(caseX, minY) != TILE_EMPTY)
            {
                ++minY;
                minYOk = true;
            }
            else
            {
                ++vertSizeUp;
                --minY;
            }
        }
    }

    ////////////////////////////////////////////////////////
    //Create Explosions component

    //create horizontal explosion
    createEntityExplosion(minX, caseY, explosionRadius, horizSizeLeft, horizSizeRight, false);
    //create vertical explosion
    createEntityExplosion(caseX, minY, explosionRadius, vertSizeUp, vertSizeDown, true);
    //WTF??
    return true;
}

void ExplosionBombermanSystem::createEntityExplosion(unsigned int positionCaseX, unsigned int positionCaseY, unsigned int explosionRadius,
                                                     unsigned int firstSize, unsigned int secondSize, bool vertical)
{
    unsigned int explosionEntity = createExplosionEntity();

    FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
            (explosionEntity, BOMBER_FLAG_COMPONENT);
    assert(flagComponent && "flagComponent is null\n");
    flagComponent->muiNumFlag = FLAG_EXPLOSION;

    TilemapBombermanComponent *tileComponent = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>
            (explosionEntity, BOMBER_TILEMAP_COMPONENT);
    assert(tileComponent && "tileComponent is null\n");

    tileComponent->mHeightTile = 16;
    tileComponent->mLenghtTile = 16;
    tileComponent->mNumAssociateTexture = 1;

    unsigned int totalSize = firstSize + 1 + secondSize;

    if(vertical)
        tileComponent->mTabTilemap.resize(1, totalSize);
    else
        tileComponent->mTabTilemap.resize(totalSize, 1);

    for(unsigned int i = 0; i < totalSize; ++i)
    {
        //check if the radius explosion is the longuest it could be in the first part
        if(i == 0 && firstSize == explosionRadius)
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_END_UP);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_END_LEFT);
        }
        //check the middle of explosion
        else if(i == firstSize)
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_CENTER);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_CENTER);
        }
        //check if the radius explosion is the longuest it could be in the second part
        else if(i == totalSize - 1 && secondSize == explosionRadius)
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_END_DOWN );
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_END_RIGHT);
        }
        else
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_VERTICAL_MIDDLE);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_HORIZONTAL_MIDDLE);
        }
    }
    loadTilePosition(*tileComponent);
    //position the tilemap
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
            (explosionEntity, ecs::POSITION_COMPONENT);
    assert(posComponent && "posComponent is null\n");
    MoteurGraphique::static_positionnerCaseTileMap(*posComponent, positionCaseX, positionCaseY);

}

void ExplosionBombermanSystem::loadTilePosition(TilemapBombermanComponent &tileComp)
{
    tileComp.mvectPositionTile.push_back({50, 5});//0 end left
    tileComp.mvectPositionTile.push_back({34, 85});//1 end right
    tileComp.mvectPositionTile.push_back({85, 85});//2 EXPLOSION_CENTER
    tileComp.mvectPositionTile.push_back({51, 85});//3 EXPLOSION_VERTICAL_MIDDLE
    tileComp.mvectPositionTile.push_back({0, 85});// 4 EXPLOSION_END_UP
    tileComp.mvectPositionTile.push_back({10, 5});//5 EXPLOSION_END_DOWN//NOTTT
    tileComp.mvectPositionTile.push_back({68, 85});//6 EXPLOSION_HORIZONTAL_MIDDLE
}

unsigned int ExplosionBombermanSystem::createExplosionEntity()
{
    //create entity
    unsigned int numCreatedEntity = mptrSystemManager->getptrEngine()->AddEntity();
    ecs::Engine *ECSEngine = mptrSystemManager->getptrEngine();
    ECSEngine->bAddComponentToEntity( numCreatedEntity, ecs::POSITION_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, ecs::COLL_RECTBOX_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, BOMBER_FLAG_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, BOMBER_TIMER_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, BOMBER_TILEMAP_COMPONENT );
    stairwayToComponentManager().updateComponentFromEntity();
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<FlagBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TimerBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TilemapBombermanComponent>());
    return numCreatedEntity;
}
