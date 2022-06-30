#pragma once
#include <memory>

#include "component.hpp"


namespace Journey {
    class AudioBuffer;
    class AudioManager;

    class AudioSourceComponent : public Component {
    public:
        AudioSourceComponent() = default;

        virtual EComponentType GetType() override { return EComponentType::AudioSourceComponent; }

        void play();
        void pause();
        void stop();
        bool getLooping();
        void setLooping(bool b);
        void setVolume(float v);
        bool isPlaying() const;
        void setBuffer(std::shared_ptr<AudioBuffer> buffer);
        std::shared_ptr<AudioBuffer> getBuffer();
    private:
        friend AudioManager;
        std::shared_ptr<AudioBuffer> mBuffer;
        bool mGlobal;
        bool mLoop;
    };

}