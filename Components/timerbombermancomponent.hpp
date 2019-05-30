#ifndef TIMERBOMBERMANCOMPONENT_HPP
#define TIMERBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <SFML/System/Clock.hpp>

struct TimerBombermanComponent : public ecs::Component
{
    TimerBombermanComponent()
    {
        muiTypeComponent = TIMER_BOMBER_COMPONENT;
    }

    void displayComponent()const override
    {}
    sf::Clock mBombClock;
    bool mLaunched = false;
    sf::Clock mBombClockB;
    sf::Clock mBombClockC;
    sf::Clock mBombClockD;
};

#endif // TIMERBOMBERMANCOMPONENT_HPP
