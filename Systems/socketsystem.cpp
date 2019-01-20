#include "socketsystem.hpp"
#include "networkcomponent.hpp"
#include "positioncomponent.hpp"
#include "networkserialstruct.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"
#include "networkserialstruct.hpp"
#include "flagcomponent.hpp"
#include <cstring>
#include <cassert>

SocketSystem::SocketSystem()
{
    if(!bAddComponentToSystem(NETWORK_BOMBER_COMPONENT))
    {
        std::cout << "Erreur SocketSystem ajout BOMB_CONFIG_BOMBER_COMPONENT.\n";
    }
}

void SocketSystem::serverSyncClientNetworkID()
{
    System::execSystem();
    serializeEntitiesData();
    sendData("127.0.0.1", 54000);//SERVER SEND TO CLIENT
}

bool SocketSystem::clientSyncNetworkID()
{
    if(!sizeof (m_data))
    {
        return false;
    }
    System::execSystem();

    for(size_t i = 0; i < m_bufferCursor; i += NETWORK_BLOC_DATA_SIZE)
    {
        NetworkData networkData;
        assert(i + NETWORK_BLOC_DATA_SIZE < SOCKET_DATA_SIZE);
        memcpy(&networkData, &m_data[i], NETWORK_BLOC_DATA_SIZE);
        std::cerr << "posX " << i << std::endl;

        for(size_t j = 0; j < mVectNumEntity.size(); ++j)
        {
            std::cerr << "possaaX " << j << std::endl;
            FlagBombermanComponent* flagComp  = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
                    (mVectNumEntity[i], FLAG_BOMBER_COMPONENT);
            //assert(flagComp && "flagComp == NULL");
            NetworkBombermanComponent* netComp  = stairwayToComponentManager().searchComponentByType <NetworkBombermanComponent>
                    (mVectNumEntity[i], NETWORK_BOMBER_COMPONENT);
            //assert(netComp && "netComp== NULL");
            ecs::PositionComponent *posComp = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
                    (mVectNumEntity[i], ecs::POSITION_COMPONENT);
            //assert(posComp && "posComp == NULL");
            if(!flagComp || !netComp || !posComp)
            {
                continue;
            }

            if(networkData.mEntityType == flagComp->muiNumFlag)
            {
                int posX = static_cast<int>(networkData.mPosX);
                std::cerr << posX << std::endl;
                int posY = static_cast<int>(networkData.mPosY);
                std::cerr << posY << std::endl;

                //if(networkData.mPosX == )
            }
        }
    }
    return true;
}

void SocketSystem::serializeEntitiesData()
{
    for(size_t i = 0; i < mVectNumEntity.size(); ++i)
    {
        NetworkBombermanComponent* networkComp = stairwayToComponentManager().
                searchComponentByType<NetworkBombermanComponent>(mVectNumEntity[i], NETWORK_BOMBER_COMPONENT);
        assert(networkComp && "BombBombermanSystem::execSystem :: timerComp == NULL\n");

        switch(networkComp->mEntityType)
        {
        case TypeEntityFlag::FLAG_BOMBERMAN:
            serializeBombermanEntity(mVectNumEntity[i], networkComp->mNetworkId);
            break;
        case TypeEntityFlag::FLAG_BOMB:
            break;
        case TypeEntityFlag::FLAG_SOLID_WALL:
            break;
        }
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

void SocketSystem::execSystem()
{
    //System::execSystem();
    //m_data.clear();
    //Ajouter les input
    //serializeEntitiesData();
    //sendData("127.0.0.1", 54000);
}
