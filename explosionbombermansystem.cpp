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
//    System::execSystem();
//    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
//    for(; it != mVectNumEntity.end() ; ++it)
//    {

//    }
}

void ExplosionBombermanSystem::displaySystem() const
{}

void ExplosionBombermanSystem::makeBombExplode(unsigned int numEntityBomb)
{
    unsigned int caseX, caseY;
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType < ecs::PositionComponent >
            (numEntityBomb, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    MoteurGraphique::static_getPositionsCurrentCase(*posComponent, caseX, caseY);

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
    unsigned int verticalExplosionEntity = createExplosionEntity();
    unsigned int horizontalExplosionEntity = createExplosionEntity();
    TilemapBombermanComponent *tileVertComponent = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>
            (verticalExplosionEntity, BOMBER_TILEMAP_COMPONENT);
    assert(tileVertComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");

    TilemapBombermanComponent *tileHorizComponent = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>
            (horizontalExplosionEntity, BOMBER_TILEMAP_COMPONENT);
    assert(tileHorizComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");

    unsigned int minX = caseX - 1, maxX = caseX + 1, minY = caseY - 1, maxY = caseY + 1;
    unsigned int vertSizeRight = 0, vertSizeLeft = 0, horizSizeLeft = 0, horizSizeRight = 0;
    bool maxXOk = false, minXOk = false, maxYOk = false, minYOk = false;

    const Tableau2D &tabNiveau = Niveau::getTabNiveau();
    if(tabNiveau.getLargeur() >= caseY || tabNiveau.getLongueur() >= caseX)
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
                ++vertSizeRight;
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
                ++vertSizeLeft;
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
                ++horizSizeLeft;
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
                ++horizSizeRight;
                --minY;
            }
        }
    }
    tileVertComponent->mTabTilemap.resize(vertSizeLeft + 1 + vertSizeRight, 1);
    for(unsigned int i = 0; i < tileVertComponent->mTabTilemap.getLongueur(); ++i)
    {
        tileVertComponent->mTabTilemap.setValAt(i, 1, EXPLOSION_HORIZONTAL_MIDDLE);
    }
    if(horizSizeLeft == explosionRadius)
    {
        tileVertComponent->mTabTilemap.setValAt(0, 1, EXPLOSION_END_LEFT);
    }
    if(horizSizeRight == explosionRadius)
    {
        tileVertComponent->mTabTilemap.setValAt(tileVertComponent->mTabTilemap.getLongueur() - 1,
                                                1, EXPLOSION_END_RIGHT);
    }
    tileVertComponent->mTabTilemap.setValAt(caseX, caseY, EXPLOSION_CENTER);
    positionTileMap(minX, caseY, verticalExplosionEntity);

    tileHorizComponent->mTabTilemap.resize(1, horizSizeLeft + 1 + horizSizeRight);
    return true;
}

void ExplosionBombermanSystem::positionTileMap(unsigned int caseX, unsigned int caseY,
                                               unsigned int numEntity)
{
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType < ecs::PositionComponent >
            (numEntity, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    //MoteurGraphique::
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
