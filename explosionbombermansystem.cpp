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

    std::cout << bombConfComponent->mNumPlayerEntity << "OKKKKKqsdqsdqsdKKKK\n";

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
    unsigned int memMaxX, memMinX, memMaxY,  memMinY;
    bool maxXOk = false, minXOk = false, maxYOk = false, minYOk = false;

    const Tableau2D &tabNiveau = Niveau::getTabNiveau();
    if(tabNiveau.getLargeur() >= caseY || tabNiveau.getLongueur() >= caseX)
    {
        return false;
    }

    for(unsigned int i = 0; i < explosionRadius; ++i, ++maxX, --minX, ++maxY, --minY)
    {
        if(!maxXOk && tabNiveau.getValAt(maxX, caseY) != TILE_EMPTY)
        {
            --maxX;
            memMaxX = maxX;
            maxXOk = true;
        }
        if(!minXOk && tabNiveau.getValAt(minX, caseY) != TILE_EMPTY)
        {
            ++minX;
            memMinX = minX;
            minXOk = true;
        }
        if(!maxYOk && tabNiveau.getValAt(caseX, maxY) != TILE_EMPTY)
        {
            --maxY;
            memMaxY = maxY;
            maxYOk = true;
        }
        if(!minYOk && tabNiveau.getValAt(caseX, minY) != TILE_EMPTY)
        {
            --minY;
            memMinY = minY;
            minYOk = true;
        }
    }
    unsigned int memSize = 1;
    if(!maxXOk)
    {
        memSize += explosionRadius;
    }
    else
    {
        memSize += memMaxX - caseX;
    }

    return true;
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
