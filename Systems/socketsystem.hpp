#ifndef SERVERSOCKETSYSTEM_HPP
#define SERVERSOCKETSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"
#include "basesocket.hpp"

struct NetworkData;

class SocketSystem : public ecs::System, public BaseSocket
{
private:
    void serializeEntitiesData();
    void serializeBombermanEntity(unsigned int entityNum, unsigned int networkID);
public:
    SocketSystem();
    void serverSyncClientNetworkID();
    bool clientSyncNetworkID();
    virtual void execSystem();
    virtual void displaySystem()const{}
    virtual ~SocketSystem() = default;
};

#endif // SERVERSOCKETSYSTEM_HPP
