#ifndef CONFIGPLAYERBOMBERMANCOMPONENT_HPP
#define CONFIGPLAYERBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <SFML/System/Clock.hpp>

struct PlayerConfigBombermanComponent : public ecs::Component
{
    PlayerConfigBombermanComponent()
    {
        muiTypeComponent = PLAYER_CONFIG_BOMBER_COMPONENT;
    }

    void displayComponent()const override
    {}
    bool mPreviousStepA;
    bool mVisibleSprite = true;
    unsigned int mNumberLife = 2;
    unsigned int mInitX;
    unsigned int mInitY;

    unsigned int mRadiusExplosion = 4;
    unsigned int mMode = MODE_PLAYER_NORMAL;
    //set signed to avoid a warning with sfml clock
    int mLatenceBetweenBomb = 2000, mLatenceBetweenRepop = 2000, mInvulnerabilityAfterDeath = 5000;
};

#endif // CONFIGPLAYERBOMBERMANCOMPONENT_HPP

