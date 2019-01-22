#pragma once

#include "constants.hpp"
#include <cstdint>

struct NetworkData
{
    TypeEntityFlag mEntityType;
    unsigned int mNetworkID;
    float mPosX, mPosY;
};

struct NetworkLevelData
{
    unsigned int mLenght, mHeight;
    unsigned int mLenghtTile, mHeightTile;
    std::vector<uint8_t>  mLevelArray;
};
