#include "GameBase.hpp"
#include "Defines.hpp"
#include "ServiceProvider.hpp"
#include "debug/DebugLog.hpp"
#include "input/Input.hpp"
#include "video/Video.hpp"
#include "helpers/Timer.hpp"


GameBase::GameBase(unsigned int sizeX, unsigned int sizeY) : m_eventListener(this, &GameBase::onEvent)
{
	IDebug* debugService = new Debug();
	ServiceProvider::provide(debugService);

	IVideo* videoService = new Video(sizeX, sizeY);
	ServiceProvider::provide(videoService);

	IInput* inputService = new Input();
	inputService->m_eventHandler.add(&m_eventListener);

	//input_service->event_handler_.add(EventListenerBase3<ReturnT, ParamT1, ParamT2, ParamT3> *slot)
	//input_service->event_handler_.attach(ListenerT *object, ReturnT (ListenerT::*member)(ParamT1, ParamT2, ParamT3));
	//input_service->event_handler_.detach(int id);

	//event_listener_.add(Event3<void, int, int, int> *cpp_event);
	//event_listener_.remove();

	ServiceProvider::provide(inputService);

	m_runningTimer = new Timer();
	m_fpsTimer = new Timer();
	m_updateTimer = new Timer();
}


GameBase::~GameBase()
{
	delete m_runningTimer;
	delete m_fpsTimer;
	delete m_updateTimer;
}


void GameBase::start()
{
	m_run = true;

	loop();
}


void GameBase::stop()
{
	m_run = false;
}


unsigned int GameBase::getTimeRunning()
{
	return m_runningTimer->getTicks();
}


unsigned int GameBase::getTickCount()
{
	return m_tickCount;
}


unsigned int GameBase::getFps()
{
	return m_fps;
}


unsigned int GameBase::getMsLastFrame()
{
	return m_msLastFrame;
}


void GameBase::delayMs(const unsigned int delayMs)
{
	sf::sleep(sf::milliseconds(delayMs));
}


void GameBase::loop()
{
	systemInit();

	IInput* input = ServiceProvider::getInput();

	while (m_run)
	{
		m_msLastFrame = m_fpsTimer->getTicks();

		if (m_msLastFrame > 1000.0f / FPS_LIMIT)
		{
			m_msLastFrame = (int)(1000.0f / FPS_LIMIT);
		}

		m_fpsTimer->start();

		input->onTick();
		onTick();
		++m_tickCount;

		onRender();

		if (m_updateTimer->getTicks() >= 1000)
		{
			m_fps = m_fpsCounter;
			m_fpsCounter = 0;
			m_updateTimer->start();
		}

		++m_fpsCounter;

		if (m_fpsTimer->getTicks() < 17)  //59fps
		{
			delayMs(17 - m_fpsTimer->getTicks());
		}
	}

	systemEnd();
}


void GameBase::systemInit()
{
	m_fpsCounter = 0;
	m_fps = FPS_LIMIT;
	m_msLastFrame = 0;

	init();

	m_runningTimer->start();
	m_updateTimer->start();
}


void GameBase::systemEnd()
{
	end();

	m_updateTimer->stop();
	m_fpsTimer->stop();
	m_runningTimer->stop();
}