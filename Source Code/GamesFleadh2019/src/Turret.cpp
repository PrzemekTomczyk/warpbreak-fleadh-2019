#include "Turret.h"
#include "Thor/Vectors.hpp"

//TO-DO:
//	make box2d bullet class for turrets
//	make turret shoot every X time when tracking player
//		- go back to patrol after shooting?

//TO-FIX:
//	orb breaks tracking and makes turret track through world??????

Turret::Turret(ResourceManager & t_resourceMng, Options & t_options, b2World & t_world) :
	m_resourceMng(t_resourceMng),
	m_options(t_options),
	m_state(EnemyState::Patrol),
	m_radius(32.0f),
	m_world(t_world),
	m_bullet(t_resourceMng, t_options)
{
}

Turret::~Turret()
{
}

void Turret::update(float t_deltaTime, Player &t_player)
{
	if (m_bullet.getAlive())
	{
		m_bullet.update(t_deltaTime);
	}
	else if(m_bullet.toDestroy())
	{
		m_bullet.destroy(m_world);
	}

	sf::Vector2f playerPos;
	playerPos.x = t_player.getB2Body()->GetPosition().x;
	playerPos.y = t_player.getB2Body()->GetPosition().y;

	switch (m_state)
	{
	case EnemyState::Patrol:
		handlePatrol();
		break;
	case EnemyState::Player_Spotted:
		//track player here
		handleTracking(playerPos);
		break;
	case EnemyState::Attack:
	{
		m_state = EnemyState::Player_Spotted;
		// fire bullet and reset to player spotted 
		// until can shoot again
		sf::Vector2f headingVec;
		headingVec = m_pos - (playerPos * SCALE);
		thor::unitVector(headingVec);

		if (!m_bullet.getAlive())
		{
			m_bullet.init(m_base.getPosition(), headingVec, m_world);
		}
		break;
	}
	default:
		break;
	}
	m_turret.setRotation(m_angle);
}

void Turret::render(sf::RenderWindow & t_window)
{
	drawDebug(t_window);
	t_window.draw(m_base);
	m_bullet.render(t_window);
	t_window.draw(m_turret);
}

void Turret::init(sf::Vector2f t_pos, int t_entityType, sf::Vector2f t_angles, int t_direction)
{
	m_pos = t_pos;

	m_patrolAngles = t_angles;

	m_base.setRadius(m_radius);
	m_base.setOrigin(sf::Vector2f(m_radius, m_radius));
	m_base.setPosition(m_pos);
	m_base.setFillColor(sf::Color(100, 100, 100));

	m_turret.setSize(sf::Vector2f(m_radius * 2.0f, m_radius / 2.0f));
	m_turret.setOrigin(sf::Vector2f(m_radius / 2.0f, m_turret.getSize().y / 2.0f));

	m_turret.setPosition(m_pos);
	m_turret.setFillColor(sf::Color(212, 214, 216));

	m_angle = ((t_angles.x + (t_angles.y - t_angles.x) / 2.0f));
	m_turret.setRotation(m_angle);

	m_entityId = t_entityType;
}

void Turret::clear()
{
	m_bullet.destroy(m_world);
}

void Turret::handlePatrol()
{
	if (m_angle >= m_patrolAngles.y ||
		m_angle <= m_patrolAngles.x)
	{
		m_angularVelo *= -1;

		if (m_angle <= m_patrolAngles.x)
		{
			m_angularVelo = 1;
			//m_turret.setRotation(m_patrolAngles.x);
		}
		else if(m_angle >= m_patrolAngles.y)
		{
			m_angularVelo = -1;
			//m_turret.setRotation(m_patrolAngles.y);
		}
	}

	if (handleRaycast())
	{
		m_state = EnemyState::Player_Spotted;
		m_bullet.setTimer();
	}

	m_angle += m_angularVelo;
}

void Turret::handleTracking(sf::Vector2f t_playerPos)
{
	if (handleRaycast())
	{
		sf::Vector2f headingVec;
		headingVec = m_pos - (t_playerPos * SCALE);
		thor::unitVector(headingVec);

		//calculate angle to the player in radians
		float turretRotation = std::atan2(-headingVec.y, -headingVec.x);
		//convert to degrees
		turretRotation = thor::toDegree(turretRotation);

		//set the rotation
		m_angle = turretRotation;
		m_state = EnemyState::Attack;
	}
	else
	{
		m_state = EnemyState::Patrol;
	}
}

void Turret::drawDebug(sf::RenderWindow &t_window)
{
	sf::Color rayColour(255, 0, 0);
	//if (m_state == EnemyState::Player_Spotted || m_state == EnemyState::Attack)
	//{
	//	rayColour = sf::Color(255, 0, 0);
	//}
	sf::Vertex ray[] =
	{
		sf::Vertex(sf::Vector2f(m_rayP1.x * SCALE, m_rayP1.y * SCALE), rayColour),
		sf::Vertex(sf::Vector2f(m_intersectionPoint.x * SCALE, m_intersectionPoint.y * SCALE), rayColour)
	};

	t_window.draw(ray, 2, sf::Lines);
}

bool Turret::handleRaycast()
{
	bool playerSpotted = false;

	float rayLength = SCREEN_SIZE.x;

	// start point of ray on either left or right side of the enemy depending which way he's facing
	m_rayP1 = b2Vec2(m_turret.getPosition().x / SCALE, m_turret.getPosition().y / SCALE);

	b2Vec2 turretAngle(-std::sinf(thor::toRadian(m_turret.getRotation() - 90)), std::cosf(thor::toRadian(m_turret.getRotation() - 90)));

	m_rayP2 = m_rayP1 + (rayLength / SCALE) * turretAngle;

	//set up input
	b2RayCastInput input;
	input.p1 = m_rayP1;
	input.p2 = m_rayP2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	//b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b2body = m_world.GetBodyList(); b2body; b2body = b2body->GetNext()) {
		for (b2Fixture* b2fixture = b2body->GetFixtureList(); b2fixture; b2fixture = b2fixture->GetNext()) {
			UserData* userData = static_cast<UserData*>(b2body->GetUserData());

			b2RayCastOutput output;
			if (!b2fixture->RayCast(&output, input, 0) || userData->entity == 11)
				continue;
			if (output.fraction < closestFraction && 
				(userData->entity != 12 && //not door
					userData->entity != 11 && //not orb
					userData->entity != 3 && //not bullet
					userData->entity != 1 && // not shielder
					userData->entity != 2)) // not charger
			{ 
				if (userData->entity == 0)
				{
					playerSpotted = true;
				}
				else if (playerSpotted == true)
				{
					playerSpotted = false;
				}

				closestFraction = output.fraction;
			}
		}
	}
	m_intersectionPoint = m_rayP1 + closestFraction * (m_rayP2 - m_rayP1);
	return playerSpotted;
}
