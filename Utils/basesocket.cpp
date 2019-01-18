#include "basesocket.hpp"
#include "networkserialstruct.hpp"
#include <iostream>
#include <string.h>

BaseSocket::BaseSocket(): m_port(54000)
{
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
    if(m_socket.send(m_data, m_bufferSize, ipAdress, port) != sf::Socket::Done)
    {
        return false;
    }
    return true;
}

bool BaseSocket::setListener()
{
    return m_socket.bind(m_port) != sf::Socket::Done;
}

bool BaseSocket::waitForReceiveData()
{
    size_t sizeReceived;
    uint8_t data[500];
    sf::IpAddress ipSender;
    unsigned short senderPort;
    m_socket.setBlocking(true);
    setListener();
    //wait while receive data CLIENT
    std::cout << "Wait for receiving ... " << std::endl;
    if (m_socket.receive(data, 500, sizeReceived, ipSender, senderPort) != sf::Socket::Done)
    {
        return false;
    }
    std::cout << "Received " << sizeReceived << " bytes from " << ipSender << " on port " << senderPort << std::endl;
    m_socket.setBlocking(false);
    return true;
}

void BaseSocket::addDestination(const sf::IpAddress &ipAdress, unsigned int port)
{
    m_vectDestination.push_back({ipAdress, port});
}

BaseSocket::~BaseSocket()
{
    clearBuffer();
}
