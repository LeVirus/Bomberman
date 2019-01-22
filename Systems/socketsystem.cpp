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

SocketSystem::SocketSystem()
{
    if(!bAddComponentToSystem(NETWORK_BOMBER_COMPONENT))
    {
        std::cout << "Erreur SocketSystem ajout BOMB_CONFIG_BOMBER_COMPONENT.\n";
    }
}

bool SocketSystem::clientSyncNetworkID()
{
    if(!sizeof (m_data))
    {
        return false;
    }
    System::execSystem();

    for(size_t i = 0; i < m_bufferCursor; i += sizeof(NetworkData))
    {
        NetworkData networkData;
        assert(i + sizeof(NetworkData) < SOCKET_DATA_SIZE);
        memcpy(&networkData, &m_data[i], sizeof(NetworkData));
        for(size_t j = 0; j < mVectNumEntity.size(); ++j)
        {
            FlagBombermanComponent* flagComp  = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
                    (mVectNumEntity[j], FLAG_BOMBER_COMPONENT);
            assert(flagComp && "flagComp == NULL");
            ecs::PositionComponent *posComp = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
                    (mVectNumEntity[j], ecs::POSITION_COMPONENT);
            assert(posComp && "posComp == NULL");
            if(networkData.mEntityType == flagComp->muiNumFlag )
            {

                int posX = static_cast<int>(posComp->vect2DPosComp.mfX);
                int posXNetwork = static_cast<int>(networkData.mPosX);
                int posY = static_cast<int>(posComp->vect2DPosComp.mfY);
                int posYNetwork = static_cast<int>(networkData.mPosY);
                if(posX == posXNetwork && posY == posYNetwork)
                {
                    NetworkBombermanComponent* netComp  = stairwayToComponentManager().searchComponentByType <NetworkBombermanComponent>
                            (mVectNumEntity[j], NETWORK_BOMBER_COMPONENT);
                    assert(netComp && "netComp == NULL");
                    //Synch num entity
                    netComp->mNetworkId = networkData.mNetworkID;
                    std::cerr << posX << std::endl;
                    std::cerr << posY << std::endl;
                    std::cerr << netComp->mNetworkId << std::endl;
                    break;
                }
            }
        }
    }
    return true;
}

bool SocketSystem::clientSyncNetworkLevel(NetworkLevelData &levelData)
{
    if(!sizeof (m_data))
    {
        return false;
    }
    memcpy(&levelData, &m_data[0], sizeof(NetworkLevelData));
    return true;
}

void SocketSystem::serializeEntitiesData()
{
    this->m_bufferCursor = 0;
    for(size_t i = 0; i < mVectNumEntity.size(); ++i)
    {
        NetworkBombermanComponent* networkComp = stairwayToComponentManager().
                searchComponentByType<NetworkBombermanComponent>(mVectNumEntity[i], NETWORK_BOMBER_COMPONENT);
        assert(networkComp && "BombBombermanSystem::execSystem :: timerComp == NULL\n");

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
    NetworkLevelData levelData;
    levelData.mHeight = level.getLongueurNiveau();
    levelData.mLenght = level.getLargeurNiveau();
    levelData.mLenghtTile = level.getLongueurTile();
    levelData.mHeightTile= level.getLargeurTile();
    levelData.mLevelArray = level.getLevelArray().getTab();
    clearBuffer();
    addSerializeData(&levelData, sizeof (levelData));
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
    if(!sizeof (m_data))
    {
        return;
    }
    for(size_t i = 0; i < m_bufferCursor; i += sizeof(NetworkData))
    {
        NetworkData networkData;
        assert(i + sizeof(NetworkData) < SOCKET_DATA_SIZE);
        memcpy(&networkData, &m_data[i], sizeof(NetworkData));
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
    System::execSystem();
    if(Jeu::getGameMode() == GameMode::SERVER)
    {
        serializeEntitiesData();
        sendData("127.0.0.1", 54000);
    }
    else if(Jeu::getGameMode() == GameMode::CLIENT)
    {
        receiveData();
        clientUpdateEntitiesFromServer();
    }

}

void SocketSystem::synchronizeLevelToClients(const Niveau &level)
{
    serializeLevelData(level);
    sendData("127.0.0.1", 54000);
}
