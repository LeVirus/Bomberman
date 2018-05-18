#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <SFML/Network.hpp>

using vectPairIpPort = std::vector<std::pair<sf::IpAddress, unsigned int>>;

class BaseSocket
{
protected:
    unsigned int m_port;
    sf::UdpSocket m_socket;
    std::string m_data;
    vectPairIpPort m_vectDestination;
public:
    BaseSocket();
    BaseSocket(unsigned int port);
    void broadcastData();
    bool sendData(unsigned int num);
    bool sendData(const sf::IpAddress &ipAdress, unsigned int port);
    bool setListener();
    bool checkReceiveData();
    void addDestination(const sf::IpAddress &ipAdress, unsigned int port);
};

#endif // BASESOCKET_HPP
