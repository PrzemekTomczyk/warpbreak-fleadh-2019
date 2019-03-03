#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D.h"	
#include <random>

const sf::Vector2f SCREEN_SIZE{ 1920.0f, 1080.0f };
const sf::Vector2f SCREEN_CENTER{ SCREEN_SIZE / 2.0f };
const int PIXELS_TO_METERS{ 50 };
const float FRICTION{ 0.99f };
const sf::Vector2f GRAVITY{ 0.0f, 50.0f * PIXELS_TO_METERS };
const sf::Vector2f PLANE_SIZE{ SCREEN_SIZE.x, 80.0f };
const sf::Vector2f PLANE_POS{ SCREEN_CENTER.x, SCREEN_SIZE.y - (PLANE_SIZE.y / 2.0f) };
const float SCALE = 64;

/// <summary>
/// Entity IDs for user data
/// 0 = player
/// 1 = shielder
/// 2 = charger
/// 3 = bullet
/// 10 = world
/// 11 = orb
/// 12 = exit
/// </summary>

enum EnemyType
{
	ShieldBoi = 1,
	ChargerBoi = 2,
	TurretBoi = 3
};

//Any state an enemy can be goes here
enum class EnemyState
{
	Patrol,
	Player_Spotted,
	Idle,
	Shield,
	Charging,
	Attack,
};

enum class EndState
{
	WIN,
	LOSE
};

struct UserData
{
	int entity;
	void* data;
};

enum _entityCategory {
	PLAYER =			0x0001,
	ORB =				0x0002,
	ENEMY =				0x0004,
	WORLD =				0x0008,
	PHASE_WALL =		0x0010,
	EXIT =				0x0020,
};