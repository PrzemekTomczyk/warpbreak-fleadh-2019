#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Utility.h"
#include <math.h>
#include <ParticlePool.h>
#include "OptionMenu.h"
#include "LevelLoader.h"
#include "AnimatedSprite.h"

class Enemy
{
public:
	/// <summary>
	/// Enemy default constructor to set references etc
	/// </summary>
	/// <param name="t_resourceMng">reference to game's resource manager</param>
	/// <param name="t_options">reference to game's option settings</param>
	/// <param name="t_size">size for the b2body</param>
	/// <param name="t_name">name or type of the enemy</param>
	Enemy(ResourceManager & t_resourceMng, Options & t_options);
	virtual ~Enemy();

	/// <summary>
	/// virtual init funcion for enemies that require it
	/// </summary>
	virtual void init(sf::Vector2f t_pos, bool t_rotation, sf::Vector2f t_size, b2World &t_world, int t_entityType, int t_direction);

	/// <summary>
	/// virtual funtion to clear the body from the world
	/// </summary>
	virtual void clear();

	/// <summary>
	/// pure virtual function to update the enemy
	/// </summary>
	virtual void update(float t_deltaTime, b2Body* t_player, b2World &t_world) = 0;

	/// <summary>
	/// pure vitual function to render simple sfml rectangle
	/// shape to display box2d hitbox
	/// </summary>
	/// <param name="t_window">reference to the main game render window</param>
	virtual void render(sf::RenderWindow &t_window);

	/// <summary>
	/// virtual function to setup the box2d body for the enemy.
	/// Sets up b2BodyDef, *b2Body, b2Shape and b2Fixture
	/// </summary>
	/// <param name="t_world">reference to the box2d world for physics</param>
	/// <param name="t_pos">position where to place the enemy</param>
	/// <param name="t_fixedRotaion">bool to set fixedRotation of the body</param>
	virtual void setupEnemy(b2World &t_world, sf::Vector2f t_pos, bool t_fixedRotaion, sf::Vector2f t_size, int t_entityType, int t_direction, uint16 categoryBits, uint16 maskBits);

	virtual void setUserData() = 0;

	/// <summary>
	/// function to lower enemy's health when he gets hit
	/// </summary>
	/// <returns>true if enemy has been killed or false 
	/// if enemy is still alive</returns>
	virtual bool takeDamage(bool left);

	/// <summary>
	/// function to set the b2Body* for the enemy
	/// </summary>
	/// <param name="t_body"></param>
	void setB2Body(b2Body* t_body);

	/// <summary>
	/// gets m_alive
	/// </summary>
	/// <returns>m_alive as bool</returns>
	bool getAlive() const;

	/// <summary>
	/// kills the enemy by setting health to 0
	/// </summary>
	virtual void kill();

	/// <summary>
	/// function to get health
	/// </summary>
	/// <returns>m_health of type int</returns>
	int getHealth();

	/// <summary>
	/// handles raycasting for the enemy
	/// </summary>
	/// <param name="t_direction">direction in which the enemy is moving in</param>
	/// <param name="t_world">reference to the b2world</param>
	/// <returns>true if enemy has to turn around, false if he can keep going</returns>
	bool handleRaycast(b2World &t_world);

	void setAnimRect(AnimatedSprite & anim, int frames, int framesPerLine, int width, int height, int pos);

// protected variables
protected:
	// reference to options for sound volume etc
	Options &m_options;

	// reference to resourceManager for textures etc
	ResourceManager &m_resourceMng;

	//pointer to enemy's b2body for posiotion etc
	b2Body *m_b2body;
	int m_width;
	int m_height;

	// is this enemy alive
	bool m_alive = true;

	//state enemy is in
	EnemyState m_state;

	// speed
	float m_speed = 4.0f;
	//speed when patrolling
	const float PATROL_SPEED = 4.0f;
	// speed when player in sight
	const float BOOST_SPEED = 20.0f;

	//user data struct for box2d userData
	UserData m_userData;

	//health of the enemy
	int m_health;

	/// <summary>
	/// draws debug shape of box2d 
	/// </summary>
	/// <param name="t_window"></param>
	void drawDebug(sf::RenderWindow &t_window);

	float m_rayAngle = 0.0f;

	bool m_hitWall = false;
	float m_wallTimer = 0.0f;
	int m_direction = 1;
	bool m_stunned = false;
	float m_stunCD = 0.0f;

// private variables
private:
	const float WALKING_STICK_ANGLE = 15.0f;
	b2Vec2 m_rayP1;
	b2Vec2 m_rayP2;
	b2Vec2 m_intersectionPoint;
};