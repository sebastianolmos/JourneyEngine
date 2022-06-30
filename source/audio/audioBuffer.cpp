#include "audioBuffer.hpp"

namespace Journey {

    AudioBuffer::AudioBuffer(unsigned int id, unsigned int buffer)
    {
        m_Id = id;
        buffer = buffer;
    }

    unsigned int AudioBuffer::GetId()
    {
        return m_Id;
    }

    unsigned int AudioBuffer::GetBuffer()
    {
        return mBuffer;
    }
    
}