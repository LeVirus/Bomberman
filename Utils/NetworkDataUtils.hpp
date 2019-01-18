#ifndef NETWORKDATAUTILS_H
#define NETWORKDATAUTILS_H

#include <cstdint>
#include "constants.hpp"

class NetworkDataUtils
{
protected:
    uint8_t m_data[SOCKET_DATA_SIZE];
    unsigned int m_bufferSize = 0;
public:
    NetworkDataUtils();
    bool addSerializeData(const void *newData, unsigned int size);
    void clearBuffer();
    const uint8_t* getBuffer() const;
    unsigned int getBufferSize()const{return m_bufferSize;}
};

#endif // NETWORKDATAUTILS_H
