#include "EndScreen.h"

EndScreen::EndScreen(Menu & state, Options & t_options, ResourceManager & t_resourceMng, Input & t_input, Xbox360Controller & t_controller) :
	m_resourceMng(t_resourceMng),
	m_state(state),
	m_input(t_input),
	m_options(t_options),
	m_controller(t_controller)
{
	m_transitionTotalTime = sf::seconds(2);

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;
}


EndScreen::~EndScreen()
{
}

void EndScreen::update(sf::Time t_deltaTime)
{
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

	if (!m_transitionIn && !m_transitionOut)
	{
		if (m_controller.m_currentState.Start || m_input.m_current.one)
		{
			// restart at level 1
			transitionOut(Menu::StateGameplay);
		}
		else if (m_controller.m_currentState.Back || m_input.m_current.two)
		{
			// go to main menu
			transitionOut(Menu::StateMainMenu);
		}
	}
}

void EndScreen::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_endSprite, m_renderState);
}

bool EndScreen::getChangeMenu()
{
	return m_changeMenu;
}

void EndScreen::resetChangeMenu()
{
	m_changeMenu = false;
}

void EndScreen::initEnd(EndState t_state)
{
	m_state = Menu::StateEnd;
	m_endState = t_state;

	switch (m_endState)
	{
	case EndState::WIN:
		m_endSprite.setTexture(m_resourceMng.getTexture(TextureID::WIN));
		break;
	case EndState::LOSE:
		m_endSprite.setTexture(m_resourceMng.getTexture(TextureID::LOSE));
		break;
	default:
		break;
	}
}

void EndScreen::transitionIn()
{
	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;

	m_transitionIn = true;

	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);
}

void EndScreen::transitionOut(Menu s)
{
	m_transitionOut = true;
	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);

	m_nextState = s;
}