#ifndef BOMBCONFIGBOMBERMANCOMPONENT_HPP
#define BOMBCONFIGBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <SFML/System/Clock.hpp>

struct BombConfigBombermanComponent : public ecs::Component
{
    BombConfigBombermanComponent()
    {
        muiTypeComponent = BOMB_CONFIG_BOMBER_COMPONENT;
    }

    void displayComponent()const override
    {}
    bool mPreviousSizeLittle = true;
    unsigned int mNumPlayerEntity;
    unsigned int mTimeBeforeExplosion = 2000;
};

#endif // BOMBCONFIGBOMBERMANCOMPONENT_HPP

