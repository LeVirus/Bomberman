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
};

#endif // CONFIGPLAYERBOMBERMANCOMPONENT_HPP

