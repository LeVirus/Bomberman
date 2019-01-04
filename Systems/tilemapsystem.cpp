#include "tilemapsystem.hpp"
#include "positioncomponent.hpp"
#include "tilemapbombermancomponent.hpp"
#include <iostream>

TilemapSystem::TilemapSystem()
{
    if(! bAddComponentToSystem(ecs::POSITION_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout POSITION_COMPONENT.\n";
    }
    if(! bAddComponentToSystem(TILEMAP_BOMBER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMBER_TILEMAP_COMPONENT.\n";
    }
}

void TilemapSystem::execSystem()
{
    System::execSystem();
    mVectComponentTilemap.clear();
    for( unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i )
    {
        TilemapBombermanComponent * tilemapComp = stairwayToComponentManager().
                searchComponentByType<TilemapBombermanComponent> (mVectNumEntity[i], TILEMAP_BOMBER_COMPONENT);
        ecs::PositionComponent * positionComp = stairwayToComponentManager().
                searchComponentByType<ecs::PositionComponent>(mVectNumEntity[i], ecs::POSITION_COMPONENT);
        if(tilemapComp && positionComp)
        {
            mVectComponentTilemap.push_back({tilemapComp ,positionComp});
        }
    }
}

void TilemapSystem::displaySystem() const
{}

const VectPairCompTilemap &TilemapSystem::getVectCompTilemap()const
{
    return mVectComponentTilemap;
}

