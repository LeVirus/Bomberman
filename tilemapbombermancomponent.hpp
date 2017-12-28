#ifndef TILEMAPBOMBERMANCOMPONENT_HPP
#define TILEMAPBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include "tableau2d.hpp"
#include <vector>

typedef std::vector<std::pair<unsigned int, unsigned int>> VectPairUi_t;

struct TilemapBombermanComponent : public ecs::Component
{
    TilemapBombermanComponent()
    {
        muiTypeComponent = BOMBER_TILEMAP_COMPONENT;
    }

    void displayComponent()const override
    {}

    Tableau2D mTabTilemap;
    //position of tile in the texture !!! MOVE TO graphic Motor
    VectPairUi_t mvectPositionTile;
    unsigned int mNumAssociateTexture = 0;
    unsigned int mSizeTile;//!!! MOVE TO graphic Motor

};

#endif // TILEMAPBOMBERMANCOMPONENT_HPP

