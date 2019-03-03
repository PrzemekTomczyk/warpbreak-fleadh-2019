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

class CreditsScreen
{
public:
	CreditsScreen(sf::Font & t_font, sf::RenderWindow & t_window, Menu & t_state, Options & options);
	~CreditsScreen();

	void processEvents(Xbox360Controller & t_pad);
	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow & t_window);
	void messageHandler(std::string t_string); //Handles messages recieved from widgets
	void transitionIn(); //Sets up for transitioning out of the menu
	void transitionOut(Menu t_string); //Sets up for transitioning into the menu
	bool getChangeMenu(); //Returns wheather the menu has finished transitioning out
	void resetChangeMenu(); //resets the bool to false

private:
	int const CHARACTER_SIZE = 25;
	void initGui(); //initializes all of the widgets
	sf::Font m_arialBlackFont;

	Menu & m_state; //reference to the state holding the current menu
	Menu m_nextState;

	Label m_credit1;
	Label m_credit2;
	TextButton* m_back;

	Frame m_creditsMenu;

	Widget* m_selected; //currently selected widget

	Options & m_options; //reference to options data ie. animation speed + characterSize

	bool m_transitionIn;
	bool m_transitionOut;
	sf::Time m_transitionTime;
	sf::Time m_transitionTotalTime;

	sf::RenderStates m_renderState; //used to position widgets as they are transitioning

	bool m_changeMenu;
};

