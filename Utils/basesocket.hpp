#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <SFML/Network.hpp>
#include "NetworkDataUtils.hpp"
#include <mutex>

using pairIpPort = std::pair<sf::IpAddress, unsigned short>;
using vectPairIpPort = std::vector<pairIpPort>;

class BaseSocket: public NetworkDataUtils
{
protected:
    bool checkExistingClient(const pairIpPort &clientMetadata);
    unsigned short m_port;
    sf::UdpSocket m_socket;
    vectPairIpPort m_vectDestination;
    std::mutex mMutex;
public:
    BaseSocket();
    BaseSocket(unsigned short port);
    void broadcastData();
    bool sendData(unsigned int num);
    bool sendData(const sf::IpAddress &ipAdress, unsigned short port);
    bool setListener();
    bool receiveData(bool memMetaData = false, bool waitForServer = false);
};

#endif // BASESOCKET_HPP
