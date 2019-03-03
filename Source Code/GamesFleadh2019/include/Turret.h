#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Utility.h"
#include <math.h>
#include "OptionMenu.h"
#include "LevelLoader.h"
#include "Player.h"
#include "Bullet.h"

class Turret
{
public:
	Turret(ResourceManager & t_resourceMng, Options & t_options, b2World & t_world);
	~Turret();

	void update(float t_deltaTime, Player &t_player);
	void render(sf::RenderWindow &t_window);

	void init(sf::Vector2f t_pos, int t_entityType, sf::Vector2f t_angles, int t_direction);
	void clear();

private:
	void handlePatrol();
	void handleTracking(sf::Vector2f t_playerPos);
	void drawDebug(sf::RenderWindow &t_window);
	bool handleRaycast();

	ResourceManager &m_resourceMng;
	Options &m_options;

	EnemyState m_state;

	sf::CircleShape m_base;
	sf::RectangleShape m_turret;
	
	sf::Vector2f m_pos;
	float m_radius;

	//variables for raycast
	b2Vec2 m_rayP1;
	b2Vec2 m_rayP2;
	b2Vec2 m_intersectionPoint;

	b2World &m_world;

	float m_angularVelo = 1.0f;

	sf::Vector2f m_patrolAngles;

	Bullet m_bullet;

	int m_entityId;
	float m_angle;
};

