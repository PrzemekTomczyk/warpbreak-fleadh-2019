#include "Game.h"
#include <iostream>
#include "Utility.h"

Game::Game() :
	m_window{ sf::VideoMode{ 1920, 1080, 32 }, "Warp Break"},
	m_exitGame{ false },
	m_currentMenu(Menu::StateSplash),
	m_mainMenu(m_resourceMng.getFont(FontID::ARIBLK), m_window, m_currentMenu, m_options),
	m_optionMenu(m_resourceMng.getFont(FontID::ARIBLK), m_window, m_currentMenu, m_options),
	m_creditsScreen(m_resourceMng.getFont(FontID::ARIBLK), m_window, m_currentMenu, m_options),
	m_licenseScreen(m_currentMenu),
	m_splashScreen(m_currentMenu),
	m_gameScreen(m_window, m_currentMenu, m_options, m_resourceMng, m_input, m_controller),
	m_endScreen(m_currentMenu, m_options, m_resourceMng, m_input, m_controller)
{
	m_window.setVerticalSyncEnabled(true);
	m_window.setFramerateLimit(60);

	m_options.musicVol = 100;
	m_options.soundVol = 100;
	m_options.mute = false;
	m_options.animSpeed = 50;
	m_options.fontSize = 2;

	if (!m_background.openFromFile("ASSETS\\SOUNDS\\background.wav"))
	{
		std::cout << "cannot load musics" << std::endl;
	}
	m_background.setLoop(true);
	m_background.play();
	
#ifdef _DEBUG
	m_currentMenu = Menu::StateGameplay;
	m_background.setVolume(1);
#else
	m_window.create(sf::VideoMode{ 1920, 1080, 32 }, "Warp Break", sf::Style::Fullscreen);
#endif // _DEBUG

	if (m_currentMenu == Menu::StateSplash)
	{
		m_splashScreen.transitionIn();
	}
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	m_controller.update();
	m_input.update();
	processScreenEvents();
	while (m_window.pollEvent(event))
	{


		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_exitGame = true;
		}

		handleKeyPresses(event);
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (!m_options.mute)
	{
#ifdef _DEBUG
		m_background.setVolume(0);
#else
		m_background.setVolume(m_options.musicVol);
#endif // _DEBUG
	}
	else
	{
		m_background.setVolume(0);
	}
	
	switch (m_currentMenu)
	{
	case Menu::StateSplash:
		if (m_changingMenu)
		{
			m_splashScreen.transitionIn();
		}
		m_splashScreen.update(t_deltaTime);
		m_changingMenu = m_splashScreen.getChangeMenu();
		m_splashScreen.resetChangeMenu();
		break;
	case Menu::StateLicense:
		if (m_changingMenu)
		{
			m_licenseScreen.transitionIn();
		}
		m_licenseScreen.update(t_deltaTime);
		m_changingMenu = m_licenseScreen.getChangeMenu();
		m_licenseScreen.resetChangeMenu();
		break;
	case StateMainMenu:
		if (m_changingMenu)
		{
			m_mainMenu.transitionIn();
		}
		m_mainMenu.update(t_deltaTime);
		m_changingMenu = m_mainMenu.getChangeMenu();
		m_mainMenu.resetChangeMenu();
		break;
	case StateOptions:
		if (m_changingMenu)
		{
			m_optionMenu.transitionIn();
		}
		m_optionMenu.update(t_deltaTime);
		m_changingMenu = m_optionMenu.getChangeMenu();
		m_optionMenu.resetChangeMenu();
		break;
	case StateGameplay:
		if (m_changingMenu)
		{
			m_gameScreen.restart();
		}
		m_gameScreen.update(t_deltaTime, m_endScreen, m_mainMenu);
		m_changingMenu = m_gameScreen.getChangeMenu();
		m_gameScreen.resetChangeMenu();
		break;
	case StateEnd:
		if (m_changingMenu)
		{
			m_endScreen.transitionIn();
		}
		m_endScreen.update(t_deltaTime);
		m_changingMenu = m_endScreen.getChangeMenu();
		m_endScreen.resetChangeMenu();
		break;
	case StateCredits:
		if (m_changingMenu)
		{
			m_creditsScreen.transitionIn();
		}
		m_creditsScreen.update(t_deltaTime);
		m_changingMenu = m_creditsScreen.getChangeMenu();
		m_creditsScreen.resetChangeMenu();
		break;
	case StateQuit:
		m_exitGame = true;
		break;
	default:
		break;
	}


	if (m_exitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(sf::Color(5,10,40));

	switch (m_currentMenu)
	{
	case StateSplash:
		m_splashScreen.render(m_window);
		break;
	case StateLicense:
		m_licenseScreen.render(m_window);
		break;
	case StateMainMenu:
		m_mainMenu.render(m_window);
		break;
	case StateOptions:
		m_optionMenu.render(m_window);
		break;
	case StateGameplay:
		m_gameScreen.render(m_window);
		break;
	case StateEnd:
		m_endScreen.render(m_window);
		break;
	case StateCredits:
		m_creditsScreen.render(m_window);
		break;
	case StateQuit:
		break;
	default:
		break;
	}

	m_window.display();
}

void Game::processScreenEvents()
{
	switch (m_currentMenu)
	{
	case StateSplash:
		m_splashScreen.processEvents(m_controller);
		break;
	case StateLicense:
		m_licenseScreen.processEvents(m_controller);
		break;
	case StateMainMenu:
		m_mainMenu.processEvents(m_controller);
		break;
	case StateOptions:
		m_optionMenu.processEvents(m_controller);
		break;
	case StateCredits:
		m_creditsScreen.processEvents(m_controller);
		break;
	case StateGameplay:
		break;
	case StateEnd:
		break;
	case StateQuit:
		break;
	default:
		break;
	}
}

void Game::handleKeyPresses(sf::Event t_event)
{
	if (sf::Event::KeyPressed == t_event.type) //user key press
	{
		if (sf::Keyboard::A == t_event.key.code)
		{
			m_input.m_current.A = true;
		}
		if (sf::Keyboard::D == t_event.key.code)
		{
			m_input.m_current.D = true;
		}
		if (sf::Keyboard::W == t_event.key.code)
		{
			m_input.m_current.W = true;
		}
		if (sf::Keyboard::S == t_event.key.code)
		{
			m_input.m_current.S = true;
		}
		if (sf::Keyboard::F == t_event.key.code)
		{
			m_input.m_current.F = true;
		}
		if (sf::Keyboard::E == t_event.key.code)
		{
			m_input.m_current.E = true;
		}
		if (sf::Keyboard::Q == t_event.key.code)
		{
			m_input.m_current.Q = true;
		}
		if (sf::Keyboard::Space == t_event.key.code)
		{
			m_input.m_current.Space = true;
		}
		if (sf::Keyboard::Return == t_event.key.code)
		{
			m_input.m_current.Return = true;
		}
		if (sf::Keyboard::Num1 == t_event.key.code)
		{
			m_input.m_current.one = true;
		}
		if (sf::Keyboard::Num2 == t_event.key.code)
		{
			m_input.m_current.two = true;
		}
		if (sf::Keyboard::Num3 == t_event.key.code)
		{
			m_input.m_current.three = true;
		}
		if (sf::Keyboard::Num4 == t_event.key.code)
		{
			m_input.m_current.four = true;
		}
		if (sf::Keyboard::Num5 == t_event.key.code)
		{
			m_input.m_current.five = true;
		}
		if (sf::Keyboard::Num6 == t_event.key.code)
		{
			m_input.m_current.six = true;
		}
	}

	if (sf::Event::KeyReleased == t_event.type)
	{
		if (sf::Keyboard::A == t_event.key.code)
		{
			m_input.m_current.A = false;
		}
		if (sf::Keyboard::D == t_event.key.code)
		{
			m_input.m_current.D = false;
		}
		if (sf::Keyboard::W == t_event.key.code)
		{
			m_input.m_current.W = false;
		}
		if (sf::Keyboard::S == t_event.key.code)
		{
			m_input.m_current.S = false;
		}
		if (sf::Keyboard::F == t_event.key.code)
		{
			m_input.m_current.F = false;
		}
		if (sf::Keyboard::E == t_event.key.code)
		{
			m_input.m_current.E = false;
		}
		if (sf::Keyboard::Q == t_event.key.code)
		{
			m_input.m_current.Q = false;
		}
		if (sf::Keyboard::Space == t_event.key.code)
		{
			m_input.m_current.Space = false;
		}
		if (sf::Keyboard::Return == t_event.key.code)
		{
			m_input.m_current.Return = false;
		}
		if (sf::Keyboard::Num1 == t_event.key.code)
		{
			m_input.m_current.one = false;
		}
		if (sf::Keyboard::Num2 == t_event.key.code)
		{
			m_input.m_current.two = false;
		}
		if (sf::Keyboard::Num3 == t_event.key.code)
		{
			m_input.m_current.three = false;
		}
		if (sf::Keyboard::Num4 == t_event.key.code)
		{
			m_input.m_current.four = false;
		}
		if (sf::Keyboard::Num5 == t_event.key.code)
		{
			m_input.m_current.five = false;
		}
		if (sf::Keyboard::Num6 == t_event.key.code)
		{
			m_input.m_current.six = false;
		}
	}
}

