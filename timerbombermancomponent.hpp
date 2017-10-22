#ifndef TIMERBOMBERMANCOMPONENT_HPP
#define TIMERBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <SFML/System/Clock.hpp>

struct TimerBombermanComponent : public ecs::Component
{
    TimerBombermanComponent()
    {
        muiTypeComponent = BOMBER_TIMER_COMPONENT;
    }

    void displayComponent()const override
    {
        //Component::displayComponent();
    }
    sf::Clock mBombClock;
    bool mLaunched = false;
};

#endif // TIMERBOMBERMANCOMPONENT_HPP
