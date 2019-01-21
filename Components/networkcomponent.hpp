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
    TypeEntityFlag mEntityType;
    virtual ~NetworkBombermanComponent() = default;
    static unsigned int attributeNum()
    {
        static unsigned int mExistingNum = 0;
        return ++mExistingNum;
    }
};

#endif // NETWORKCOMPONENT_HPP

