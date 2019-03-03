#pragma once

#include "SFML/Graphics.hpp"
#include "Utility.h"
#include "ResourceManager.h"

class Tutorials
{
public:
	Tutorials(ResourceManager & t_resources);
	~Tutorials();

	void update(int t_level, float t_deltaTime);
	void render(sf::RenderWindow & t_window, bool t_hasOrb);
private:
	ResourceManager & m_resource;

	int m_currentLevel;

	sf::Sprite m_move;
	sf::Sprite m_jump;
	sf::Sprite m_punch;
	sf::Sprite m_punchSuccess;
	sf::Sprite m_punchFail;
	sf::Sprite m_orbThrow;
	sf::Sprite m_orbTeleport;
	sf::Sprite m_orbRecall;

	sf::Sprite m_pedastalEmpty;

	int m_currentFrame = 0;
	sf::Sprite m_orb;
	float m_frameTime = 0.5f;
	const float FRAME_DURATION = 0.1f;
};