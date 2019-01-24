#pragma once

#include "constants.hpp"
#include <cstdint>
#include <array>



struct NetworkData
{
    TypeEntityFlag mEntityType;
    uint8_t mNetworkID;
    float mPosX, mPosY;
};

struct NetworkLevelData
{
    uint8_t mLenght, mHeight;
    uint8_t mLenghtTile, mHeightTile;
    uint8_t mLevelArray[LEVEL_MAX_SIZE];
};
