#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <SFML/Network.hpp>
#include "NetworkDataUtils.hpp"

using vectPairIpPort = std::vector<std::pair<sf::IpAddress, unsigned short>>;

class BaseSocket: public NetworkDataUtils
{
protected:
    unsigned short m_port;
    sf::UdpSocket m_socket;
    vectPairIpPort m_vectDestination;
public:
    BaseSocket();
    BaseSocket(unsigned short port);
    void broadcastData();
    bool sendData(unsigned int num);
    bool sendData(const sf::IpAddress &ipAdress, unsigned short port);
    bool setListener();
    bool waitForReceiveData();
    void addDestination(const sf::IpAddress &ipAdress, unsigned short port);
};

#endif // BASESOCKET_HPP
