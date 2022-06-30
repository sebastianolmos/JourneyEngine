#include "audioSource.hpp"
#include "../audio/audioBuffer.hpp"

#include <iostream>
#include "AL/al.h"
#include "AL/alc.h"

#ifndef OPENALCALL
#include <cassert>
#define OPENALCALL(function)\
	function;\
	{\
		ALenum error = alGetError();\
		assert(error != AL_NO_ERROR, "OpenAL Error");\
	}
#endif

namespace Journey{
    void AudioSourceComponent::play()
    {
        OPENALCALL(alSourcePlay(mBuffer->m_Id));
    }

    void AudioSourceComponent::pause()
    {
        OPENALCALL(alSourcePause(mBuffer->m_Id));
    }

    void AudioSourceComponent::stop()
    {
        OPENALCALL(alSourceStop(mBuffer->m_Id));
    }

    bool AudioSourceComponent::getLooping()
    {
        int b;
        OPENALCALL(alGetSourcei(mBuffer->m_Id, AL_LOOPING, &b));
        mLoop = b;
        return b;
    }
    void AudioSourceComponent::setLooping(bool b)
    {
        OPENALCALL(alSourcei(mBuffer->m_Id, AL_LOOPING, b));
    }

    void AudioSourceComponent::setVolume(float v)
    {
        OPENALCALL(alSourcef(mBuffer->m_Id, AL_GAIN, v));
    }

    bool AudioSourceComponent::isPlaying() const
    {
        ALint source_state;
	    OPENALCALL(alGetSourcei(mBuffer->m_Id, AL_SOURCE_STATE, &source_state));
	    return source_state == AL_PLAYING;
    }

    void AudioSourceComponent::setBuffer(std::shared_ptr<AudioBuffer> buffer)

    {
        OPENALCALL(alSourcei(mBuffer->m_Id, AL_BUFFER, buffer->m_Id));
        mBuffer = std::move(buffer);
    }

    std::shared_ptr<AudioBuffer> AudioSourceComponent::getBuffer()
    {
        return mBuffer;
    }
}

#ifdef OPENALCALL
#undef OPENALCALL
#endif
