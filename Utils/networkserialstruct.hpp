#pragma once

#include "constants.hpp"
#include <cstdint>
#include <array>

using pairUi = std::pair<uint32_t, uint32_t>;


struct NetworkData
{
    TypeEntityFlag mEntityType;
    uint32_t mNetworkID;
    float mPosX, mPosY;
};

struct NetworkLevelData
{
    uint8_t mLenght, mHeight;
    uint8_t mLenghtTile, mHeightTile;
    uint8_t mLevelArray[LEVEL_MAX_SIZE];
    uint8_t mNumPlayers;
    std::array<pairUi, MAX_PLAYER> mPlayersInitPos;
};
