#pragma once


namespace Journey {
    class AudioSourceComponent;
    class AudioManager;

    class AudioBuffer {
    public:
        AudioBuffer(unsigned int id, unsigned int buffer);
        unsigned int GetId();
        unsigned int GetBuffer();
    private:
        friend class AudioSourceComponent;
        friend class AudioManager;
        unsigned int m_Id;
        unsigned int mBuffer;
    };

}
