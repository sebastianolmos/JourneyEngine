#include "audioManager.hpp"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#include "AL/al.h"
#include "AL/alc.h"


#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>
#include <glm/gtc/type_ptr.hpp>

#include "../entity/entity.hpp"
#include "../component/audioSource.hpp"
#include "audioBuffer.hpp"
#include "../scene/scene.hpp"


#ifndef OPENALCALL
#define OPENALCALL(function)\
	function;\
	{\
		ALenum error = alGetError();\
		assert(error != AL_NO_ERROR, "OpenAL Error");\
	}
#endif

namespace Journey {

void AudioManager::StartUp() 
{
    //Create the context
    /* Conecting to the default device */
	const char* devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	ALCdevice* device = OPENALCALL(alcOpenDevice(devicename));
	if (device == nullptr) return;

	/* Checking if our OpenAL support enumeration of devices */
	ALboolean enumeration = OPENALCALL(alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"));
	if (enumeration == AL_FALSE)
		std::cout << "enumeration not supported" << std::endl;
	else
		std::cout << "enumeration supported" << std::endl;

	// list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	ALCcontext* context = OPENALCALL(alcCreateContext(device, NULL));
	bool success = OPENALCALL(alcMakeContextCurrent(context));
	if (!success) return;

	std::cout << "OpenAL context OK" << std::endl;
}

void AudioManager::ShutDown() 
{
    cleanBatch();
}

void AudioManager::LoadAudioBuffer(std::string audioName, std::string audioPath) 
{
    std::size_t hashedName = mHasher(audioName);
    if (mAudioBufferRecord.count(hashedName) != 0)
        return;

    AudioData audioData;
	drwav_int16* sampleData = drwav_open_file_and_read_pcm_frames_s16(
		audioPath.c_str(),
		&audioData.channels,
		&audioData.sampleRate,
		&audioData.totalPCMFrameCount,
		nullptr);
    if (!sampleData)
	{
		std::cerr << "Audio Clip Error: Failed to load file " << audioPath.c_str();
		drwav_free(sampleData, nullptr);
		return;
	}
	else if (audioData.GetTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
	{
		std::cerr << "Audio Clip Error: File " << audioPath.c_str() << " is to big to be loaded.";
		drwav_free(sampleData, nullptr);
		return;
	}
    /*at this point, the wav file is correctly loaded */

	//Si la carga usando dr_wav fue exitosa se comienza el transpaso de estos datos a OpenAL.
	audioData.pcmData.resize(size_t(audioData.GetTotalSamples()));

	//Primero se copian todos los datos a un vector de uint16_t, para luego liberar los datos recien copiados.
	std::memcpy(audioData.pcmData.data(), sampleData, audioData.pcmData.size() * 2);
	drwav_free(sampleData, nullptr);
    std::shared_ptr<AudioData> fData = std::make_shared<AudioData>(audioData);
    mAudioBufferRecord.insert({hashedName, fData});
}

void AudioManager::AddAudioSourceComponent(std::shared_ptr<Entity> entity, std::string audioName, bool global, bool loop)
{
    std::size_t hashedName = mHasher(audioName);
    if (entity == nullptr || entity->mComponents.count(EComponentType::AudioSourceComponent) != 0)
        return;
    if (mAudioBufferRecord.count(hashedName) == 0) {
        std::cout << audioName << " No registered"<< std::endl;
        return;
    }

    /* Setting up a source */
	ALuint source;
	OPENALCALL(alGenSources((ALuint)1, &source));
	OPENALCALL(alSourcef(source, AL_PITCH, 1));
	OPENALCALL(alSourcef(source, AL_GAIN, 1));
    OPENALCALL(alSource3f(source, AL_POSITION, 0, 0, 0));
	OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
	OPENALCALL(alSourcei(source, AL_LOOPING, loop));
    if (global) {
        OPENALCALL(alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE));
    }

    std::shared_ptr<AudioData> fData = mAudioBufferRecord[hashedName];
    /* Generating a buffer */
	ALuint buffer;
	OPENALCALL(alGenBuffers((ALuint)1, &buffer));
    OPENALCALL(alBufferData(
		buffer,
		fData->channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
		fData->pcmData.data(),
		fData->pcmData.size() * 2,
		fData->sampleRate)
	);

    /* Binding the buffer with the data to source */
	OPENALCALL(alSourcei(source, AL_BUFFER, buffer));

    std::shared_ptr<AudioSourceComponent> audioSrc = std::make_shared<AudioSourceComponent>();
    audioSrc->mLoop = loop;
    audioSrc->mGlobal = global;

    std::shared_ptr<AudioBuffer> aBuffer = std::make_shared<AudioBuffer>(source, buffer);
    audioSrc->mBuffer = aBuffer;
    entity->mComponents.insert({EComponentType::AudioSourceComponent, audioSrc});
}

void AudioManager::AddListener(glm::vec3 position, float volume)
{
    /* Setting up the listener */
	ALfloat listenerOrigin[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    OPENALCALL(alListenerfv(AL_POSITION, glm::value_ptr(position)));
	OPENALCALL(alListener3f(AL_VELOCITY, 0, 0, 0));
	OPENALCALL(alListenerfv(AL_ORIENTATION, listenerOrigin));
    OPENALCALL(alListenerf(AL_GAIN, volume));
}

void AudioManager::setListenerVolume(float volume)
{
    OPENALCALL(alListenerf(AL_GAIN, volume));
}

float AudioManager::getListenerVolume()
{
    float volume;
    OPENALCALL(alGetListenerf(AL_GAIN, &volume));
    return volume;
}

void AudioManager::pushSourceToBatch(glm::vec3 pos, std::shared_ptr<AudioBuffer> buffer)
{
    mAudioSourcesBatch.push_back(std::make_pair(pos, buffer));
}

void AudioManager::cleanBatch()
{
    mAudioSourcesBatch.clear();
}

void AudioManager::updateListener(Scene& scene, float deltaTime)
{
    glm::vec3 lastPos;
    OPENALCALL(alGetListenerfv(AL_POSITION, glm::value_ptr(lastPos)));
    OPENALCALL(alListenerfv(AL_POSITION, glm::value_ptr(scene.GetCameraHandler().getViewPos())));

    glm::vec3 velocity = (scene.GetCameraHandler().getViewPos() - lastPos) / deltaTime;
    OPENALCALL(alListenerfv(AL_VELOCITY, glm::value_ptr(velocity)));

    glm::vec3 orientation[] {scene.GetCameraHandler().getFront(), scene.GetCameraHandler().getUp()};
    OPENALCALL(alListenerfv(AL_ORIENTATION, glm::value_ptr(orientation[0])));
}

void AudioManager::updateSources(float deltaTime)
{
    for(auto& aInfo: mAudioSourcesBatch) {
        glm::vec3 lastPos;
        OPENALCALL(alGetSourcefv(aInfo.second->m_Id, AL_POSITION, glm::value_ptr(lastPos)));

        OPENALCALL(alSourcefv(aInfo.second->m_Id, AL_POSITION, glm::value_ptr(aInfo.first)));

        glm::vec3 velocity = (aInfo.first - lastPos) / float(deltaTime);
        OPENALCALL(alSourcefv(aInfo.second->m_Id, AL_VELOCITY, glm::value_ptr(velocity)));
    }   
}

void AudioManager::list_audio_devices(const char* devices)
{
	const ALCchar* device = devices, * next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}

bool AudioManager::load_wav_file(const char* audiofile, char bufferId)
{
	/* loading the wav file to OpenAL */
	struct WavData {
		unsigned int channels = 0;
		unsigned int sampleRate = 0;
		drwav_uint64 totalPCMFrameCount = 0;
		std::vector<uint16_t> pcmData;
		drwav_uint64 GetTotalSamples() const { return totalPCMFrameCount * channels; }
	};

	/*
	* Primero se cargan los datos del archivo ubicado en audioFilePath
	* usando la libreria drwav (https://github.com/mackron/dr_libs)
	*/

	WavData audioData;
	drwav_int16* sampleData = drwav_open_file_and_read_pcm_frames_s16(
		audiofile,
		&audioData.channels,
		&audioData.sampleRate,
		&audioData.totalPCMFrameCount,
		nullptr);

	if (!sampleData)
	{
		std::cerr << "Audio Clip Error: Failed to load file " << audiofile;
		drwav_free(sampleData, nullptr);
		return false;
	}
	else if (audioData.GetTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
	{
		std::cerr << "Audio Clip Error: File " << audiofile << " is to big to be loaded.";
		drwav_free(sampleData, nullptr);
		return false;
	}

	/*at this point, the wav file is correctly loaded */

	//Si la carga usando dr_wav fue exitosa se comienza el transpaso de estos datos a OpenAL.
	audioData.pcmData.resize(size_t(audioData.GetTotalSamples()));

	//Primero se copian todos los datos a un vector de uint16_t, para luego liberar los datos recien copiados.
	std::memcpy(audioData.pcmData.data(), sampleData, audioData.pcmData.size() * 2);
	drwav_free(sampleData, nullptr);

	assert(bufferId != 0);

	OPENALCALL(alBufferData(
		bufferId,
		audioData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
		audioData.pcmData.data(),
		audioData.pcmData.size() * 2,
		audioData.sampleRate)
	);

	/* end loading wav to OpenAL */
}


        
void AudioManager::test() 
{
    /* Conecting to the default device */
	const char* devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	ALCdevice* device = OPENALCALL(alcOpenDevice(devicename));
	if (device == nullptr) return;

	/* Checking if our OpenAL support enumeration of devices */
	ALboolean enumeration = OPENALCALL(alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"));
	if (enumeration == AL_FALSE)
		std::cout << "enumeration not supported" << std::endl;
	else
		std::cout << "enumeration supported" << std::endl;

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	ALCcontext* context = OPENALCALL(alcCreateContext(device, NULL));
	bool success = OPENALCALL(alcMakeContextCurrent(context));
	if (!success) return;

	std::cout << "OpenAL context OK" << std::endl;

	/* Setting up the listener */
	ALfloat listenerOrigin[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	OPENALCALL(alListener3f(AL_POSITION, 0, 0, 1.0f));
	OPENALCALL(alListener3f(AL_VELOCITY, 0, 0, 0));
	OPENALCALL(alListenerfv(AL_ORIENTATION, listenerOrigin));

	/* Setting up a source */
	ALuint source;
	OPENALCALL(alGenSources((ALuint)1, &source));
	OPENALCALL(alSourcef(source, AL_PITCH, 1));
	OPENALCALL(alSourcef(source, AL_GAIN, 1));
	OPENALCALL(alSource3f(source, AL_POSITION, 0, 0, 0));
	OPENALCALL(alSource3f(source, AL_VELOCITY, 0, 0, 0));
	OPENALCALL(alSourcei(source, AL_LOOPING, AL_FALSE));

	/* Generating a buffer */
	ALuint buffer;
	OPENALCALL(alGenBuffers((ALuint)1, &buffer));

	success = load_wav_file("../../../assets/audio/loop.wav", buffer);
	if (!success) return;

	std::cout << "wav file loaded correctly" << std::endl;

	/* Binding the buffer with the data to source */
	OPENALCALL(alSourcei(source, AL_BUFFER, buffer));

	auto t0 = std::chrono::steady_clock::now();

	/* Playing the source */
	OPENALCALL(alSourcePlay(source));

	/* Wait while playing the song */
	ALint source_state;
	OPENALCALL(alGetSourcei(source, AL_SOURCE_STATE, &source_state));
	while (source_state == AL_PLAYING) {
		OPENALCALL(alGetSourcei(source, AL_SOURCE_STATE, &source_state));
	}

	auto dt = std::chrono::steady_clock::now() - t0;

	// cleanup context
	OPENALCALL(alDeleteSources(1, &source));
	OPENALCALL(alDeleteBuffers(1, &buffer));
	device = OPENALCALL(alcGetContextsDevice(context));
	OPENALCALL(alcMakeContextCurrent(NULL));
	OPENALCALL(alcDestroyContext(context));
	OPENALCALL(alcCloseDevice(device));

	const auto duration = std::chrono::duration_cast<std::chrono::seconds>(dt).count();

	std::cout << "The wav file lasted " << duration << " seconds." << std::endl;
    }
}

#ifdef OPENALCALL
#undef OPENALCALL
#endif
