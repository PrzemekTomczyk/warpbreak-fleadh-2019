#pragma once

#include "Frame.h"
#include <SFML/Graphics.hpp>
#include "VLayout.h"
#include "TextButton.h"
#include "button.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "Slider.h"
#include "XBOX360CONTROLLER.h"
#include "States.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Utility.h"
#include "Exit.h"

class EndScreen
{
public:
	/// <summary>
	/// default  contructor
	/// </summary>
	/// <param name="state">game state</param>
	/// <param name="t_options">game options</param>
	/// <param name="t_resourceMng">game resource manager</param>
	/// <param name="t_input">game input handler</param>
	/// <param name="t_controller">controller</param>
	EndScreen(Menu & state, Options & t_options, ResourceManager & t_resourceMng, Input & t_input, Xbox360Controller & t_controller);
	~EndScreen();

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

	bool getChangeMenu();

	/// <summary>
	/// resets the m_changeMenu variable
	/// </summary>
	void resetChangeMenu();

	void initEnd(EndState t_state);

	void transitionIn(); //Sets up for transitioning out of the menu
	void transitionOut(Menu s); //Sets up for transitioning into the menu


private:
	ResourceManager & m_resourceMng;
	Options & m_options;
	Menu & m_state; //reference to the state holding the current menu
	Menu m_nextState;

	sf::RenderStates m_renderState;

	bool m_transitionIn;
	bool m_transitionOut;

	sf::Time m_transitionTime;
	sf::Time m_transitionTotalTime;

	bool m_changeMenu = false;

	Input & m_input;

	sf::Sprite m_endSprite;

	EndState m_endState;

	Xbox360Controller &m_controller;
};

