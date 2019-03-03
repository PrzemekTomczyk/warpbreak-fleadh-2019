#include "EnemyHandler.h"
#include "Player.h"

EnemyHandler::EnemyHandler(b2World &t_world, ResourceManager & t_resourceMng, Options & t_options) :
	m_world(t_world)
{
}

EnemyHandler::~EnemyHandler()
{
}

void EnemyHandler::initEnemies(LevelData &t_level, ResourceManager &t_resourceMng, Options &t_options)
{
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->clear();
		delete m_enemies.at(i);
	}
	m_enemies.clear();

	for (int i = 0; i < m_turrets.size(); i++)
	{
		m_turrets[i].clear();
	}
	m_turrets.clear();

	for (int i = 0; i < t_level.m_enemies.size(); i++)
	{
		switch (t_level.m_enemies[i].m_type)
		{
		case EnemyType::ShieldBoi: 
		{
			Shielder* shielder = new Shielder(
				t_resourceMng,
				t_options
			);

			shielder->init(
				t_level.m_enemies[i].m_position, // charger's pos
				t_level.m_enemies[i].m_fixedRotation, // is its rotation fixed
				t_level.m_enemies[i].m_size, // charger's size
				m_world, // world to add enemy to
				t_level.m_enemies[i].m_type, // type of entity this is
				t_level.m_enemies[i].m_direction
			);

			m_enemies.push_back(shielder);

			m_enemies[i]->setUserData();
			break;
		}
		case EnemyType::ChargerBoi:
		{
			Charger* charger = new Charger(
				t_resourceMng, 
				t_options
			);

			charger->init(
				t_level.m_enemies[i].m_position, // charger's pos
				t_level.m_enemies[i].m_fixedRotation, // is its rotation fixed
				t_level.m_enemies[i].m_size, // charger's size
				m_world, // world to add enemy to
				t_level.m_enemies[i].m_type, // type of entity this is
				t_level.m_enemies[i].m_direction
			);

			m_enemies.push_back(charger);

			m_enemies[i]->setUserData();
			break;
		}
		case EnemyType::TurretBoi:
		{
			Turret turret(
				t_resourceMng,
				t_options,
				m_world
			);

			turret.init(
				t_level.m_enemies[i].m_position,
				t_level.m_enemies[i].m_type,
				t_level.m_enemies[i].m_size,
				t_level.m_enemies[i].m_direction
			);
			m_turrets.push_back(turret);
			break;
		}
		default:
			break;
		}
	}
}

void EnemyHandler::update(float t_deltaTime, Player &t_player)
{
	for (Enemy *enemy : m_enemies) {
		if (enemy->getHealth() > 0)
		{
			enemy->update(t_deltaTime, t_player.getB2Body(), m_world);
		}
		if (enemy->getHealth() <= 0 && enemy->getAlive())
		{
			enemy->clear();
		}
	}

	for (auto &turret : m_turrets)
	{
		turret.update(t_deltaTime, t_player);
	}
}

void EnemyHandler::render(sf::RenderWindow & t_window)
{
	for (Enemy *enemy : m_enemies) {
		if (enemy->getAlive())
		{
			enemy->render(t_window);
		}
	}

	for (auto &turret : m_turrets)
	{
		turret.render(t_window);
	}
}