#ifndef CONFIGPLAYERBOMBERMANCOMPONENT_HPP
#define CONFIGPLAYERBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <SFML/System/Clock.hpp>

struct PlayerConfigBombermanComponent : public ecs::Component
{
    PlayerConfigBombermanComponent()
    {
        muiTypeComponent = BOMBER_PLAYER_CONFIG_COMPONENT;
    }

    void displayComponent()const override
    {}

    unsigned int mNumberLife = 2;
    unsigned int mInitX = 1;
    unsigned int mInitY = 1;

    unsigned int mRadiusExplosion = 4;
    unsigned int mMode = MODE_PLAYER_NORMAL;
    //set signed to avoid a warning with sfml clock
    int mLatenceBetweenBomb = 2000, mLatenceBetweenRepop = 2000, mInvulnerabilityAfterDeath = 2000;
};

#endif // CONFIGPLAYERBOMBERMANCOMPONENT_HPP

