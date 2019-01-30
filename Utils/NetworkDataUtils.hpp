#ifndef NETWORKDATAUTILS_H
#define NETWORKDATAUTILS_H

#include <cstdint>
#include <iostream>
#include "constants.hpp"

class NetworkDataUtils
{
protected:
    uint8_t m_ReceptData[SOCKET_DATA_SIZE];
    uint8_t m_SendData[SOCKET_DATA_SIZE];
    unsigned long m_bufferReceptCursor = 0;
    unsigned long m_bufferSendCursor = 0;
public:
    NetworkDataUtils();
    bool addSerializeData(const void *newData, unsigned int size);
    inline void clearReceptBuffer(){m_bufferReceptCursor = 0;}
    inline void clearSendBuffer(){m_bufferSendCursor = 0;}
    inline const uint8_t* getReceptBuffer()const{return m_ReceptData;}
    inline unsigned long getBufferReceptSize()const{return m_bufferReceptCursor;}
};

#endif // NETWORKDATAUTILS_H
