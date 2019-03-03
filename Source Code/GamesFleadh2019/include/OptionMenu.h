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

class OptionMenu
{
public:
	/// <summary>
	/// default constructor
	/// </summary>
	/// <param name="t_font">a reference to a font</param>
	/// <param name="t_window">reference to the render window for the frame</param>
	/// <param name="state">reference to the game state</param>
	/// <param name="t_options">a reference to the game settings in the options screen</param>
	OptionMenu(sf::Font & t_font, sf::RenderWindow & t_window, Menu & state, Options & options);
	~OptionMenu();

	void processEvents(Xbox360Controller & pad);
	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow & t_window);
	void messageHandler(std::string s); //Handles messages recieved from widgets
	void transitionIn(); //Sets up for transitioning out of the menu
	void transitionOut(Menu s); //Sets up for transitioning into the menu
	bool getChangeMenu(); //Returns wheather the menu has finished transitioning out
	void resetChangeMenu(); //resets the bool to false


private:
	int const CHARACTER_SIZE = 25;
	void initGui(); //initializes all of the widgets
	sf::Font &m_arialBlackFont;

	Menu & m_state; //reference to the state holding the current menu
	Menu m_nextState;

	Label m_title;
	Slider* m_musicVol;
	Slider* m_soundVol;
	CheckButton* m_mute;
	Slider* m_animationSpeed;
	RadioButton* m_fontSize;
	TextButton* m_back;

	Frame m_optionMenu;

	Widget* m_selected; //currently selected widget

	Options & m_options; //reference to options data ie. animation speed + characterSize

	int m_radioValue = 2;

	bool m_transitionIn;
	bool m_transitionOut;
	sf::Time m_transitionTime;
	sf::Time m_transitionTotalTime;

	sf::RenderStates m_renderState; //used to position widgets as they are transitioning

	bool m_changeMenu;
};

