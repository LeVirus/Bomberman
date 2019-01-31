#include "basesocket.hpp"
#include "networkserialstruct.hpp"
#include "jeu.hpp"
#include <iostream>
#include <string.h>


BaseSocket::BaseSocket(): m_port(SERVER_PORT)
{
    if(Jeu::getGameMode() == GameMode::CLIENT)
    {
        m_port = CLIENT_PORT;
    }
    setListener();
    m_socket.setBlocking(true);
}

BaseSocket::BaseSocket(unsigned short port): m_port(port)
{
    m_socket.setBlocking(true);
}

void BaseSocket::broadcastData()
{
    for(size_t i = 0; i < m_vectDestination.size(); ++i)
    {
        if(sendData(m_vectDestination[i].first, m_vectDestination[i].second))
        {
            // error...
        }
    }
}

bool BaseSocket::sendData(unsigned int num)
{
    if(num >= m_vectDestination.size())
    {
        return false;
    }
    return sendData(m_vectDestination[num].first, m_vectDestination[num].second);
}

bool BaseSocket::sendData(const sf::IpAddress &ipAdress, unsigned short port)
{
    if(m_socket.send(m_SendData, m_bufferSendCursor, ipAdress, port) != sf::Socket::Done)
    {
        return false;
    }
    return true;
}

bool BaseSocket::setListener()
{
    if(Jeu::getGameMode() == GameMode::SERVER)
    {
        return m_socket.bind(m_port) != sf::Socket::Done;
    }
    else if(Jeu::getGameMode() == GameMode::CLIENT)
    {
        while(m_socket.bind(m_port) != sf::Socket::Done)
        {
            ++m_port;
        }
    }
    return true;
}

bool BaseSocket::checkExistingClient(const pairIpPort &clientMetadata)
{
    for(size_t i = 0 ; i < m_vectDestination.size(); ++i)
    {
        if(clientMetadata == m_vectDestination[i])
        {
            return true;
        }
    }
    return false;
}

bool BaseSocket::receiveData(bool memMetaData)
{
    mMutex.lock();
//    clearReceptBuffer();
    size_t sizeReceived;
    sf::IpAddress ipSender;
    unsigned short senderPort;
    //wait while receive data CLIENT
    std::cout << "Waiting for receiving... " << std::endl;
    if (m_socket.receive(m_ReceptData, sizeof(m_ReceptData), sizeReceived, ipSender, senderPort) != sf::Socket::Done)
    {
        mMutex.unlock();
        return false;
    }
    //MAX_PLAYER - 1 for SERVER
    if(memMetaData && m_vectDestination.size() < MAX_PLAYER - 1 &&
            !checkExistingClient({ipSender, senderPort}))
    {
        m_vectDestination.push_back({ipSender, senderPort});
        std::cout << "Client ip :: " << ipSender << " senderPort " << senderPort << std::endl;
    }
    m_bufferReceptCursor = sizeReceived;
    std::cout << "Received " << sizeReceived << " bytes from " << ipSender << " on port " << senderPort << std::endl;
    mMutex.unlock();
    return true;
}
