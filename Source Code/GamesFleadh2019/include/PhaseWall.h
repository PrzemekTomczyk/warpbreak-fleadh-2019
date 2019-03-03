#pragma once

#include "SFML/Graphics.hpp"
#include "Utility.h"
#include "ResourceManager.h"

class PhaseWall
{
public:
	PhaseWall(ResourceManager & t_resources, b2World &t_world, sf::Vector2f t_pos, sf::Vector2f t_size, b2Vec2 t_direction, float t_duration);

	void update(float t_deltaTime);
	void render(sf::RenderWindow & t_window);
	void initialize(sf::Vector2f t_pos, sf::Vector2f t_size);
	void makeBody(uint16 categoryBits);
	void setUserData();
	void destroy();
	void setB2Body(b2Body * t_body);
private:
	sf::RectangleShape m_body;
	UserData m_userData;

	b2World& m_world;
	b2Body* m_b2body;
	sf::Vector2f m_size;


	b2Vec2 m_startPos;
	b2Vec2 m_direction;

	float m_duration;
	float m_timer;

	ResourceManager & m_resourceMng;
};