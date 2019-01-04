#include "basesocket.hpp"
#include <iostream>

BaseSocket::BaseSocket(): m_port(54000)
{
    m_data.resize(100);
    //m_socket.setBlocking(false);
}

BaseSocket::BaseSocket(unsigned int port): m_port(port)
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
    if(m_socket.send(m_data.c_str(), m_data.size(), m_vectDestination[num].first, m_vectDestination[num].second) != sf::Socket::Done)
    {
        return false;
    }
    return true;
}

bool BaseSocket::sendData(const sf::IpAddress &ipAdress, unsigned int port)
{
    if(m_socket.send(m_data.c_str(), m_data.size(), ipAdress, port) != sf::Socket::Done)
    {
        return false;
    }
    return true;
}

bool BaseSocket::setListener()
{
    return m_socket.bind(m_port) != sf::Socket::Done;
}

bool BaseSocket::checkReceiveData()
{
    size_t sizeReceived;
    char data[100];
    sf::IpAddress ipSender;
    unsigned short senderPort;
    //wait while receive data
    if (m_socket.receive(data, 100, sizeReceived, ipSender, senderPort) != sf::Socket::Done)
    {
        return false;
    }
    std::cout << "Received " << sizeReceived << " bytes from " << ipSender << " on port " << senderPort << std::endl;
    return true;
}

void BaseSocket::addDestination(const sf::IpAddress &ipAdress, unsigned int port)
{
    m_vectDestination.push_back({ipAdress, port});
}
