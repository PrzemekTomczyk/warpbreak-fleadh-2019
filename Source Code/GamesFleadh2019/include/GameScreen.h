#pragma once

#include "Frame.h"
#include <SFML/Graphics.hpp>
#include "VLayout.h"
#include "TextButton.h"
#include "button.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "Slider.h"
#include "XBOX360CONTROLLER.h"
#include "States.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Player.h"
#include "EnemyHandler.h"
#include "Utility.h"
#include "Obstacle.h"
#include "Crate.h"
#include "MyListener.h"
#include "LevelLoader.h"
#include "Exit.h"
#include "EndScreen.h"
#include "Hud.h"
#include "PhaseWall.h"
#include "Tutorials.h"
#include "Minimap.h"
#include "MainMenu.h"

class GameScreen
{
public:
	/// <summary>
	/// default constructor
	/// </summary>
	/// <param name="t_window">reference to the main game window</param>
	/// <param name="state">state of the game</param>
	/// <param name="t_options">reference to the game options</param>
	/// <param name="t_resourceMng">reference to the resource manager</param>
	/// <param name="t_input">reference to the input handler</param>
	/// <param name="t_controller">reference to the controller</param>
	GameScreen(sf::RenderWindow & t_window, Menu & state, Options & t_options, ResourceManager & t_resourceMng, Input & t_input, Xbox360Controller & t_controller);
	~GameScreen();

	/// <summary>
	/// processes game events
	/// </summary>
	/// <param name="pad"> reference to the game controller</param>
	/// <param name="t_event">an event passed in from game.cpp</param>
	void processEvents(Xbox360Controller & pad);

	/// <summary>
	/// updates all objects in this class
	/// </summary>
	/// <param name="t_deltaTime">time since last update</param>
	void update(sf::Time t_deltaTime, EndScreen &t_endScreen, MainMenu &t_mainMenu);

	/// <summary>
	/// function that renders all drawable objects
	/// </summary>
	/// <param name="t_window">reference to the main window</param>
	void render(sf::RenderWindow & t_window);

	bool getChangeMenu();
	/// <summary>
	/// resets the m_changeMenu variable
	/// </summary>
	void resetChangeMenu();

	void initLevel(int t_levelId);

	void restart();

private:

	const int MAX_OBSTACLES = 10; // TO-DO: Make obstacle class for terrain
	int m_aliveObstacles = 5;
	void createWorld2D();

	void contactListHandler();

	ResourceManager & m_resourceMng;

	Options & m_options;

	ParticlePool m_particlePool;
	
	Hud m_hud;
	Tutorials m_tutorials;
	Player m_player;
	int m_currentLevel;

	std::vector<Obstacle*> m_obstacles;
	std::vector<Crate*> m_crates;
	std::vector<Exit*> m_exits;
	std::vector<PhaseWall*> m_phaseWalls;

	Menu & m_state; //reference to the state holding the current menu
	Menu m_nextState;

	sf::RenderStates m_renderState;

	bool m_changeMenu = false;

	bool m_firstInit = true;
	bool m_resetting = true;

	// box2D variables
	b2Vec2 m_gravity;
	b2World m_world;
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;
	MyListener m_listener;

	LevelData m_level;
	Input & m_input;

	EnemyHandler m_enemyHandler;

	Minimap m_minimap;

	sf::RenderWindow &m_window;

	Xbox360Controller &m_controller;
};

