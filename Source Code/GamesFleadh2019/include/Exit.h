#pragma once

#include "SFML/Graphics.hpp"
#include "Utility.h"
#include <functional>

class Exit
{
public:
	Exit(b2World &t_world, sf::Vector2f t_pos, sf::Vector2f t_size, int t_level, sf::Vector2f t_playerSpawn, uint16 categoryBits);
	~Exit();

	void destroy();
	void hit();
	bool hasExited();
	void debugDraw(sf::RenderWindow & t_window); 
	int getLevel();
	sf::Vector2f getPlayerSpawn() const;

	void setUserData();
private:
	sf::Vector2f m_position;
	UserData m_userData;

	b2World& m_world;
	b2Body* m_b2body;

	bool m_exit = false;
	sf::Vector2f m_playerSpawn;
	int m_toLevel;

};

