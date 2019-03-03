#include "GameScreen.h"
#include "Thor/Vectors.hpp"

GameScreen::GameScreen(sf::RenderWindow & t_window, Menu & state, Options & t_options, ResourceManager & t_resourceMng, Input & t_input, Xbox360Controller & t_controller) :
	m_resourceMng(t_resourceMng),
	m_hud(m_resourceMng),
	m_tutorials(m_resourceMng),
	m_state(state),
	m_player(m_resourceMng, t_input, t_controller, t_window, m_particlePool, m_options, m_world),
	m_input(t_input),
	m_gravity(0.0f, 20.0f),
	m_world(m_gravity),
	m_options(t_options),
	m_enemyHandler(m_world, t_resourceMng, t_options),
	m_resetting(false),
	m_minimap(m_resourceMng),
	m_window(t_window),
	m_controller(t_controller)
{
	m_exits.reserve(4);
	m_world.SetContactListener(&m_listener);

	initLevel(1);

	sf::Transform temp;
	temp.translate(0, -500);
	m_renderState.transform = temp;

	sf::Transform tempTrans;
	temp.translate(0, -1200);
	m_renderState.transform = temp;	

}

GameScreen::~GameScreen()
{
}

void GameScreen::processEvents(Xbox360Controller & t_gamepad)
{
}

void GameScreen::update(sf::Time t_deltaTime, EndScreen &t_endScreen, MainMenu &t_mainMenu)
{
	m_world.Step(timeStep, velocityIterations, positionIterations);
	m_enemyHandler.update(t_deltaTime.asSeconds(), m_player);

	for (int i = 0; i < m_exits.size(); i++)
	{
		if (m_exits.at(i)->hasExited())
		{
			if (m_exits.at(i)->getLevel() == 7)
			{
				initLevel(1);
				m_firstInit = true;
				m_nextState = Menu::StateEnd;
				m_changeMenu = true;
				t_endScreen.initEnd(EndState::WIN);
				t_endScreen.transitionIn();
			}
			else
			{
				initLevel(m_exits.at(i)->getLevel());
			}
		}
	}

#ifdef _DEBUG
	if (m_input.m_current.one)
	{
		m_firstInit = true;
		initLevel(1);
	}
	if (m_input.m_current.two)
	{
		m_firstInit = true;
		initLevel(2);
	}
	if (m_input.m_current.three)
	{
		m_firstInit = true;
		initLevel(3);
	}
	if (m_input.m_current.four)
	{
		m_firstInit = true;
		initLevel(4);
	}
	if (m_input.m_current.five)
	{
		m_firstInit = true;
		initLevel(5);
	}
	if (m_input.m_current.six)
	{
		m_firstInit = true;
		initLevel(6);
	}
#endif // _DEBUG

	if (!m_player.getAlive() && m_player.getLives() > 0)
	{
		m_player.loseLife();
		m_resetting = true;
		initLevel(m_currentLevel);
	}

	m_player.update(t_deltaTime);

	m_particlePool.update(t_deltaTime);

	contactListHandler();

	if (m_player.getLives() <= 0 && !m_changeMenu)
	{
		initLevel(1);
		m_firstInit = true;
		m_nextState = Menu::StateEnd;
		m_changeMenu = true;
		t_endScreen.initEnd(EndState::LOSE);
		t_endScreen.transitionIn();
	}

	for (int i = 0; i < m_crates.size(); i++)
	{
		m_crates[i]->update();
	}

	for (int i = 0; i < m_phaseWalls.size(); i++)
	{
		m_phaseWalls.at(i)->update(t_deltaTime.asSeconds());
	}

	sf::Vector2f temp = m_player.getPos() - sf::Vector2f(1152, 950);
	if (thor::length(temp) < 40 && m_currentLevel == 4 && !m_player.getHasOrb())
	{
		m_player.setHasOrb(true);
	}

	m_hud.update(m_player.getChargeScale(), m_player.getLives(), t_deltaTime.asSeconds());
	m_minimap.update(m_currentLevel, m_player.getHasOrb());
	m_tutorials.update(m_currentLevel, t_deltaTime.asSeconds());

	if (m_controller.m_currentState.Back && m_state == Menu::StateGameplay)
	{
		m_changeMenu = true;
		m_nextState = Menu::StateMainMenu;
		m_state = Menu::StateMainMenu;
		t_mainMenu.transitionIn();
	}
}

void GameScreen::render(sf::RenderWindow & t_window)
{
	m_enemyHandler.render(t_window);

	for (int i = 0; i < m_obstacles.size(); i++)
	{
		t_window.draw(m_obstacles[i]->getBody());
	}

	for (int i = 0; i < m_crates.size(); i++)
	{
		m_crates[i]->render(t_window);
	}

	for (int i = 0; i < m_phaseWalls.size(); i++)
	{
		m_phaseWalls.at(i)->render(t_window);
	}

#ifdef _DEBUG
	for (int i = 0; i < m_exits.size(); i++)
	{
		m_exits[i]->debugDraw(t_window);
	}
#endif // _DEBUG

	m_tutorials.render(t_window, m_player.getHasOrb());
	m_minimap.render(t_window);
	m_player.render(t_window);
	m_particlePool.render(t_window);

	m_hud.render(t_window, m_player.getHasOrb());
}

bool GameScreen::getChangeMenu()
{
	return m_changeMenu;
}

void GameScreen::resetChangeMenu()
{
	m_changeMenu = false;
}

void GameScreen::initLevel(int t_levelId)
{
	m_currentLevel = t_levelId;
	// load level data from a yaml file
	if (!LevelLoader::load(t_levelId, m_level))
	{
		return;
	}
	createWorld2D();
}

void GameScreen::restart()
{
	m_firstInit = true;
	initLevel(1);
	m_player.setLives(10);
	m_minimap.init();
}

void GameScreen::createWorld2D()
{
	if (m_firstInit)
	{
		m_minimap.init();
		m_player.setStartPos(m_level.m_player.m_position);
		m_player.setLives(10);
		m_hud.reset();
		m_player.setupPlayer(
			m_world,
			m_level.m_player.m_fixedRotation, _entityCategory::PLAYER
		);
		m_player.resetHasOrb();
		m_firstInit = false;
	}
	else if (m_resetting)
	{
		m_player.setupPlayer(
			m_world,
			m_level.m_player.m_fixedRotation, _entityCategory::PLAYER
		);
		m_resetting = false;
	}
	else
	{
		for (int i = 0; i < m_exits.size(); i++)
		{
			if (m_exits.at(i)->hasExited())
			{
				m_player.setStartPos(m_exits.at(i)->getPlayerSpawn());
				m_player.setupPlayer(
					m_world,  
					m_level.m_player.m_fixedRotation, _entityCategory::PLAYER
				);
			}
		}
	}
	m_player.setUserData();
	
	m_enemyHandler.initEnemies(m_level, m_resourceMng, m_options);

	for (int i = 0; i < m_exits.size(); i++)
	{
		m_exits.at(i)->destroy();
		delete m_exits.at(i);
	}
	m_exits.clear();

	for (int i = 0; i < m_obstacles.size(); i++)
	{
		m_obstacles.at(i)->destroy();
		delete m_obstacles.at(i);
	}
	m_obstacles.clear();

	for (int i = 0; i < m_phaseWalls.size(); i++)
	{
		m_phaseWalls.at(i)->destroy();
		delete m_phaseWalls.at(i);
	}
	m_phaseWalls.clear();

	for (int i = 0; i < m_crates.size(); i++)
	{
		m_crates.at(i)->destroy();
		delete m_crates.at(i);
	}
	m_crates.clear();

	// for loop to load at and add all obstacles to the box2d world
	for (int i = 0; i < m_level.m_obstacles.size(); i++)
	{
		Obstacle* obstacle = new Obstacle(
			m_resourceMng,
			m_world,
			m_level.m_obstacles[i].m_position,
			m_level.m_obstacles[i].m_size,
			_entityCategory::WORLD
		);

		m_obstacles.push_back(obstacle);
		m_obstacles[i]->setUserData();
	}

	// for loop to load at and add all obstacles to the box2d world
	for (int i = 0; i < m_level.m_crates.size(); i++)
	{
		Crate* crate = new Crate(
			m_resourceMng,
			m_world,
			m_level.m_crates[i].m_position,
			m_level.m_crates[i].m_size,
			_entityCategory::WORLD,
			_entityCategory::WORLD | _entityCategory::ORB | _entityCategory::PLAYER | _entityCategory::ENEMY
		);

		m_crates.push_back(crate);
		m_crates[i]->setUserData();
	}

	for (int i = 0; i < m_level.m_exits.size(); i++)
	{
		Exit* exit = new Exit(
			m_world,
			m_level.m_exits[i].m_position,
			m_level.m_exits[i].m_size,
			m_level.m_exits[i].m_toLevel,
			m_level.m_exits[i].m_playerSpawn,
			_entityCategory::EXIT
		);

		m_exits.push_back(exit);
		m_exits[i]->setUserData();
	}

	for (int i = 0; i < m_level.m_phaseWalls.size(); i++)
	{
		PhaseWall* phaseWall = new PhaseWall(
			m_resourceMng,
			m_world,
			m_level.m_phaseWalls[i].m_position,
			m_level.m_phaseWalls[i].m_size,
			b2Vec2(m_level.m_phaseWalls[i].m_direction.x, m_level.m_phaseWalls[i].m_direction.y),
			m_level.m_phaseWalls[i].m_duration
		);

		m_phaseWalls.push_back(phaseWall);
		m_phaseWalls[i]->setUserData();
	}
}

void GameScreen::contactListHandler()
{
	for (b2Contact* contact = m_world.GetContactList(); contact; contact = contact->GetNext())
	{
		UserData* dataA = static_cast<UserData*>(contact->GetFixtureA()->GetBody()->GetUserData());
		UserData* dataB = static_cast<UserData*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (contact->GetFixtureA()->IsSensor() && (dataB->entity == 1 || dataB->entity == 2))
		{
			Enemy* enemy = static_cast<Enemy*>(dataB->data);
			if (contact->GetFixtureA()->GetBody()->GetPosition().x > contact->GetFixtureB()->GetBody()->GetPosition().x)
			{
				enemy->takeDamage(false);
			}
			else if (contact->GetFixtureA()->GetBody()->GetPosition().x < contact->GetFixtureB()->GetBody()->GetPosition().x)
			{
				enemy->takeDamage(true);
			}
		}
		if (contact->GetFixtureB()->IsSensor() && (dataA->entity == 1 || dataA->entity == 2))
		{
			Enemy* enemy = static_cast<Enemy*>(dataA->data);
			if (contact->GetFixtureB()->GetBody()->GetPosition().x > contact->GetFixtureA()->GetBody()->GetPosition().x)
			{
				enemy->takeDamage(false);
			}
			else if (contact->GetFixtureB()->GetBody()->GetPosition().x < contact->GetFixtureA()->GetBody()->GetPosition().x)
			{
				enemy->takeDamage(true);
			}
		}
	}
}
