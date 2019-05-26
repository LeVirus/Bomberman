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
    void clientAddServerIPAndPort();
    void serializeLevelData(const Niveau &level);
    void addPlayersConf(NetworkLevelData &levelData);
    void serializeBombermanEntity(uint32_t entityNum, uint32_t networkID);
    void serializeBombEntity(uint32_t entityNum);
    void serializeCommonDataEntity(uint32_t entityNum, uint32_t networkID,
                                   NetworkData &bombData);
    void delThread();
    void serverSyncInitPlayersEntityToClient();
    void updateBombermanEntity(NetworkData &networkData);
    void updateBombEntity(NetworkData &networkData);

    /**
     * @brief synchronizeProcessPlayersNetworkID
     * Send process player ID to each connected client, in order of connections.
     */
    void synchronizeProcessPlayersNetworkID();
    void synchronizeLevelToClients(const Niveau &level);

    std::thread mDataReceptThread;
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
    void updateEntitiesFromRemote();
    inline Players getProcessPlayerID()const{return mProcessPlayerIdentity;}
    inline void setProcessPlayerID(Players newID){mProcessPlayerIdentity = newID;}
    virtual void execSystem();
    virtual void displaySystem()const{}
    virtual ~SocketSystem();
};

#endif // SERVERSOCKETSYSTEM_HPP
