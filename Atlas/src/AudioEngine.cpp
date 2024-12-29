#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
	FMOD::System_Create(&m_System);
	m_System->init(512, FMOD_INIT_NORMAL, nullptr);
}

AudioEngine::~AudioEngine()
{
	std::unordered_map<std::string, FMOD::ChannelGroup*>::iterator it = m_AudioChannelGroups.begin();

	while (it != m_AudioChannelGroups.end()) 
	{

		it->second->release();
		it++;

	}

	m_System->release();
}

// EVENT FUNCTIONS

void AudioEngine::OnUpdate()
{
	m_System->update();
}

void AudioEngine::OnMuteEvent(MuteEvent* data)
{
	if (data->All)
		if (data->Mute)
			MuteAll();
		else
			UnMuteAll();
	else
		if (data->Mute)
			MuteChannelGroup(data->Channel);
		else
			UnMuteChannelGroup(data->Channel);
}
void AudioEngine::OnPauseEvent(PauseEvent* data)
{
	if (data->All)
		if (data->Pause)
			PauseAll();
		else
			UnPauseAll();
	else
		if (data->Pause)
			PauseChannelGroup(data->Channel);
		else
			UnPauseChannelGroup(data->Channel);
}

// MUTE AUDIO FUNCTIONS
void AudioEngine::MuteChannelGroup(FMOD::ChannelGroup*& ChannelGroup)
{
	if (ChannelGroup)
	{
		ChannelGroup->setMute(true);
	}
	else
	{
		ATLAS_WARN("MuteChannelGroup Function FAILED! Channel group pointer invalid");
	}
}
void AudioEngine::MuteChannelGroup(const std::string& ChannelGroupName)
{
	FMOD::ChannelGroup* ChannelGroup = GetChannelGroup(ChannelGroupName);

	if (ChannelGroup)
	{
		ChannelGroup->setMute(true);
	}
	else
	{
		ATLAS_WARN("MuteChannelGroup Function FAILED! Channel group " + ChannelGroupName + " not found!");
	}
}
void AudioEngine::UnMuteChannelGroup(FMOD::ChannelGroup*& ChannelGroup)
{
	if (ChannelGroup)
	{
		ChannelGroup->setMute(false);
	}
	else
	{
		ATLAS_WARN("UnMuteChannelGroup Function FAILED! Channel group pointer invalid");
	}
}
void AudioEngine::UnMuteChannelGroup(const std::string& ChannelGroupName)
{
	FMOD::ChannelGroup* ChannelGroup = GetChannelGroup(ChannelGroupName);

	if (ChannelGroup)
	{
		ChannelGroup->setMute(false);
	}
	else
	{
		ATLAS_WARN("UnMuteChannelGroup Function FAILED! Channel group " + ChannelGroupName + " not found!");
	}
}
void AudioEngine::MuteAll()
{
	for (auto& it : m_AudioChannelGroups)
	{
		it.second->setMute(true);
	}
}
void AudioEngine::UnMuteAll()
{
	for (auto& it : m_AudioChannelGroups)
	{
		it.second->setMute(false);
	}
}

// PAUSE AUDIO FUNCTIONS
void AudioEngine::PauseChannelGroup(FMOD::ChannelGroup*& ChannelGroup)
{
	if (ChannelGroup)
	{
		ChannelGroup->setPaused(1);
	}
	else
	{
		ATLAS_WARN("PauseChannelGroup Function FAILED! Channel group pointer invalid");
	}
}
void AudioEngine::PauseChannelGroup(const std::string& ChannelGroupName)
{
	FMOD::ChannelGroup* ChannelGroup = GetChannelGroup(ChannelGroupName);

	if (ChannelGroup)
	{
		ChannelGroup->setPaused(1);
	}
	else
	{
		ATLAS_WARN("PauseChannelGroup Function FAILED! Channel group " + ChannelGroupName + " not found!");
	}
}
void AudioEngine::UnPauseChannelGroup(FMOD::ChannelGroup*& ChannelGroup)
{
	if (ChannelGroup)
	{
		ChannelGroup->setPaused(0);
	}
	else
	{
		ATLAS_WARN("UnPauseChannelGroup Function FAILED! Channel group pointer invalid");
	}
}
void AudioEngine::UnPauseChannelGroup(const std::string& ChannelGroupName)
{
	FMOD::ChannelGroup* ChannelGroup = GetChannelGroup(ChannelGroupName);

	if (ChannelGroup)
	{
		ChannelGroup->setPaused(0);
	}
	else
	{
		ATLAS_WARN("UnPauseChannelGroup Function FAILED! Channel group " + ChannelGroupName + " not found!");
	}
}
void AudioEngine::PauseAll()
{
	for (auto& it : m_AudioChannelGroups)
	{
		it.second->setPaused(1);
	}
}
void AudioEngine::UnPauseAll()
{
	for (auto& it : m_AudioChannelGroups)
	{
		it.second->setPaused(0);
	}
}


std::string AudioEngine::CreateTTSFile(const std::string& data)
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		ATLAS_ERROR("Failed Init");
		return "";
	}
	
	ISpVoice* pVoice = NULL;
	
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice); // Create the voice object
	if (FAILED(hr))
	{
		ATLAS_ERROR("Failed creating instance");
		return "";
	}

	static std::uint64_t Counter = 0;

	// Set output format to WAV
	std::string filepath = "src/assets/Temp/temp_" + std::to_string(Counter) + ".wav";
	ISpStream* pStream = NULL;
	CSpStreamFormat	cAudioFmt;
	cAudioFmt.AssignFormat(SPSF_22kHz16BitMono);

	SPBindToFile(std::wstring(filepath.begin(), filepath.end()).c_str(), SPFM_CREATE_ALWAYS, &pStream, &(cAudioFmt.FormatId()), cAudioFmt.WaveFormatExPtr());

	// Set voice to output to WAV stream
	pVoice->SetOutput(pStream, TRUE);

	// Speak the input text
	pVoice->Speak(std::wstring(data.begin(), data.end()).c_str(), SPF_DEFAULT, NULL);

	Counter++;

	// Clean up
	pStream->Release();
	pVoice->Release();
	CoUninitialize(); // Uninitialize COM

	return filepath;
}

FMOD::Sound* AudioEngine::CreateSound(const std::string& path)
{

	FMOD::Sound* newSound;
	m_System->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &newSound);
	return newSound;

}

FMOD::ChannelGroup* AudioEngine::CreateChannelGroup(const std::string& ChannelGroupName)
{

	FMOD::ChannelGroup* NewGroup = nullptr;
	m_System->createChannelGroup(ChannelGroupName.c_str(), &NewGroup);
	m_AudioChannelGroups.emplace(ChannelGroupName, NewGroup);

	return NewGroup;
}

FMOD::ChannelGroup* AudioEngine::GetChannelGroup(const std::string& ChannelGroupName)
{

	auto it = m_AudioChannelGroups.find(ChannelGroupName);

	if (it == m_AudioChannelGroups.end())
	{
		ATLAS_WARN("Did not find " + ChannelGroupName + " channel group!");
		return nullptr;
	}

	return it->second;
}
