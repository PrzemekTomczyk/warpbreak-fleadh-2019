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

class MainMenu
{
public:
	/// <summary>
	/// default constructor
	/// </summary>
	/// <param name="t_font">a reference to a font</param>
	/// <param name="t_window">reference to the render window for the frame</param>
	/// <param name="state">reference to the game state</param>
	/// <param name="t_options">a reference to the game settings in the options screen</param>
	MainMenu(sf::Font & font, sf::RenderWindow & window, Menu & state, Options & options);
	~MainMenu();
	sf::Font &m_ArialBlackfont;

	void messageHandler(std::string s); //Handles messages recieved from widgets
	void processEvents(Xbox360Controller & pad);
	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow & window);
	void transitionIn(); //Sets up for transitioning out of the menu
	void transitionOut(Menu s); //Sets up for transitioning into the menu
	bool getChangeMenu(); //Returns wheather the menu has finished transitioning out
	void resetChangeMenu(); //resets the bool to false

private:
	void initGui(); //initializes all of the widgets
	Frame m_mainMenu;
	std::string m_playText;

	Menu& m_state;  //reference to the state holding the current menu
	Menu m_nextState;

	Label m_title;
	TextButton* m_play;
	TextButton* m_optionsButton;
	TextButton* m_credits;
	TextButton* m_quit;

	Widget* m_selected; //currently selected widget

	Options & m_options; //reference to options data ie. animation speed + characterSize

	bool m_transitionIn;
	bool m_transitionOut;
	sf::Time m_transitionTime;
	sf::Time m_transitionTotalTime;

	sf::RenderStates m_renderState; //used to position widgets as they are transitioning

	bool m_changeMenu;
};

