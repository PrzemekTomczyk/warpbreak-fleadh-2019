#include "Hud.h"

Hud::Hud(ResourceManager & t_resources) :
	m_resource(t_resources),
	m_orbChargeMaxSize(140, 10)
{
	m_lives.setFont(m_resource.getFont(FontID::ARIBLK));
	m_timer.setFont(m_resource.getFont(FontID::ARIBLK));
	m_orbChargeBorder.setFillColor(sf::Color(0,0,0,0));
	m_orbChargeBorder.setOutlineThickness(3);
	m_orbChargeBorder.setOutlineColor(sf::Color(180,40,40,255));
	m_orbChargeBorder.setSize(m_orbChargeMaxSize);

	m_orbCharge.setSize(sf::Vector2f(0, 0));
	m_orbCharge.setFillColor(sf::Color::Green);

	m_gameTimer.start();

	m_lives.setPosition(25, 0);
	m_timer.setPosition(SCREEN_SIZE.x * 0.5f, 0);
	m_orbCharge.setPosition(28, 40);
	m_orbChargeBorder.setPosition(28, 40);
}

Hud::~Hud()
{
}

void Hud::update(float t_chargeScale, int t_lives, float t_deltaTime)
{
	int min = int(m_gameTimer.getElapsedTime().asSeconds() / 60);
	int sec = int(int(m_gameTimer.getElapsedTime().asSeconds()) % 60);
	std::string seconds = std::to_string(sec);

	if (sec < 10)
	{
		seconds = "0" + seconds;
	}
	m_timer.setString(std::to_string(min) + ":" + seconds);

	sf::Vector2f temp(m_orbChargeMaxSize.x * t_chargeScale, m_orbChargeMaxSize.y);
	m_orbCharge.setSize(temp);

	m_lives.setString("Lives: " + std::to_string(t_lives));
}

void Hud::reset()
{
	m_gameTimer.restart();
}

void Hud::render(sf::RenderWindow & t_window, bool t_hasOrb)
{
	t_window.draw(m_timer);
	t_window.draw(m_lives);
	if (t_hasOrb)
	{
		t_window.draw(m_orbCharge);
		t_window.draw(m_orbChargeBorder);
	}
}
