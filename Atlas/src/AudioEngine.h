#pragma once

#include "events/Event.h"
#include "utilities/Log.h"

#pragma warning(disable:4996) 
#include <sphelper.h>
#pragma warning(default: 4996)
#include <sapi.h>
#include <atlbase.h>
#include <unordered_map>
#include <iostream>
#include <string>

#include <FMOD/fmod.hpp>

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	void OnUpdate();

	void OnMuteEvent(MuteEvent* data);
	void OnPauseEvent(PauseEvent* data);

	// Mute functions
	void MuteChannelGroup(FMOD::ChannelGroup*& ChannelGroup);
	void MuteChannelGroup(const std::string& ChannelGroupName);
	void UnMuteChannelGroup(FMOD::ChannelGroup*& ChannelGroup);
	void UnMuteChannelGroup(const std::string& ChannelGroupName);
	void MuteAll();
	void UnMuteAll();

	// Pause functions
	void PauseChannelGroup(FMOD::ChannelGroup*& ChannelGroup);
	void PauseChannelGroup(const std::string& ChannelGroupName);
	void UnPauseChannelGroup(FMOD::ChannelGroup*& ChannelGroup);
	void UnPauseChannelGroup(const std::string& ChannelGroupName);
	void PauseAll();
	void UnPauseAll();

	std::string CreateTTSFile(const std::string& data);
	FMOD::Sound* CreateSound(const std::string& path);
	FMOD::ChannelGroup* CreateChannelGroup(const std::string& ChannelGroupName);

	FMOD::ChannelGroup* GetChannelGroup(const std::string& ChannelGroupName);

	inline FMOD::System* GetAudioSystem() { return m_System; }

private:
	float m_GlobalVolume{ 1.0f };

private:
	FMOD::System* m_System;
	std::unordered_map<std::string ,FMOD::ChannelGroup*> m_AudioChannelGroups;
};

