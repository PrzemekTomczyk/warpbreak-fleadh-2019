#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Utility.h"
#include "yaml-cpp/yaml.h"

// A struct to represent Obstacle data in the level
struct ObstacleData {
	sf::Vector2f m_position;
	sf::Vector2f m_size;
};

// A struct to represent Crate data in the level
struct CrateData {
	sf::Vector2f m_position;
	sf::Vector2f m_size;
};

// A struct to represent Player data in the level 
struct PlayerData
{
	sf::Vector2f m_position;
	bool m_fixedRotation;
};

// A struct to represent Player data in the level 
struct EnemyData
{
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	bool m_fixedRotation;
	int m_direction = 1;

	int m_type;
};

// A struct to represent Exit data in the level 
struct ExitData
{
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::Vector2f m_playerSpawn;
	int m_toLevel;
};

struct PhaseWallData
{
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::Vector2f m_direction;
	int m_duration;
};

// A struct to represent all the Level Data
struct LevelData
{
	PlayerData m_player;
	std::vector<ObstacleData> m_obstacles;
	std::vector<CrateData> m_crates;
	std::vector<EnemyData> m_enemies;
	std::vector<ExitData> m_exits;
	std::vector<PhaseWallData> m_phaseWalls;
};

// A class to manage level loading using YAML
class LevelLoader
{
public:
	LevelLoader() = default;

	static bool load(int nr, LevelData& t_level);
};

