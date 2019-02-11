#include "socketsystem.hpp"
#include "networkcomponent.hpp"
#include "positioncomponent.hpp"
#include "networkserialstruct.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"
#include "networkserialstruct.hpp"
#include "flagcomponent.hpp"
#include "jeu.hpp"
#include <cstring>
#include <cassert>
#include <thread>
#include <chrono>

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
}

void SocketSystem::launchReceptThread(bool memMetaData)
{
    if(mDataReceptThread.joinable())
    {
        delThread();
    }
    mDataReceptThread = std::thread(&SocketSystem::threadReception, this, memMetaData);
}

void SocketSystem::threadReception(bool memMetaData)
{
    mThreadContinue = true;
    do
    {
        if(!m_bufferReceptCursor)
        {
            receiveData(memMetaData);
        }
    }while(mThreadContinue);
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
    serializeEntitiesData(true);
    broadcastData();
    clearSendBuffer();
}

void SocketSystem::clientSyncPlayerID()
{
    receiveData(false);
    assert(m_bufferReceptCursor == sizeof(mProcessPlayerIdentity));
    memcpy(&mProcessPlayerIdentity, &m_ReceptData[0], sizeof(mProcessPlayerIdentity));
}

void SocketSystem::serverSyncClientsGlobal(const Niveau &level)
{
    synchronizeProcessPlayersNetworkID();
    synchronizeLevelToClients(level); // send level info to client(s)
    serverSyncInitPlayersEntityToClient(); // send players info to client(s)
    launchReceptThread(false);
    mProcessPlayerIdentity = Players::P_SERVER;
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

void SocketSystem::serializeEntitiesData(bool sendAllPlayersEntities = false)
{
    clearSendBuffer();
    for(size_t i = 0; i < mVectNumEntity.size(); ++i)
    {
        NetworkBombermanComponent* networkComp = stairwayToComponentManager().
                searchComponentByType<NetworkBombermanComponent>(mVectNumEntity[i], NETWORK_BOMBER_COMPONENT);
        assert(networkComp && "BombBombermanSystem::execSystem :: timerComp == NULL\n");

        if(!sendAllPlayersEntities)
        {
            if((Jeu::getGameMode() == GameMode::SERVER && mProcessPlayerIdentity != i) ||
                    (Jeu::getGameMode() == GameMode::CLIENT && mProcessPlayerIdentity != i))
                //A modifier pour les clients a b ou c cerr
            {
                continue;
            }
        }
        if(networkComp->mEntityType == TypeEntityFlag::FLAG_BOMBERMAN)
        {
            //        case TypeEntityFlag::FLAG_BOMBERMAN:
            serializeBombermanEntity(mVectNumEntity[i], networkComp->mNetworkId);
            //            break;
            //        case TypeEntityFlag::FLAG_BOMB:
            //            break;
            //        case TypeEntityFlag::FLAG_SOLID_WALL:
            //            break;
        }
    }
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

void SocketSystem::serializeBombermanEntity(unsigned int entityNum, unsigned int networkID)
{
    NetworkData bombermanData;
    bombermanData.mEntityType = TypeEntityFlag::FLAG_BOMBERMAN;
    bombermanData.mNetworkID = networkID;
    ecs::PositionComponent* posComp = stairwayToComponentManager().
            searchComponentByType<ecs::PositionComponent>(entityNum, ecs::POSITION_COMPONENT);
    assert(posComp && "posComp == NULL\n");
    bombermanData.mPosX = posComp->vect2DPosComp.mfX;
    bombermanData.mPosY = posComp->vect2DPosComp.mfY;
    addSerializeData(&bombermanData, sizeof (bombermanData));
}

void SocketSystem::clientUpdateEntitiesFromServer()
{
    if(!m_bufferReceptCursor)
    {
        return;
    }
    for(size_t i = 0; i < m_bufferReceptCursor; i += sizeof(NetworkData))
    {
        NetworkData networkData;
        assert(i + sizeof(NetworkData) < SOCKET_DATA_SIZE);
        memcpy(&networkData, &m_ReceptData[i], sizeof(NetworkData));
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
}

void SocketSystem::execSystem()
{
    clearSendBuffer();
    System::execSystem();
    clientUpdateEntitiesFromServer();
    serializeEntitiesData();
    if(Jeu::getGameMode() == GameMode::SERVER)
    { 
        broadcastData();
    }
    else
    {
        sendData("127.0.0.1", SERVER_PORT);
    }
    clearReceptBuffer();
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
