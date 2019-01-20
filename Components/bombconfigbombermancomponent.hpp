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
    unsigned int mNumPlayerEntity;
    unsigned int mTimeBeforeExplosion = 2000;
    bool mPreviousSizeLittle = true;
    virtual ~BombConfigBombermanComponent() = default;
};

#endif // BOMBCONFIGBOMBERMANCOMPONENT_HPP

