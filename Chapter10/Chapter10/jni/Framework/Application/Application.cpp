#include "Application.h"
#include "../Utilities/File.h"
#include "../EventManager/EventManager.h"
#include "../Utilities/Timer.h"
#include "../Renderer/Renderer.h"
#include "../Collision/CollisionManager.h"
#include "../Audio/AudioManager.h"

namespace Framework
{
	Application::Application(android_app* state)
		:	m_pAppState(state)
		,	m_androidTask(state, Task::PLATFORM_PRIORITY)
	{
		File::SetAssetManager(state->activity->assetManager);
	}

	Application::~Application()
	{

	}

	bool Application::Initialize()
	{
		bool ret = true;

		CreateSingletons();

		m_kernel.AddTask(&m_androidTask);
		m_kernel.AddTask(Timer::GetSingletonPtr());
		assert(Renderer::GetSingletonPtr());
		m_kernel.AddTask(Renderer::GetSingletonPtr());

		return ret;
	}

	void Application::Run()
	{
		m_kernel.Execute();

		DestroySingletons();
	}

	void Application::CreateSingletons()
	{
		new Timer(Task::TIMER_PRIORITY);
		new Renderer(m_pAppState, Task::RENDER_PRIORITY);
		new EventManager();
		new CollisionManager();
		new AudioManager(m_pAppState->activity->assetManager);

		RegisterEvent(PAUSEAPP_EVENT);
		RegisterEvent(RESUMEAPP_EVENT);
	}

	void Application::DestroySingletons()
	{
		assert(CollisionManager::GetSingletonPtr());
		delete CollisionManager::GetSingletonPtr();

		assert(EventManager::GetSingletonPtr());
		delete EventManager::GetSingletonPtr();

		assert(Renderer::GetSingletonPtr());
		delete Renderer::GetSingletonPtr();

		assert(Timer::GetSingletonPtr());
		delete Timer::GetSingletonPtr();
	}
}
