#pragma once

#include "SFML/Graphics.hpp"
#include "Utility.h"
#include "ResourceManager.h"

class Obstacle
{
public:
	Obstacle(ResourceManager & t_resources, b2World &t_world, sf::Vector2f t_pos, sf::Vector2f t_size, uint16 categoryBits, float t_friction = 0.9f);

	void render(sf::RenderWindow & t_window);
	sf::RectangleShape getBody() const;
	void initialize(sf::Vector2f t_pos, sf::Vector2f t_size);
	void destroy();
	sf::Vector2f getPos() const { return m_position; };
	void setPos(b2Vec2 t_pos) { m_position.x = t_pos.x * SCALE; m_position.y = t_pos.y * SCALE; m_body.setPosition(m_position); };
	void setUserData();

	b2Body* getB2Body();
	UserData getUserData();

private:
	sf::RectangleShape m_body;
	sf::Vector2f m_position;
	UserData m_userData;

	b2World& m_world;
	b2Body* m_b2body;

	ResourceManager & m_resourceMng;
};

