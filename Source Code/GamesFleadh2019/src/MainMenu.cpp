#include "MainMenu.h"



MainMenu::MainMenu(sf::Font & font, sf::RenderWindow & window, Menu & state, Options & options) :
	m_title("Main Menu", font),
	m_mainMenu(window),
	m_state(state),
	m_options(options),
	m_ArialBlackfont(font)
{
	initGui();
	m_transitionTotalTime = sf::seconds(2);

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;
}


MainMenu::~MainMenu()
{
}

void MainMenu::initGui()
{
	//initialising all the pointers to actual buttons
	m_play = new TextButton("Play", m_ArialBlackfont, 280);
	m_optionsButton = new TextButton("Options", m_ArialBlackfont, 280);
	m_credits = new TextButton("Credits", m_ArialBlackfont, 280);
	m_quit = new TextButton("Quit", m_ArialBlackfont, 280);

	//setting their size
	m_play->setCharacterSize(15);
	m_optionsButton->setCharacterSize(15);
	m_credits->setCharacterSize(15);
	m_quit->setCharacterSize(15);

	//setting the strings that will be returned on a button input
	m_play->setDown("options");
	m_play->setA("playmenu"); 

	m_optionsButton->setUp("play");
	m_optionsButton->setDown("credits");
	m_optionsButton->setA("optionsmenu");

	m_credits->setUp("options");
	m_credits->setDown("quit");
	m_credits->setA("creditsmenu");

	m_quit->setUp("credits");
	m_quit->setA("quitmenu");

	//adding the buttons to the layout
	VLayout* layout = new VLayout;
	layout->setSpace(25);
	layout->add(m_play);
	layout->add(m_optionsButton);
	layout->add(m_credits);
	layout->add(m_quit);

	m_mainMenu.setLayout(layout);

	//setting the current selected widget
	m_selected = m_play;
	m_selected->select();
}

//takes in a string and does something based on the string
void MainMenu::messageHandler(std::string s)
{
	if (s == "play")
	{
		m_selected->deselect(); //changes colour back to default
		m_selected = m_play; //changes the currently selected widget to this
		m_selected->select(); //changes the colour so that it looks like it's selected
		std::cout << "play" << std::endl;
	}

	if (s == "options")
	{
		m_selected->deselect();
		m_selected = m_optionsButton;
		m_selected->select();
		std::cout << "options" << std::endl;
	}

	if (s == "credits")
	{
		m_selected->deselect();
		m_selected = m_credits;
		m_selected->select();
		std::cout << "credits" << std::endl;
	}

	if (s == "quit")
	{
		m_selected->deselect();
		m_selected = m_quit;
		m_selected->select();
		std::cout << "quit" << std::endl;
	}

	if (s == "playmenu")
	{
		//pass in a reference to the current manu state and change it to PlayMenu
		transitionOut(Menu::StateGameplay);
	}

	if (s == "optionsmenu")
	{
		transitionOut(Menu::StateOptions);
	}

	if (s == "creditsmenu")
	{
		transitionOut(Menu::StateCredits);
	}

	if (s == "quitmenu")
	{
		transitionOut(Menu::StateQuit);
	}
}

void MainMenu::processEvents(Xbox360Controller & pad)
{

	if (!m_transitionIn && !m_transitionOut)
	{
		if (pad.m_currentState.DpadUp && !pad.m_previousState.DpadUp)
		{
			messageHandler(m_selected->getUp()); //returns a string 
		}
		if (pad.m_currentState.DpadDown && !pad.m_previousState.DpadDown)
		{
			messageHandler(m_selected->getDown());
		}
		if (pad.m_currentState.DpadLeft && !pad.m_previousState.DpadLeft)
		{
			messageHandler(m_selected->getLeft());
		}
		if (pad.m_currentState.DpadRight && !pad.m_previousState.DpadRight)
		{
			messageHandler(m_selected->getRight());
		}
		if (pad.m_currentState.A && !pad.m_previousState.A ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			messageHandler(m_selected->getA());
		}
	}
}

void MainMenu::update(sf::Time t_deltaTime)
{
	m_play->setCharacterSize(m_options.fontSize * 5 + 10);
	m_optionsButton->setCharacterSize(m_options.fontSize * 5 + 10);
	m_credits->setCharacterSize(m_options.fontSize * 5 + 10);
	m_quit->setCharacterSize(m_options.fontSize * 5 + 10);

	double scaler;
	if (m_transitionIn || m_transitionOut)
	{
		m_transitionTime -= t_deltaTime;
		scaler = t_deltaTime.asSeconds() / (m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);	
	}

	if (m_transitionIn)
	{
		m_renderState.transform.translate(sf::Vector2f(0, scaler * 1200));

		if (m_transitionTime.asSeconds() < 0)
		{
			m_transitionIn = false;

			m_renderState = sf::Transform();
		}
	}

	if (m_transitionOut)
	{
		m_renderState.transform.translate(sf::Vector2f(0, scaler * -1200));
		if (m_transitionTime.asSeconds() < 0)
		{
			m_transitionOut = false;
			m_state = m_nextState;
			m_changeMenu = true;
		}
	}
}

void MainMenu::render(sf::RenderWindow & window)
{
	window.draw(m_title, m_renderState);
	window.draw(m_mainMenu, m_renderState);
}

void MainMenu::transitionIn()
{
	m_selected->deselect();
	//m_selected = m_play;
	m_selected->select();

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;

	m_transitionIn = true;

	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);
}

void MainMenu::transitionOut(Menu s)
{
	m_transitionOut = true;
	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);

	m_nextState = s;
}

bool MainMenu::getChangeMenu()
{
	return m_changeMenu;
}

void MainMenu::resetChangeMenu()
{
	m_changeMenu = false;
}
