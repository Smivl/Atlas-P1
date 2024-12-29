#include "Application.h"

void Application::Init()
{
	Log::Init();
	StateManager::LoadState();

	m_EventDispatcher = std::make_shared<EventDispatcher>();
	m_ComponentStack = std::make_shared<ComponentStack>();
	m_AudioEngine = std::make_shared<AudioEngine>();
	m_NLPEngine = std::make_shared<NLPEngine>();

	// Init system OUT audio to be able to communicate with user
	m_AudioEngine->CreateChannelGroup("SFX");
	m_AudioEngine->CreateChannelGroup("Voice");
	m_AudioEngine->CreateChannelGroup("Music");
	ATLAS_INFO("Created SFX Audio Channel Group");
	ATLAS_INFO("Created Voice Audio Channel Group");
	ATLAS_INFO("Created Music Audio Channel Group");

	if (m_IsMutedGlobal) m_AudioEngine->MuteAll();


	// PRE INITIALIZATION
	auto SystemMain_SFX = m_ComponentStack->AttachComponent<AudioComponent>("SystemSFX", m_AudioEngine, "src/assets/System/Sounds/System_InitSFX.wav", "SFX");
	auto SystemMain_Audio = m_ComponentStack->AttachComponent<AudioComponent>("SystemVoice", m_AudioEngine, "src/assets/System/Sounds/System_Init.wav", "Voice");
	SystemMain_SFX.lock().get()->PlayAudio();
	Sleep(1000);
	SystemMain_Audio.lock().get()->PlayAudio();
	

	//// INIT ALL MAIN SYSTEMS
	ATLAS_INFO("Initializing main systems...");
	ATLAS_INFO("Initializing NLP Engine...");
	// m_NLPEngine->Init();
	ATLAS_INFO("NLP Engine Online");
	ATLAS_INFO("Displaying screen...");
	m_ComponentStack->AttachWindowComponent("MainWindow", 1280, 720, "ATLAS");

	m_EventDispatcher->subscribe<AudioEngine, MuteEvent>(m_AudioEngine.get(), &AudioEngine::OnMuteEvent);
	m_EventDispatcher->subscribe<AudioEngine, PauseEvent>(m_AudioEngine.get(), &AudioEngine::OnPauseEvent);


	// POST INITIALIZATION
	ATLAS_INFO("Initialization Complete! All Systems Online");
	auto SoundPath = m_AudioEngine->CreateTTSFile("System initialization complete...");
	SystemMain_Audio.lock().get()->SetSound(SoundPath);
	SystemMain_Audio.lock().get()->PlayAudio();

	auto SystemBack_Audio = m_ComponentStack->AttachComponent<AudioComponent>("Music", m_AudioEngine, "src/assets/Sounds/song.wav", "Music");
	SystemBack_Audio.lock().get()->PlayAudio();
	SystemBack_Audio.lock().get()->SetVolume(0.1f);

}

void Application::DeInit()
{
	StateManager::SaveState();

	m_ComponentStack = nullptr;
	m_AudioEngine = nullptr;
}

void Application::Run()
{
	StateManager::SaveState();

	while (m_Running)
	{
		m_ComponentStack->OnUpdate();
		m_AudioEngine->OnUpdate();
		//std::cout << "\rFPS: " << fps << std::flush;
	}
}

