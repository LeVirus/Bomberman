#include "socketsystem.hpp"
#include "networkcomponent.hpp"
#include "positioncomponent.hpp"
#include "bombconfigbombermancomponent.hpp"
#include "networkserialstruct.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"
#include "networkserialstruct.hpp"
#include "bombbombermansystem.hpp"
#include "flagcomponent.hpp"
#include "jeu.hpp"
#include <cstring>
#include <cassert>
#include <thread>
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

SocketSystem::SocketSystem()
{
    if(!bAddComponentToSystem(NETWORK_BOMBER_COMPONENT))
    {
        std::cout << "Erreur SocketSystem ajout BOMB_CONFIG_BOMBER_COMPONENT.\n";
    }
    if(Jeu::getGameMode() == GameMode::SERVER)
    {
        //launch thread to get ip and port from client
        launchReceptThread(true);
    }
    else
    {
        clientAddServerIPAndPort();
    }
    memPtrSocketSystem(this);
}

void SocketSystem::launchReceptThread(bool memMetaData)
{
    if(mDataReceptThread.joinable())
    {
        delThread();
    }
    //loop on method
    mDataReceptThread = std::thread(&SocketSystem::receiveData, this, memMetaData, true);
}

void SocketSystem::delThread()
{
    mThreadContinue = false;
    mDataReceptThread.join();
    mThreadContinue = true;
}

void SocketSystem::serverSyncInitPlayersEntityToClient()
{
    if(Jeu::getGameMode() != GameMode::SERVER)
    {
        return;
    }
    System::execSystem();
    serializeEntitiesData();
    broadcastData();
    clearSendBuffer();
}

void SocketSystem::clientSyncPlayerID()
{
    receiveData(false, false);
    assert(m_bufferReceptCursor == sizeof(mProcessPlayerIdentity));
    memcpy(&mProcessPlayerIdentity, &m_ReceptData[0], sizeof(mProcessPlayerIdentity));
    clearReceptBuffer();
}

void SocketSystem::serverSyncClientsGlobal(const Niveau &level)
{
    synchronizeProcessPlayersNetworkID();
    synchronizeLevelToClients(level); // send level info to client(s)
    serverSyncInitPlayersEntityToClient(); // send players info to client(s)
    launchReceptThread(false);
}

bool SocketSystem::clientSyncNetworkID()
{
    if(!sizeof (m_ReceptData))
    {
        return false;
    }
    System::execSystem();
    for(size_t i = 0; i < m_bufferReceptCursor; i += sizeof(NetworkData))
    {
        NetworkData networkData;
        assert(i + sizeof(NetworkData) < SOCKET_DATA_SIZE);
        memcpy(&networkData, &m_ReceptData[i], sizeof(NetworkData));
        for(size_t j = 0; j < mVectNumEntity.size(); ++j)
        {
            FlagBombermanComponent* flagComp  = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
                    (mVectNumEntity[j], FLAG_BOMBER_COMPONENT);
            assert(flagComp && "flagComp == NULL");
            ecs::PositionComponent *posComp = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
                    (mVectNumEntity[j], ecs::POSITION_COMPONENT);
            assert(posComp && "posComp == NULL");
            NetworkBombermanComponent* netComp  = stairwayToComponentManager().searchComponentByType <NetworkBombermanComponent>
                    (mVectNumEntity[j], NETWORK_BOMBER_COMPONENT);
            assert(netComp && "netComp == NULL");
            if(networkData.mEntityType == flagComp->muiNumFlag && netComp->mNetworkId == 0)
            {
                posComp->vect2DPosComp.mfX = networkData.mPosX;
                posComp->vect2DPosComp.mfY = networkData.mPosY;
                netComp->mNetworkId = networkData.mNetworkID;
                break;
            }
        }
    }
    clearReceptBuffer();
    return true;
}

void SocketSystem::attributePlayerNetworkID(uint32_t networkID)
{
    mNetworkIdPlayer = networkID;
}

bool SocketSystem::clientSyncNetworkLevel(NetworkLevelData &levelData)
{
    assert(sizeof(NetworkLevelData) != sizeof (m_ReceptData) && "Incoherent buffer size for level conf");
    if(!sizeof (m_ReceptData))
    {
        return false;
    }
    memcpy(&levelData, &m_ReceptData[0], sizeof(NetworkLevelData));
    clearReceptBuffer();
    return true;
}

void SocketSystem::serializeEntitiesData()
{
    clearSendBuffer();
    for(size_t i = 0; i < mVectNumEntity.size(); ++i)
    {

        NetworkBombermanComponent* networkComp = stairwayToComponentManager().
                searchComponentByType<NetworkBombermanComponent>(mVectNumEntity[i], NETWORK_BOMBER_COMPONENT);
        assert(networkComp && "BombBombermanSystem::execSystem :: timerComp == NULL\n");
        switch (networkComp->mEntityType)
        {
        case TypeEntityFlag::FLAG_BOMBERMAN:
            if(Jeu::getGameMode() == GameMode::CLIENT &&
                    mNetworkIdPlayer != networkComp->mNetworkId)
            {
                break;
            }
            serializeBombermanEntity(mVectNumEntity[i], networkComp->mNetworkId);
            break;
        case TypeEntityFlag::FLAG_BOMB:
            serializeBombEntity(mVectNumEntity[i]);
            break;
        case TypeEntityFlag::FLAG_SOLID_WALL:
        case TypeEntityFlag::FLAG_OBJECT:
        case TypeEntityFlag::FLAG_EXPLOSION:
        case TypeEntityFlag::FLAG_DESTRUCTIBLE_WALL:
            break;
        }
    }
}

void SocketSystem::clientAddServerIPAndPort()
{
//    sf::IpAddress
    m_vectDestination.push_back({"127.0.0.1", SERVER_PORT});
}

void SocketSystem::serializeLevelData(const Niveau &level)
{
    TilemapBombermanComponent* tileComp = stairwayToComponentManager().
            searchComponentByType<TilemapBombermanComponent>(Niveau::getNumEntityLevel(), TILEMAP_BOMBER_COMPONENT);
    assert(tileComp && "tileComp == nullptr");
    NetworkLevelData levelData;
    levelData.mHeight = level.getLongueurNiveau();
    levelData.mLenght = level.getLargeurNiveau();
    levelData.mLenghtTile = level.getLongueurTile() / SIZE_SCALE;
    levelData.mHeightTile= level.getLargeurTile() / SIZE_SCALE;
    std::copy(tileComp->mTabTilemap.getTab().begin(), tileComp->mTabTilemap.getTab().end(),
              &levelData.mLevelArray[0]);
    addPlayersConf(levelData);
    clearReceptBuffer();
    addSerializeData(&levelData, sizeof (levelData));
}

void SocketSystem::addPlayersConf(NetworkLevelData &levelData)
{
    levelData.mNumPlayers = Niveau::getNumCurrentNumberPlayer();
    for(size_t i = 0; i < levelData.mNumPlayers; ++i)
    {
        const vectPairUi_t &initPos = Niveau::static_getVectInitPositionBomberman();
        assert(levelData.mNumPlayers == initPos.size() && "Incoherent size from init position players");
        levelData.mPlayersInitPos[i] = initPos[i];
    }
}

void SocketSystem::serializeBombermanEntity(uint32_t entityNum, uint32_t networkID)
{
    NetworkData bombermanData;
    bombermanData.mEntityType = TypeEntityFlag::FLAG_BOMBERMAN;
    serializeCommonDataEntity(entityNum, networkID, bombermanData);
    addSerializeData(&bombermanData, sizeof (bombermanData));
}

void SocketSystem::serializeBombEntity(uint32_t entityNum)
{
    NetworkData bombData;
    bombData.mEntityType = TypeEntityFlag::FLAG_BOMB;
    serializeCommonDataEntity(entityNum, mNetworkIdPlayer, bombData);
    addSerializeData(&bombData, sizeof (bombData));
    if(Jeu::getGameMode() == GameMode::CLIENT)
    {
        if(mptrSystemManager->getptrEngine()->bRmEntity(entityNum))
        {
            std::cerr << "WARNING :: Entity " << entityNum << " doesn't exists" << std::endl;
        }
    }
    else // SERVER
    {
        mptrSystemManager->getptrEngine()->bRmComponentToEntity(entityNum, NETWORK_BOMBER_COMPONENT);
    }
}

void SocketSystem::serializeCommonDataEntity(uint32_t entityNum, uint32_t networkID, NetworkData &bombData)
{
    bombData.mNetworkID = networkID;// set player network id
    ecs::PositionComponent* posComp = stairwayToComponentManager().
            searchComponentByType<ecs::PositionComponent>(entityNum, ecs::POSITION_COMPONENT);
    assert(posComp && "posComp == NULL\n");
    bombData.mPosX = posComp->vect2DPosComp.mfX;
    bombData.mPosY = posComp->vect2DPosComp.mfY;
//    std::cerr << "Network ID :: " << networkID << "\n";
//    std::cerr << "SENT :: " << bombData.mPosX << "  " << bombData.mPosY << "\n";
}

void SocketSystem::updateEntitiesFromRemote()
{
//    GameMode game = Jeu::getGameMode();
    if(!m_bufferReceptCursor)
    {
        return;
    }
    NetworkData networkData;
    for(size_t i = 0; i < m_bufferReceptCursor; i += sizeof(NetworkData))
    {
        assert(i + sizeof(NetworkData) < SOCKET_DATA_SIZE);
        memcpy(&networkData, &m_ReceptData[i], sizeof(NetworkData));
        if(networkData.mEntityType == TypeEntityFlag::FLAG_BOMBERMAN)
        {
            updateBombermanEntity(networkData);
        }
        else if (networkData.mEntityType == TypeEntityFlag::FLAG_BOMB)
        {
            updateBombEntity(networkData);
        }
    }
    clearReceptBuffer();
}

void SocketSystem::updateBombermanEntity(NetworkData &networkData)
{
    //ignore sent of process player from server
    if(Jeu::getGameMode() == GameMode::CLIENT && mNetworkIdPlayer == networkData.mNetworkID)
    {
        return;
    }
    for(size_t j = 0; j < mVectNumEntity.size(); ++j)
    {
        NetworkBombermanComponent* netComp  = stairwayToComponentManager().searchComponentByType <NetworkBombermanComponent>
                (mVectNumEntity[j], NETWORK_BOMBER_COMPONENT);
        assert(netComp && "netComp == NULL");

        if(netComp->mNetworkId == networkData.mNetworkID)
        {
            ecs::PositionComponent *posComp = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
                    (mVectNumEntity[j], ecs::POSITION_COMPONENT);
            assert(posComp && "posComp == NULL");
            posComp->vect2DPosComp.mfX = networkData.mPosX;
            posComp->vect2DPosComp.mfY = networkData.mPosY;
            break;
        }
    }
}

void SocketSystem::updateBombEntity(NetworkData &networkData)
{
    bool server = false;
    for(size_t j = mVectNumEntity.size() - 1; j < mVectNumEntity.size(); --j)
    {
        NetworkBombermanComponent* netComp  = stairwayToComponentManager().searchComponentByType <NetworkBombermanComponent>
                (mVectNumEntity[j], NETWORK_BOMBER_COMPONENT);
        assert(netComp && "netComp == NULL");
        if(netComp->mNetworkId == networkData.mNetworkID)
        {
            ecs::PositionComponent posA;
            posA.vect2DPosComp.mfX = networkData.mPosX;
            posA.vect2DPosComp.mfY = networkData.mPosY;
            if(Jeu::getGameMode() == GameMode::SERVER)
            {
                server = true;
            }
            mptrSystemManager->searchSystemByType<BombBombermanSystem>(BOMB_BOMBER_SYSTEM)->
                    lauchBomb(mVectNumEntity[j], posA, server);
            break;
        }
    }
}

void SocketSystem::execSystem()
{
    clearSendBuffer();
    System::execSystem();
    serializeEntitiesData();
    if(Jeu::getGameMode() == GameMode::SERVER)
    { 
        broadcastData();
    }
    else
    {
        bool res = sendData(0);
        assert(res);
//        sendData("127.0.0.1", SERVER_PORT);
    }
}

void SocketSystem::synchronizeLevelToClients(const Niveau &level)
{
    serializeLevelData(level);
    broadcastData();
    clearSendBuffer();
}

void SocketSystem::synchronizeProcessPlayersNetworkID()
{
    assert(getDestinationsNumber() < MAX_PLAYER - 1);
    Players id = Players::P_CLIENT_A;
    for(uint32_t i = 0; i < getDestinationsNumber(); ++i)
    {
        addSerializeData(&id, sizeof (Players));
        sendData(i);
        clearSendBuffer();
        if(id == Players::P_CLIENT_A)
        {
            id = Players::P_CLIENT_B;
        }
        else if(id == Players::P_CLIENT_B)
        {
            id = Players::P_CLIENT_C;
        }
    }
}

SocketSystem::~SocketSystem()
{
    delThread();
}
