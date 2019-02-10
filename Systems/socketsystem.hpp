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

    /**
     * @brief synchronizeProcessPlayersNetworkID
     * Send process player ID to each connected client, in order of connections.
     */
    void synchronizeProcessPlayersNetworkID();
    void synchronizeLevelToClients(const Niveau &level);

    std::thread mDataReceptThread;
    bool mThreadContinue = true;
    uint32_t mNetworkIdPlayer;
    Players mProcessPlayerIdentity;
public:
    SocketSystem();
    bool clientSyncNetworkID();
    void attributePlayerNetworkID(uint32_t networkID);
    bool clientSyncNetworkLevel(NetworkLevelData &levelData);
    void clientSyncPlayerID();
    void serverSyncClientsGlobal(const Niveau &level);
    void launchReceptThread(bool memMetaData = false);
    inline Players getProcessPlayerID()const{return mProcessPlayerIdentity;}
    virtual void execSystem();
    virtual void displaySystem()const{}
    virtual ~SocketSystem();
};

#endif // SERVERSOCKETSYSTEM_HPP
