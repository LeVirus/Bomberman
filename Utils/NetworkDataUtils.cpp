#include "NetworkDataUtils.hpp"
#include "constants.hpp"
#include <string.h>
#include <cassert>
#include <iostream>

NetworkDataUtils::NetworkDataUtils()
{

}

bool NetworkDataUtils::addSerializeData(const void *newData, unsigned int size)
{
    if(!newData || !size)
    {
        return false;
    }
    assert(m_bufferSize + size < SOCKET_DATA_SIZE && "SOCKET BUFFER OVERFLOW");
    memcpy(&m_data[m_bufferSize], newData, size);
    m_bufferSize += size;
    return true;
}

void NetworkDataUtils::clearBuffer()
{
    m_bufferSize = 0;
}

const uint8_t* NetworkDataUtils::getBuffer() const
{
    return m_data;
}

