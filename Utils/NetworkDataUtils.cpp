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
    assert(m_bufferSendCursor + size < SOCKET_DATA_SIZE && "SOCKET BUFFER OVERFLOW");
    memcpy(&m_SendData[m_bufferSendCursor], newData, size);
    m_bufferSendCursor += size;
    return true;
}
