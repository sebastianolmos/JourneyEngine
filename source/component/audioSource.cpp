#include "audioSource.hpp"
#include "../audio/audioBuffer.hpp"

#include <iostream>

#ifndef AL_CALL_TMPL
#define AL_CALL_TMPL

#include <stdexcept> 
#include <exception>  
// Checkeo de errores para las llamadas de OpenAL
template<auto ALFunction, class... Args>
void ALCall(Args... args) {
    alGetError();
    ALFunction(args...);
    auto error = alGetError();
    if (error) {
        switch (error) {
#ifndef CASE
#define CASE(X) case X: throw std::runtime_error("[OpenAL Error] " #X);
            CASE(AL_INVALID_NAME)
            CASE(AL_INVALID_ENUM)
            CASE(AL_INVALID_VALUE)
            CASE(AL_INVALID_OPERATION)
            CASE(AL_OUT_OF_MEMORY)
            default:
                throw std::runtime_error("Unknown OpenAL error");
        }
    }
}
#endif
#endif

namespace Journey{
    void AudioSourceComponent::play()
    {
        ALCall<alSourcePlay>(mBuffer->m_id);
    }

    void AudioSourceComponent::pause()
    {
        ALCall<alSourcePause>(mBuffer->m_id);
    }

    void AudioSourceComponent::stop()
    {
        ALCall<alSourceStop>(mBuffer->m_id);
    }

    bool AudioSourceComponent::getLooping()
    {
        int b;
        ALCall<alGetSourcei>(mBuffer->m_id, AL_LOOPING, &b);
        mLoop = b;
        return b;
    }
    void AudioSourceComponent::setLooping(bool b)
    {
        ALCall<alSourcei>(mBuffer->m_id, AL_LOOPING, b);
    }

    void AudioSourceComponent::setVolume(float v)
    {
        ALCall<alSourcef>(mBuffer->m_id, AL_GAIN, v);
    }

    bool AudioSourceComponent::isPlaying() const
    {
        ALint source_state;
	    ALCall<alGetSourcei>(mBuffer->m_id, AL_SOURCE_STATE, &source_state);
	    return source_state == AL_PLAYING;
    }

    void AudioSourceComponent::setBuffer(std::shared_ptr<AudioBuffer> buffer)

    {
        ALCall<alSourcei>(mBuffer->m_id, AL_BUFFER, buffer->m_id);
        mBuffer = std::move(buffer);
    }

    std::shared_ptr<AudioBuffer> AudioSourceComponent::getBuffer()
    {
        return mBuffer;
    }
}

#ifdef CASE
#undef CASE
#endif
