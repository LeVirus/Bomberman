#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <SFML/Network.hpp>

using vectPairIpPort = std::vector<std::pair<sf::IpAddress, unsigned short>>;

class BaseSocket
{
protected:
    unsigned short m_port;
    sf::UdpSocket m_socket;
    char *m_data = nullptr;
    vectPairIpPort m_vectDestination;
    void clearBuffer();
public:
    BaseSocket();
    BaseSocket(unsigned short port);
    void broadcastData();
    bool sendData(unsigned int num);
    bool sendData(const sf::IpAddress &ipAdress, unsigned short port);
    bool setListener();
    bool waitForReceiveData();
    void addDestination(const sf::IpAddress &ipAdress, unsigned int port);
    ~BaseSocket();
};

#endif // BASESOCKET_HPP
