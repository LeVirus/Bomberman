#include "basesocket.hpp"

BaseSocket::BaseSocket(): m_port(54000), m_ipAdress("192.168.0.1")
{
    setListener();
}

BaseSocket::BaseSocket(const sf::IpAddress &ipAdress, unsigned int port): m_port(port), m_ipAdress(ipAdress)
{
    setListener();
}

void BaseSocket::broadcastData()
{
    for(size_t i = 0; i < m_vectDestination.size(); ++i)
    {
        if (m_socket.send(m_data.c_str(), m_data.size(), m_vectDestination[i].first, m_vectDestination[i].second) != sf::Socket::Done)
        {
            // error...
        }
    }
}

bool BaseSocket::setListener()
{
    return m_socket.bind(m_port) != sf::Socket::Done;
}

void BaseSocket::addDestination(const sf::IpAddress &ipAdress, unsigned int port)
{
    m_vectDestination.push_back({ipAdress, port});
}
