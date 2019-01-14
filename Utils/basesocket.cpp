#include "basesocket.hpp"
#include "networkserialstruct.hpp"
#include <iostream>

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
    std::cerr << sizeof (m_data) << std::endl;
    if(!m_data || m_socket.send(m_data, sizeof (m_data), ipAdress, port) != sf::Socket::Done)
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
    char *data = new char[sizeof(NetworkData)];
    sf::IpAddress ipSender;
    unsigned short senderPort;
//    m_socket.setBlocking(true);
    setListener();
    //wait while receive data CLIENT
    if (m_socket.receive(data, sizeof(NetworkData), sizeReceived, ipSender, senderPort) != sf::Socket::Done)
    {
        return false;
    }
    std::cout << "Received " << sizeReceived << " bytes from " << ipSender << " on port " << senderPort << std::endl;
    m_socket.setBlocking(false);
    delete[] data;
    return true;
}

void BaseSocket::addDestination(const sf::IpAddress &ipAdress, unsigned int port)
{
    m_vectDestination.push_back({ipAdress, port});
}

void BaseSocket::clearBuffer()
{
    if(m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

BaseSocket::~BaseSocket()
{
    clearBuffer();
}
