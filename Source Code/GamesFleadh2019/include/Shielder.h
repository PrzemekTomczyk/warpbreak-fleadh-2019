#pragma once
#include "Enemy.h"
#include "FSM.h"

class Shielder : public Enemy
{
public:
	Shielder(ResourceManager & t_resourceMng, Options & t_options);
	~Shielder();

	/// <summary>
	/// update function for the shielder
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
	/// function to make shielder take damage
	/// </summary>
	/// <param name="left">is shielder being punched from the left</param>
	/// <returns>true if shielder is alive</returns>
	bool takeDamage(bool left);

	void handleCurrentAnim();

	void render(sf::RenderWindow &t_window);

private:
	bool playerInFront(b2World &t_world);
	const float FORWARD_DETECTION_RAD = 1.5708f;
	b2Vec2 m_rayP1;
	b2Vec2 m_rayP2;
	b2Vec2 m_intersectionPoint;

	AnimatedSprite* m_currentAnim;
	AnimatedSprite m_idleAnim;
	AnimatedSprite m_walkAnim;
};

