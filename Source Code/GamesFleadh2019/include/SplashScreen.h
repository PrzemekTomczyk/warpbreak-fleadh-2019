#pragma once

#include <SFML/Graphics.hpp>
#include "XBOX360CONTROLLER.h"
#include "States.h"

class SplashScreen
{
public:
	/// <summary>
	/// default constructor
	/// </summary>
	/// <param name="state">reference to the screen states</param>
	SplashScreen(Menu & state);
	~SplashScreen();

	/// <summary>
	/// processes game events
	/// </summary>
	/// <param name="pad"> reference to the game controller</param>
	void processEvents(Xbox360Controller & t_pad);
	/// <summary>
	/// updates all objects in this class
	/// </summary>
	/// <param name="t_deltaTime">time since last update</param>
	void update(sf::Time t_deltaTime);
	/// <summary>
	/// function that renders all drawable objects
	/// </summary>
	/// <param name="t_window">reference to the main window</param>
	void render(sf::RenderWindow & t_window);
	void transitionIn();
	void transitionOut(Menu t_string);
	bool getChangeMenu();
	void resetChangeMenu();

private:
	sf::Texture m_texture;
	sf::Sprite m_splash;

	Menu & m_state; //reference to the state holding the current menu
	Menu m_nextState;
	
	bool m_transitionIn;
	bool m_transitionOut;
	sf::Time m_transitionTime;
	sf::Time m_transitionTotalTime;

	sf::RenderStates m_renderState;

	bool m_changeMenu;
};

