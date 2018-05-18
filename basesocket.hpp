#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <SFML/Network.hpp>

using vectPairIpPort = std::vector<std::pair<sf::IpAddress, unsigned int>>;

class BaseSocket
{
private:
    unsigned int m_port;
    sf::IpAddress m_ipAdress;
    sf::UdpSocket m_socket;
    std::string m_data;
    vectPairIpPort m_vectDestination;
public:
    BaseSocket();
    BaseSocket(const sf::IpAddress &ipAdress, unsigned int port);
    void broadcastData();
    bool setListener();
    void addDestination(const sf::IpAddress &ipAdress, unsigned int port);
};

#endif // BASESOCKET_HPP
