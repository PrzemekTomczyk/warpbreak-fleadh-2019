#pragma once

#include "XBOX360CONTROLLER.h"
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include <FSM.h>
#include "Utility.h"
#include <math.h>
#include <ParticlePool.h>
#include "OptionMenu.h"
#include "Orb.h"

class Player
{
public:
	Player(ResourceManager & t_resources, Input & t_input, Xbox360Controller &t_controller, sf::RenderWindow & t_window, ParticlePool & t_pool, Options & t_options, b2World& m_world);
	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow & t_window);

	sf::Vector2f getPos() const { return m_position; };
	bool isFacingLeft();
	void setPos(b2Vec2 t_pos) { m_position.x = (t_pos.x * SCALE); m_position.y = (t_pos.y * SCALE); };

	void movePos(sf::Vector2f t_vec);

	int getFrameWidth();
	int getFrameHeight();
	void setAirborn(bool t_bool);
	void setHasOrb(bool t_orb);
	void resetHasOrb();
	bool getHasOrb() { return m_hasOrb; };
	bool isPunching();

	void playStep();
	void playLanding();

	void setupPlayer(b2World &t_world, bool t_fixedRotaion, uint16 categoryBits);
	void teleportPlayer(b2Vec2 t_pos, uint16 categoryBits);
	void setB2Body(b2Body* t_body);
	b2Body* getB2Body();

	void setUserData();

	void setAlive(bool t_alive) { m_alive = t_alive; };
	bool getAlive() { return m_alive; };
	int getLives() { return m_lives; };
	float getChargeScale();
	void loseLife() { m_lives--; };

	void setStartPos(sf::Vector2f t_pos) { m_startPosition = t_pos;};

	void setLives(int t_lives);

	float getOrbCooldown() const;

private:
	Xbox360Controller &m_controller;

	// instantiate events
	Idle idle;
	Run run;
	Jump jump;
	Fall fall;
	Punch punch;
	// functions
	void handleMovement(float t_time);
	void handlePunching(float t_time);
	void handleOrb(float t_time);
	void createPunchFixture();
	void handleSpriteFacingDirection();
	void setAnimRect(AnimatedSprite& anim, int frames, int framesPerLine, int width, int height, int pos);
	void updateSize();
	void jumpHandler();
	void step();
	void debugDraw(sf::RenderWindow & t_window);

	// consts
	const int MASS;
	const int SIZE;
	const int SPEED;
	const int MAX_SPEED;
	const int JUMP_VELO;
	const sf::Vector2f STARTING_POS;
	const int MAX_FRAMES;
	const float TIME_PER_FRAME;
	
	//Objects
	Orb m_orb;

	// variables
	Vector2f m_position;
	Vector2f m_startPosition;
	bool m_alive = false;
	bool m_hasOrb;
	int m_lives;
	int m_width = 0;
	int m_height = 0;

	ResourceManager & m_resourceMng;
	ParticlePool& m_particlePool;
	Input & m_input;

	sf::Sound m_stepSound;
	sf::Sound m_landSound;
	sf::Sound m_warpSound;
	sf::Sound m_pickupSound;

	bool m_facingLeft = false;
	bool m_airborn = false;
	bool m_isPunching = false;
	float m_punchCooldown;
	float m_orbDelay;
	float m_orbCooldown;

	Font m_font;

	// box2d
	b2World& m_world;
	b2Body* m_b2body;
	b2Fixture* m_punchFixture;

	Options & m_options;

	UserData m_userData;

	FSM fsm;
};

