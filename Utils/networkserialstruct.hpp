#pragma once

#include "constants.hpp"

struct NetworkData
{
    TypeEntityFlag mEntityType;
    unsigned int mNetworkID;
    float mPosX, mPosY;
    static unsigned int attributeNum()
    {
        static unsigned int mExistingNum = 0;
        return ++mExistingNum;
    }
};
