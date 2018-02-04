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

    unsigned int mRadiusExplosion = 4;
    int mLatenceBetweenBomb = 2000;//set signed to avoid a warning with sfml clock
};

#endif // CONFIGPLAYERBOMBERMANCOMPONENT_HPP

