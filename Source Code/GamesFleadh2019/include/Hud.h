#pragma once

#include "SFML/Graphics.hpp"
#include "Utility.h"
#include "ResourceManager.h"
#include "Thor/Time.hpp"

class Hud
{
public:
	Hud(ResourceManager & t_resources);
	~Hud();

	void update(float t_chargeScale, int t_lives, float t_deltaTime);
	void reset();
	void render(sf::RenderWindow & t_window, bool t_hasOrb);
private:
	ResourceManager & m_resource;

	thor::StopWatch m_gameTimer;

	sf::Text m_timer;
	sf::RectangleShape m_orbCharge;
	sf::RectangleShape m_orbChargeBorder;
	sf::Text m_lives;

	sf::Vector2f m_orbChargeMaxSize;
};