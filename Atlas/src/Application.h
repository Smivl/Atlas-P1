#pragma once

#include "utilities/Log.h"

#include "events/EventDispatcher.h"
#include "components/ComponentStack.h"
#include "components/AudioComponent.h"
#include "components/WindowComponent.h"
#include "AudioEngine.h"
#include "NLPEngine.h"
#include "StateManager.h"

#include <memory>
#include <chrono>
#include <thread>
#include <string>


class Application
{
public:
	Application() = default;
	~Application() = default;

	void Init();
	void DeInit();
	void Run();

private:
	bool m_IsMutedGlobal{ false };

private:
	std::shared_ptr<EventDispatcher> m_EventDispatcher;
	std::shared_ptr<ComponentStack> m_ComponentStack;
	std::shared_ptr<AudioEngine> m_AudioEngine;
	std::shared_ptr<NLPEngine> m_NLPEngine;

	std::chrono::steady_clock::time_point m_LastFrameTime;

	bool m_Running{ true };
};

