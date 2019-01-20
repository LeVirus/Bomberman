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
    assert(m_bufferCursor + size < SOCKET_DATA_SIZE && "SOCKET BUFFER OVERFLOW");
    memcpy(&m_data[m_bufferCursor], newData, size);
    m_bufferCursor += size;
    return true;
}

void NetworkDataUtils::clearBuffer()
{
    m_bufferCursor = 0;
}

const uint8_t* NetworkDataUtils::getBuffer() const
{
    return m_data;
}

