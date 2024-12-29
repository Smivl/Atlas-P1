#include "AudioComponent.h"

AudioComponent::AudioComponent(const std::string& tag, std::shared_ptr<AudioEngine> audioEngine)
	: Component(tag), m_AudioEngine(audioEngine) 
{

}

AudioComponent::AudioComponent(const std::string& tag, std::shared_ptr<AudioEngine> audioEngine, const std::string& SoundfilePath)
	: Component(tag), m_AudioEngine(audioEngine)
{
	m_Sound = m_AudioEngine.lock().get()->CreateSound(SoundfilePath);
}

AudioComponent::AudioComponent(const std::string& tag, std::shared_ptr<AudioEngine> audioEngine, const std::string& SoundfilePath, const std::string& ChannelGroupName)
	: Component(tag), m_AudioEngine(audioEngine), m_ChannelGroupName(ChannelGroupName)
{
	m_Sound = m_AudioEngine.lock().get()->CreateSound(SoundfilePath);
	SetChannelGroup(m_ChannelGroupName);
}

void AudioComponent::OnDetach()
{
	StateManager::QueueObjectItem(Tag, "Volume", std::to_string(m_LocalVolume));

	if (m_Sound)
	{
		m_Sound->release();
	}
}

void AudioComponent::PlayAudio()
{
	if (!m_Sound)
	{
		ATLAS_WARN("No sound currently attached");
		return;
	}

	if (!m_ChannelGroupName.empty())
	{
		m_AudioEngine.lock().get()->GetAudioSystem()->playSound(m_Sound, m_AudioEngine.lock().get()->GetChannelGroup(m_ChannelGroupName), false, &m_AudioChannel);
	}
	else
	{
		m_AudioEngine.lock().get()->GetAudioSystem()->playSound(m_Sound, nullptr, false, &m_AudioChannel);
	}
}

void AudioComponent::MuteChannel()
{
	m_AudioChannel->setMute(true);
}

void AudioComponent::UnMuteChannel()
{
	m_AudioChannel->setMute(false);
}

void AudioComponent::PauseChannel()
{
	m_AudioChannel->setPaused(1);
}

void AudioComponent::UnPauseChannel()
{
	m_AudioChannel->setPaused(0);
}

void AudioComponent::SetVolume(const float& Volume)
{
	m_LocalVolume = Volume;
	m_AudioChannel->setVolume(Volume);
}

void AudioComponent::SetChannelGroup(const std::string& Groupname)
{

	m_ChannelGroupName = Groupname;
	auto* Group = m_AudioEngine.lock().get()->GetChannelGroup(Groupname);
	m_AudioChannel->setChannelGroup(Group);

}

void AudioComponent::SetSound(FMOD::Sound* NewSound)
{
	if (m_Sound)
	{
		m_Sound->release();
	}

	m_Sound = NewSound;

}

void AudioComponent::SetSound(const std::string& NewSoundfilePath)
{

	if (m_Sound)
	{
		m_Sound->release();
	}

	m_Sound = m_AudioEngine.lock().get()->CreateSound(NewSoundfilePath);

}