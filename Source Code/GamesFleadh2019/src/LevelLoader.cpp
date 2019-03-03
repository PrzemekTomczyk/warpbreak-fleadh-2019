#include "LevelLoader.h"

/// <summary>
/// Extracts Player data
/// </summary>
/// <param name="t_playerNode">A YAML node</param>
/// <param name="t_player">A simple struct to store the player data</param>
void operator >> (const YAML::Node& t_playerNode, PlayerData& t_player){
	t_player.m_position.x = t_playerNode["position"]["x"].as<float>();
	t_player.m_position.y = t_playerNode["position"]["y"].as<float>();

	t_player.m_fixedRotation = t_playerNode["rotation"].as<bool>();
}

/// <summary>
/// Extracts Obstacle data
/// </summary>
/// <param name="t_obstacleNode">A YAML node</param>
/// <param name="t_obstacle">A simple struct to store the Obstacle data</param>
void operator >> (const YAML::Node& t_obstacleNode, ObstacleData& t_obstacle) {
	t_obstacle.m_position.x = t_obstacleNode["position"]["x"].as<float>();
	t_obstacle.m_position.y = t_obstacleNode["position"]["y"].as<float>();
	t_obstacle.m_size.x = t_obstacleNode["size"]["x"].as<float>();
	t_obstacle.m_size.y = t_obstacleNode["size"]["y"].as<float>();
}

/// <summary>
/// Extracts Crate data
/// </summary>
/// <param name="t_obstacleNode">A YAML node</param>
/// <param name="t_obstacle">A simple struct to store the Crate data</param>
void operator >> (const YAML::Node& t_crateNode, CrateData& t_crate) {
	t_crate.m_position.x = t_crateNode["position"]["x"].as<float>();
	t_crate.m_position.y = t_crateNode["position"]["y"].as<float>();
	t_crate.m_size.x = t_crateNode["size"]["x"].as<float>();
	t_crate.m_size.y = t_crateNode["size"]["y"].as<float>();
}

/// <summary>
/// Extracts Exit data
/// </summary>
/// <param name="t_obstacleNode">A YAML node</param>
/// <param name="t_obstacle">A simple struct to store the Exit data</param>
void operator >> (const YAML::Node& t_exitNode, ExitData& t_exit) {
	//load position of this door
	t_exit.m_position.x = t_exitNode["position"]["x"].as<float>();
	t_exit.m_position.y = t_exitNode["position"]["y"].as<float>();

	//load size of this door
	t_exit.m_size.x = t_exitNode["size"]["x"].as<float>();
	t_exit.m_size.y = t_exitNode["size"]["y"].as<float>();

	//load player spawn location when entering through this door
	t_exit.m_playerSpawn.x = t_exitNode["playerStartPos"]["x"].as<float>();
	t_exit.m_playerSpawn.y = t_exitNode["playerStartPos"]["y"].as<float>();

	//load where this level leads to
	t_exit.m_toLevel = t_exitNode["leadToLevel"].as<int>();
}

/// <summary>
/// Extracts Enemy data
/// </summary>
/// <param name="t_enemyNode">A YAML node</param>
/// <param name="t_enemy">A simple struct to store the Obstacle data</param>
void operator >> (const YAML::Node& t_enemyNode, EnemyData& t_enemy) {
	t_enemy.m_position.x = t_enemyNode["position"]["x"].as<float>();
	t_enemy.m_position.y = t_enemyNode["position"]["y"].as<float>();

	t_enemy.m_size.x = t_enemyNode["size"]["x"].as<float>();
	t_enemy.m_size.y = t_enemyNode["size"]["y"].as<float>();

	t_enemy.m_fixedRotation = t_enemyNode["rotation"].as<bool>();
	t_enemy.m_direction = t_enemyNode["direction"].as<int>();

	t_enemy.m_type = t_enemyNode["type"].as<int>();
}

void operator >> (const YAML::Node& t_phaseWallNode, PhaseWallData& t_phaseWall) {
	t_phaseWall.m_position.x = t_phaseWallNode["position"]["x"].as<float>();
	t_phaseWall.m_position.y = t_phaseWallNode["position"]["y"].as<float>();

	t_phaseWall.m_size.x = t_phaseWallNode["size"]["x"].as<float>();
	t_phaseWall.m_size.y = t_phaseWallNode["size"]["y"].as<float>();

	t_phaseWall.m_direction.x = t_phaseWallNode["direction"]["x"].as<float>();
	t_phaseWall.m_direction.y = t_phaseWallNode["direction"]["y"].as<float>();

	t_phaseWall.m_duration = t_phaseWallNode["duration"].as<int>();
}

/// <summary>
/// Level function that extracts various game data from the YAML data structure
/// Invokes other functions to extract the player, obstacles and enemies
/// Because there are multiple obstacles and enemies, they are stores in a vector
/// </summary>
/// <param name="t_levelNode">A YAML node</param>
/// <param name="t_level">A simple struct to store data</param>
void operator >> (const YAML::Node& t_levelNode, LevelData& t_level){
	t_levelNode["player"] >> t_level.m_player;

	const YAML::Node& obstacleNode = t_levelNode["obstacles"].as<YAML::Node>();
	for (unsigned i = 0; i < obstacleNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstacleNode[i] >> obstacle;
		t_level.m_obstacles.push_back(obstacle);
	}

	const YAML::Node& crateNode = t_levelNode["crates"].as<YAML::Node>();
	for (unsigned i = 0; i < crateNode.size(); ++i)
	{
		CrateData crate;
		crateNode[i] >> crate;
		t_level.m_crates.push_back(crate);
	}

	const YAML::Node& exitNode = t_levelNode["exits"].as<YAML::Node>();
	for (unsigned i = 0; i < exitNode.size(); ++i)
	{
		ExitData exit;
		exitNode[i] >> exit;
		t_level.m_exits.push_back(exit);
	}

	const YAML::Node& enemyNode = t_levelNode["enemies"].as<YAML::Node>();
	for (unsigned i = 0; i < enemyNode.size(); ++i)
	{
		EnemyData enemy;
		enemyNode[i] >> enemy;
		t_level.m_enemies.push_back(enemy);
	}

	const YAML::Node& phaseWallNode = t_levelNode["phaseWall"].as<YAML::Node>();
	for (unsigned i = 0; i < phaseWallNode.size(); ++i)
	{
		PhaseWallData phaseWall;
		phaseWallNode[i] >> phaseWall;
		t_level.m_phaseWalls.push_back(phaseWall);
	}
}

/// <summary>
/// Function to load the level from the YAML file
/// </summary>
/// <param name="t_level"></param>
/// <returns></returns>
bool LevelLoader::load(int nr, LevelData& t_level)
{
	std::stringstream ss;
	ss << "ASSETS/LEVELS/level";
	ss << nr;
	ss << ".yaml";

	//clear vectors when loading again
	t_level.m_obstacles.clear();
	t_level.m_crates.clear();
	t_level.m_enemies.clear();
	t_level.m_exits.clear();
	t_level.m_phaseWalls.clear();

	try
	{
		YAML::Node baseNode = YAML::LoadFile(ss.str());
		if (baseNode.IsNull())
		{
			std::string message("file: " + ss.str() + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> t_level;
	}
	catch (YAML::ParserException& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}

	return true;
}
