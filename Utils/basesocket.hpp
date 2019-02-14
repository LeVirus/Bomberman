#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <SFML/Network.hpp>
#include "NetworkDataUtils.hpp"
#include <mutex>

using pairIpPort = std::pair<sf::IpAddress, unsigned short>;
using vectPairIpPort = std::vector<pairIpPort>;

class SocketSystem;

class BaseSocket: public NetworkDataUtils
{
protected:
    bool checkExistingClient(const pairIpPort &clientMetadata);
    inline void memPtrSocketSystem(SocketSystem *ss){mSockSys = ss;}

    unsigned short m_port;
    sf::UdpSocket m_socket;
    vectPairIpPort m_vectDestination;
    std::mutex mMutex;
    bool mThreadContinue = true;
    SocketSystem *mSockSys = nullptr;
public:
    BaseSocket();
    BaseSocket(unsigned short port);
    void broadcastData();
    bool sendData(unsigned int num);
    bool sendData(const sf::IpAddress &ipAdress, unsigned short port);
    bool setListener();
    bool receiveData(bool memMetaData = false, bool loop = true);
    inline size_t getDestinationsNumber()const{return m_vectDestination.size();}
};

#endif // BASESOCKET_HPP
