#pragma once
#include "Enemy.h"
#include "AnimatedSprite.h"
#include <FSM.h>
#include <SFML/Audio.hpp>
#include "Thor/Time.hpp"

class Charger : public Enemy
{
public:
	Charger(ResourceManager & t_resourceMng, Options & t_options);
	~Charger();

	/// <summary>
	/// update function for the charger
	/// </summary>
	/// <param name="t_deltaTime">time since last update</param>
	/// <param name="t_player">pointer to player's body</param>
	/// <param name="t_world">reference to box2d world</param>
	void update(float t_deltaTime, b2Body* t_player, b2World &t_world);

	/// <summary>
	/// sets up user data
	/// </summary>
	void setUserData();

	/// <summary>
	/// deal damage to charger
	/// </summary>
	/// <returns></returns>
	bool takeDamage(bool left);

	void handleCurrentAnim();

	void render(sf::RenderWindow &t_window);

private:
	void handleCharge(b2World &t_world);

	const float CHARGE_ANGLE_RAD = 1.5708f;
	b2Vec2 m_rayP1;
	b2Vec2 m_rayP2;
	b2Vec2 m_intersectionPoint;

	sf::Sound m_alertSound;
	bool m_playAlert = true;
	thor::Timer m_chargeDelay;
	AnimatedSprite* m_currentAnim;
	AnimatedSprite m_idleAnim;
	AnimatedSprite m_walkAnim;
	AnimatedSprite m_runAnim;
};

