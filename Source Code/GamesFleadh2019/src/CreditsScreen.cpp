#include "CreditsScreen.h"



CreditsScreen::CreditsScreen(sf::Font & t_font, sf::RenderWindow & t_window, Menu & t_state, Options & options) :
	m_credit1("Emmett Byrne", t_font),
	m_credit2("Przemek Tomczyk", t_font),
	m_creditsMenu(t_window),
	m_state(t_state),
	m_options(options)
{
	initGui();
	m_transitionTotalTime = sf::seconds(2);

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;

	m_credit1.setPosition(sf::Vector2f(400, 200));
	m_credit2.setPosition(sf::Vector2f(400, 300));
}


CreditsScreen::~CreditsScreen()
{
}

void CreditsScreen::processEvents(Xbox360Controller & t_pad)
{
	if (!m_transitionIn && !m_transitionOut)
	{
		if (t_pad.m_currentState.DpadUp && !t_pad.m_previousState.DpadUp)
		{
			messageHandler(m_selected->getUp()); //returns a string 
		}
		if (t_pad.m_currentState.DpadDown && !t_pad.m_previousState.DpadDown)
		{
			messageHandler(m_selected->getDown());
		}
		if (t_pad.m_currentState.DpadLeft && !t_pad.m_previousState.DpadLeft)
		{
			messageHandler(m_selected->getLeft());
		}
		if (t_pad.m_currentState.DpadRight && !t_pad.m_previousState.DpadRight)
		{
			messageHandler(m_selected->getRight());
		}
		if (t_pad.m_currentState.A && !t_pad.m_previousState.A ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
		{
			messageHandler(m_selected->getA());
		}
	}
}

void CreditsScreen::update(sf::Time t_deltaTime)
{
	m_back->setCharacterSize(m_options.fontSize * 5 + 10);
	m_credit1.setCharacterSize(m_options.fontSize * 5 + 10);
	m_credit2.setCharacterSize(m_options.fontSize * 5 + 10);

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

void CreditsScreen::render(sf::RenderWindow & t_window)
{
	sf::RenderStates state = m_renderState;
	state.transform.translate(0, 400);

	t_window.draw(m_creditsMenu, state);
	t_window.draw(m_credit1, m_renderState);
	t_window.draw(m_credit2, m_renderState);
}

void CreditsScreen::messageHandler(std::string t_string)
{
	if (t_string == "back")
	{
		transitionOut(Menu::StateMainMenu);
	}
}

void CreditsScreen::transitionIn()
{
	m_selected->deselect();
	m_selected = m_back;
	m_selected->select();

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;

	m_transitionIn = true;

	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);
}

void CreditsScreen::transitionOut(Menu t_string)
{
	m_transitionOut = true;
	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);

	m_nextState = t_string;
}

bool CreditsScreen::getChangeMenu()
{
	return m_changeMenu;
}

void CreditsScreen::resetChangeMenu()
{
	m_changeMenu = false;
}

void CreditsScreen::initGui()
{
	if (!m_arialBlackFont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	VLayout* layout = new VLayout;
	layout->setSpace(25);

	m_back = new TextButton("Back", m_arialBlackFont, 150);
	m_back->setCharacterSize(CHARACTER_SIZE);
	layout->add(m_back);

	m_creditsMenu.setLayout(layout);

	m_back->setA("back");

	m_selected = m_back;
	m_selected->select();
}
