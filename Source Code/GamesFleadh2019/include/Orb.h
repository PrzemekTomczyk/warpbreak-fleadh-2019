#pragma once

#include "SFML/Graphics.hpp"
#include "Utility.h"
#include "ResourceManager.h"
#include "SFML/Audio.hpp"

class Orb
{
public:
	Orb(ResourceManager & t_resources);
	~Orb();

	void render(sf::RenderWindow & t_window);
	void createOrb(b2World &t_world, sf::Vector2f t_pos, bool t_left, bool t_drop, uint16 categoryBits, uint16 maskBits);
	void destroyOrb();
	void setFrame(float t_deltaTime);

	void setSpriteAngle();
	void playCollision();

	b2Vec2 getTeleportPos(float t_pHeight);
	bool getAlive() { return m_alive; };

private:
	UserData m_userData;

	b2Body* m_b2body;
	sf::CircleShape m_shape;
	bool m_alive;
	int m_radius;

	int m_currentFrame = 0;
	sf::Sprite m_sprite;
	float m_frameTime = 0.5f;
	const float FRAME_DURATION = 0.1f;
	std::vector<sf::Sound*> m_colSounds;
	ResourceManager & m_resourceMng;
};

