#pragma once
#include "Utility.h"
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "OptionMenu.h"
#include "Thor/Time.hpp"

class Bullet
{
public:
	Bullet(ResourceManager & t_resourceMng, Options & t_options);
	~Bullet();

	void update(float t_deltaTime);
	void render(sf::RenderWindow &t_window);

	void init(sf::Vector2f t_position, sf::Vector2f t_heading, b2World &t_world);

	void destroy(b2World &t_world);
	void kill();

	void setB2Body(b2Body* t_body);
	b2Body* getB2Body() const;

	bool getAlive() const;
	bool toDestroy() const;

	void setUserData();

	void setTimer();

private:
	ResourceManager &m_resourceMng;
	Options &m_options;

	sf::Vector2f m_pos;

	b2Body *m_b2body;

	sf::Sprite m_sprite;

	bool m_alive = false;
	bool m_toBeDestroyed = false;

	UserData m_userData;
	
	thor::Timer m_aliveTimer;
};

