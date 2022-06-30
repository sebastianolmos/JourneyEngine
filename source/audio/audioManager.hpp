#pragma once 

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <utility>  

namespace Journey {
    class Entity;
    class Scene;
    class AudioBuffer;
    class AudioSourceComponent;

    struct AudioData {
        signed short* data;
        unsigned int channels;
        unsigned int sample_rate;
        unsigned __int64 frame_count;
    };

    class AudioManager {
    public:
        friend class Scene;
        AudioManager() {};

        void StartUp();
        void ShutDown();

        // Test functions
        void test();
        void list_audio_devices(const char* devices);
        bool load_wav_file(const char* audiofile, char bufferId);

        void LoadAudioBuffer(std::string audioName, std::string audioPath);
        void AddAudioSourceComponent(std::shared_ptr<Entity> entity, std::string audioName, bool global, bool loop);

        // Listener methods

        void AddListener(glm::vec3 position, float volume);
        void setListenerVolume(float volume);
        float getListenerVolume();

        
        void pushSourceToBatch(glm::vec3 pos, std::shared_ptr<AudioBuffer> buffer);
    private:
        void cleanBatch();
        void updateListener(Scene& scene, float deltaTime);
        void updateSources(float deltaTime);

        std::hash<std::string> mHasher;
        std::unordered_map<std::size_t, std::shared_ptr<AudioData> > mAudioBufferRecord;

        std::vector< std::pair<glm::vec3, std::shared_ptr<AudioBuffer>> > mAudioSourcesBatch;
    };

}