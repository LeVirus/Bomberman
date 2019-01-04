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
        muiTypeComponent = TILEMAP_BOMBER_COMPONENT;
    }

    void displayComponent()const override
    {}

    Tableau2D mTabTilemap;
    //position of tile in the texture
    VectPairUi_t mvectPositionTile;
    unsigned int mNumAssociateTexture = TEXTURE_BOMBERMAN;
    unsigned int mLenghtTile, mHeightTile;
    //if the tilemap have to be memorize in the graphic motor
    bool mPersistant = false;
};

#endif // TILEMAPBOMBERMANCOMPONENT_HPP

