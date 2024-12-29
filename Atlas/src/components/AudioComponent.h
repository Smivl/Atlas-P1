#pragma once

#include "Component.h"
#include "../AudioEngine.h"
#include "../StateManager.h"
#include "../utilities/Log.h"

#include <string>
#include <memory>

#include <FMOD/fmod.hpp>

class AudioComponent : public Component
{
public:
	AudioComponent(const std::string& tag, std::shared_ptr<AudioEngine> audioEngine);
	AudioComponent(const std::string& tag, std::shared_ptr<AudioEngine> audioEngine, const std::string& SoundfilePath);
	AudioComponent(const std::string& tag, std::shared_ptr<AudioEngine> audioEngine, const std::string& SoundfilePath, const std::string& ChannelGroupName);

	virtual void OnDetach() override;

	void PlayAudio();

	void MuteChannel();
	void UnMuteChannel();

	void PauseChannel();
	void UnPauseChannel();

	void SetVolume(const float& Volume);

	void SetChannelGroup(const std::string& Groupname);
	void SetSound(FMOD::Sound* NewSound);
	void SetSound(const std::string& NewSoundfilePath);

	inline const float GetVolume() { return m_LocalVolume; }
	inline const bool IsPlaying() { return m_isPlaying; }

private:
	float m_LocalVolume{ 1.0f };
	bool m_isPlaying = false;

private:
	std::string m_ChannelGroupName;

	std::weak_ptr<AudioEngine> m_AudioEngine;

	FMOD::Sound* m_Sound = nullptr;
	FMOD::Channel* m_AudioChannel = nullptr;

};