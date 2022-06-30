#pragma once

#include <AL/al.h>
#include <stdexcept> 
#include <iostream>
#include <exception>  

#ifndef AL_CALL_TMPL
#define AL_CALL_TMPL
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

namespace Journey {
        
    template <auto alGen, auto alDelete>
    class ALObject
    {
    public:
        ALObject() {
            ALCall<alGen>(1, &m_id);
        }
        ~ALObject() {
            deleteObject();
        }
        ALObject(ALObject&& o) noexcept {
            deleteObject();
            m_id = o.m_id;
            o.m_id = 0;
        }
        ALObject& operator= (ALObject&& o) noexcept {
            deleteObject();
            m_id = o.m_id;
            o.m_id = 0;
            return *this;
        }
    protected:
        ALuint m_id {0};

    private:
        void deleteObject() noexcept
        try {
            ALCall<alDelete>(1, &m_id);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    };

    class AudioSourceComponent;
    class AudioManager;

    class AudioBuffer final : public ALObject<alGenBuffers, alDeleteBuffers> {
        friend class AudioSourceComponent;
        friend class AudioManager;
    public:
        enum class Format {
            Mono8 = AL_FORMAT_MONO8,
            Mono16 = AL_FORMAT_MONO16,
            Stereo8 = AL_FORMAT_STEREO8,
            Stereo16 = AL_FORMAT_STEREO16
        };

        void setData(Format fmt, int freq, int size, const void* data) const {
            ALCall<alBufferData>(m_id, static_cast<ALenum>(fmt), data, size, freq);
        }
    };

}

#ifdef CASE
#undef CASE
#endif
