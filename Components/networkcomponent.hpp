#ifndef NETWORKCOMPONENT_HPP
#define NETWORKCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"

struct NetworkBombermanComponent : public ecs::Component
{
    NetworkBombermanComponent()
    {
        muiTypeComponent = NETWORK_BOMBER_COMPONENT;
    }

    void displayComponent()const override
    {
        //Component::displayComponent();
    }
    unsigned int mNetworkId;
    NetworkTypeEntity mEntityType;
};

#endif // NETWORKCOMPONENT_HPP

