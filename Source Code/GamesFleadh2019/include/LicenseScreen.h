#pragma once
#include <SFML/Graphics.hpp>
#include "XBOX360CONTROLLER.h"
#include "States.h"

class LicenseScreen
{
public:
	LicenseScreen(Menu & state);
	~LicenseScreen();
	void processEvents(Xbox360Controller & t_pad);
	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow & t_window);
	void transitionIn(); //Sets up for transitioning out of the menu
	void transitionOut(Menu t_string); //Sets up for transitioning into the menu
	bool getChangeMenu(); //Returns wheather the menu has finished transitioning out
	void resetChangeMenu(); //resets the bool to false

private:
	sf::Texture m_texture;
	sf::Sprite m_license;

	Menu & m_state; //reference to the state holding the current menu
	Menu m_nextState;

	bool m_transitionIn;
	bool m_transitionOut;
	sf::Time m_transitionTime;
	sf::Time m_transitionTotalTime;

	sf::RenderStates m_renderState; //used to position widgets as they are transitioning

	bool m_changeMenu;
};

