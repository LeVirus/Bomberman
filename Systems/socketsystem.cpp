#include "socketsystem.hpp"
#include "networkcomponent.hpp"
#include "positioncomponent.hpp"
#include "networkserialstruct.hpp"
#include "ECSconstantes.hpp"
#include "networkserialstruct.hpp"
#include <cstring>
#include <cassert>

SocketSystem::SocketSystem()
{
    if(! bAddComponentToSystem(NETWORK_BOMBER_COMPONENT))
    {
        std::cout << "Erreur SocketSystem ajout BOMB_CONFIG_BOMBER_COMPONENT.\n";
    }
}

void SocketSystem::syncClientNetworkID()
{
    System::execSystem();
    m_data.clear();
    serializeEntitiesData();
    sendData("127.0.0.1", 54000);//SERVER SEND TO CLIENT
}

void SocketSystem::unserializeEntitiesData()
{
    if(m_data.empty())
    {
        return;
    }
    switch(m_data[0])
    {
    case NetworkTypeEntity::TYPE_BOMBERMAN:
//        serializeBombermanEntity(mVectNumEntity[i], networkComp->mNetworkId);
        break;
    case NetworkTypeEntity::TYPE_BOMB:
        break;
    case NetworkTypeEntity::TYPE_DESTRUCTIVE_WALL:
        break;
    }
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
        case NetworkTypeEntity::TYPE_BOMBERMAN:
            serializeBombermanEntity(mVectNumEntity[i], networkComp->mNetworkId);
            break;
        case NetworkTypeEntity::TYPE_BOMB:
            break;
        case NetworkTypeEntity::TYPE_DESTRUCTIVE_WALL:
            break;
        }
    }
}

void SocketSystem::serializeBombermanEntity(unsigned int entityNum, unsigned int networkID)
{
    NetworkData bombermanData;
    bombermanData.mEntityType = NetworkTypeEntity::TYPE_BOMBERMAN;
    bombermanData.mNetworkID = networkID;
    ecs::PositionComponent* posComp = stairwayToComponentManager().
            searchComponentByType<ecs::PositionComponent>(entityNum, ecs::POSITION_COMPONENT);
    assert(posComp && "posComp == NULL\n");
    bombermanData.mPosX = posComp->vect2DPosComp.mfX;
    bombermanData.mPosY = posComp->vect2DPosComp.mfY;
    char *buffer = new char[sizeof(NetworkData)];
    memcpy(buffer, &bombermanData, sizeof(NetworkData));
    m_data += std::string(buffer);
    delete buffer;
}

void SocketSystem::execSystem()
{
    //System::execSystem();
    //m_data.clear();
    //Ajouter les input
    //serializeEntitiesData();
    //sendData("127.0.0.1", 54000);
}

void SocketSystem::displaySystem() const
{

}
