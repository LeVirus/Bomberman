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
    //m_socket.setBlocking(false);
}

BaseSocket::BaseSocket(unsigned short port): m_port(port)
{
    //m_socket.setBlocking(false);
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
    return m_socket.bind(m_port) != sf::Socket::Done;
}

bool BaseSocket::receiveData(bool waitForServer)
{
    mMutex.lock();
    clearReceptBuffer();
    size_t sizeReceived;
    sf::IpAddress ipSender;
    unsigned short senderPort;
    //clearBuffer();
    if(waitForServer)
    {
        m_socket.setBlocking(true);
    }
    else
    {
        m_socket.setBlocking(false);
    }
    setListener();
    //wait while receive data CLIENT
    std::cout << "Waiting for receiving... " << std::endl;
    if (m_socket.receive(m_ReceptData, sizeof(m_ReceptData), sizeReceived, ipSender, senderPort) != sf::Socket::Done)
    {
        mMutex.unlock();
        return false;
    }
    m_bufferReceptCursor = sizeReceived;
    std::cout << "Received " << sizeReceived << " bytes from " << ipSender << " on port " << senderPort << std::endl;
    mMutex.unlock();
    return true;
}

void BaseSocket::addDestination(const sf::IpAddress &ipAdress, unsigned short port)
{
    m_vectDestination.push_back({ipAdress, port});
}
