#ifndef SERVERSOCKETSYSTEM_HPP
#define SERVERSOCKETSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"
#include "basesocket.hpp"

struct NetworkData;
struct NetworkLevelData;
class Niveau;

class SocketSystem : public ecs::System, public BaseSocket
{
private:
    void serializeEntitiesData();
    void serializeLevelData(const Niveau &level);
    void addPlayersConf(NetworkLevelData &levelData);
    void serializeBombermanEntity(unsigned int entityNum, unsigned int networkID);
    void clientUpdateEntitiesFromServer();
public:
    SocketSystem();
    bool clientSyncNetworkID();
    bool clientSyncNetworkLevel(NetworkLevelData &levelData);
    virtual void execSystem();
    void synchronizeLevelToClients(const Niveau &level);
    virtual void displaySystem()const{}
    virtual ~SocketSystem() = default;
};

#endif // SERVERSOCKETSYSTEM_HPP
