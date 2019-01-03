#ifndef SERVERSOCKETSYSTEM_HPP
#define SERVERSOCKETSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"
#include "basesocket.hpp"

class ServerSocketSystem : public ecs::System, BaseSocket
{
public:
    ServerSocketSystem();
    void syncClientNetworkID();
    virtual void execSystem();
    virtual void displaySystem()const;
};

#endif // SERVERSOCKETSYSTEM_HPP
