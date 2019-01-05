#ifndef SERVERSOCKETSYSTEM_HPP
#define SERVERSOCKETSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"
#include "basesocket.hpp"

class SocketSystem : public ecs::System, BaseSocket
{
private:
    void serializeEntitiesData();
    void serializeBombermanEntity(unsigned int entityNum, unsigned int networkID);
public:
    SocketSystem();
    void syncClientNetworkID();
    virtual void execSystem();
    virtual void displaySystem()const;
};

#endif // SERVERSOCKETSYSTEM_HPP
