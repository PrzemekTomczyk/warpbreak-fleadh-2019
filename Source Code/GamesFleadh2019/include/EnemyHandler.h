#pragma once

#include "Shielder.h"
#include "Charger.h"
#include "Turret.h"
#include "LevelLoader.h"

class EnemyHandler
{
public:
	EnemyHandler(b2World &t_world, ResourceManager & t_resourceMng, Options & t_options);
	~EnemyHandler();

	/// <summary>
	/// function to initialize enemies
	/// </summary>
	void initEnemies(LevelData &t_level, ResourceManager &t_resourceMng, Options &t_options);

	/// <summary>
	/// function to update all enemies
	/// </summary>
	void update(float t_deltaTime, Player &t_player);

	/// <summary>
	/// renders all enemies
	/// </summary>
	/// <param name="t_window">reference to the main game render window</param>
	void render(sf::RenderWindow &t_window);

private:
	std::vector<Turret> m_turrets;
	std::vector<Enemy*> m_enemies;

	b2World &m_world;
};

