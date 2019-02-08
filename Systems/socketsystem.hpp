#ifndef SERVERSOCKETSYSTEM_HPP
#define SERVERSOCKETSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"
#include "basesocket.hpp"
#include <thread>

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
    void threadReception(bool memMetaData);
    void delThread();
    std::thread mDataReceptThread;
    bool mThreadContinue = true;
    uint32_t mNetworkIdPlayer;
    Players mPlayerIdentity;
public:
    SocketSystem();
    void launchReceptThread(bool memMetaData = false);
    bool clientSyncNetworkID();
    void attributePlayerNetworkID(uint32_t networkID);
    bool clientSyncNetworkLevel(NetworkLevelData &levelData);
    void setPlayerID(Players playerId);
    void syncPlayerID();
    virtual void execSystem();
    void synchronizeLevelToClients(const Niveau &level);
    virtual void displaySystem()const{}
    virtual ~SocketSystem();
};

#endif // SERVERSOCKETSYSTEM_HPP
