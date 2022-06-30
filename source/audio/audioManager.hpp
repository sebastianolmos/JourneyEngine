#pragma once 

#include "dr_wav.h"

#include "AL/al.h"
#include "AL/alc.h"

namespace Journey {
    class Scene;

    class AudioManager {
    public:
        friend class Scene;
        AudioManager() {};

        void StartUp() {};
        void ShutDown() {};

        void test();
        void list_audio_devices(const ALCchar* devices);
        bool load_wav_file(const char* audiofile, ALuint bufferId);

    private:
    };

}