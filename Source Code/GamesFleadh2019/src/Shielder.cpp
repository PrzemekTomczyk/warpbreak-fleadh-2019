#include "Shielder.h"



Shielder::Shielder(ResourceManager & t_resourceMng, Options & t_options) :
	Enemy(t_resourceMng, t_options)
{
	//------------------Idle------------------------
	m_idleAnim.setTexture(m_resourceMng.getTexture(TextureID::SHIELDER_IDLE));
	setAnimRect(m_idleAnim, 29, 11, 128, 128, 0);
	m_idleAnim.setTime(Time(seconds(1.0f / 30.0f)));
	m_idleAnim.setOrigin(128 / 2, 128 / 2);
	m_idleAnim.setOffset(sf::Vector2f(-28, -10));
	m_idleAnim.setLooping(true);

	//------------------Walk------------------------
	m_walkAnim.setTexture(m_resourceMng.getTexture(TextureID::SHIELDER_WALK));
	setAnimRect(m_walkAnim, 34, 11, 128, 128, 0);
	m_walkAnim.setTime(Time(seconds(1.0f / 30.0f)));
	m_walkAnim.setOrigin(128 / 2, 128 / 2);
	m_walkAnim.setOffset(sf::Vector2f(-28, -10));
	m_walkAnim.setLooping(true);

	m_currentAnim = &m_idleAnim;
}


Shielder::~Shielder()
{
}

void Shielder::update(float t_deltaTime, b2Body* t_player, b2World &t_world)
{
	handleCurrentAnim();

	m_stunCD -= t_deltaTime;
	m_wallTimer -= t_deltaTime;

	if (m_stunCD <= 0.0f)
	{
		m_stunned = false;
	}	

	if (m_hitWall && m_wallTimer <= 0.0f)
	{
		m_hitWall = false;
		m_direction *= -1;
	}

	if (!m_stunned && !m_hitWall)
	{
		m_b2body->SetLinearVelocity(b2Vec2(m_direction * m_speed, 0.0f));

		if (handleRaycast(t_world) && !m_hitWall && !playerInFront(t_world))
		{
			m_hitWall = true;
			m_wallTimer = 2.f;
			m_b2body->SetLinearVelocity(b2Vec2(0.0f, m_b2body->GetLinearVelocity().y));
		}
	}
	playerInFront(t_world);
}

void Shielder::setUserData() {
	m_userData.data = this;
	m_userData.entity = 1;
	m_b2body->SetUserData(&m_userData);
}

bool Shielder::takeDamage(bool left)
{
	m_hitWall = false;
	m_wallTimer = 0.0f;
	if (m_health <= 0 || m_stunned)
	{
		return false;
	}

	if (left)
	{
		if (m_direction == -1)
		{
			m_b2body->ApplyForce(b2Vec2(1500, -0), m_b2body->GetPosition(), true);
		}
		else
		{
			m_b2body->ApplyForce(b2Vec2(2500.0f, -2500.0f), m_b2body->GetPosition(), true);
			m_health--;
			m_direction *= -1;
		}
	}
	else
	{
		if (m_direction == 1)
		{
			m_b2body->ApplyForce(b2Vec2(-1500, -0), m_b2body->GetPosition(), true);
		}
		else
		{
			m_b2body->ApplyForce(b2Vec2(-2500.0f, -2500.0f), m_b2body->GetPosition(), true);
			m_health--;
			m_direction *= -1;
		}
	}
	std::cout << "shielder's health " << m_health << std::endl;

	m_stunned = true;
	m_stunCD = 0.4f;
	m_b2body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	
	return true;
}

void Shielder::handleCurrentAnim()
{
	m_currentAnim->update();

	if (m_hitWall || m_stunned)
	{
		if (m_currentAnim != &m_idleAnim)
		{
			m_currentAnim->reset();
			m_currentAnim = &m_idleAnim;
		}
	}
	else
	{
		if (m_currentAnim != &m_walkAnim)
		{
			m_currentAnim->reset();
			m_currentAnim = &m_walkAnim;
		}
	}
}

bool Shielder::playerInFront(b2World & t_world)
{
	float rayLength = 100;
	bool playerSpotted = false;

	// start point of ray on either left or right side of the enemy depending which way he's facing
	m_rayP1.x = m_b2body->GetPosition().x + (m_direction * ((m_width / 2.0f) / SCALE));
	// start point on the y axis (half of charger's height)
	m_rayP1.y = m_b2body->GetPosition().y;
	m_rayP2 = m_rayP1 + (rayLength / SCALE) * b2Vec2(std::sinf(m_direction * FORWARD_DETECTION_RAD), std::cosf(m_direction * FORWARD_DETECTION_RAD));

	//set up input
	b2RayCastInput input;
	input.p1 = m_rayP1;
	input.p2 = m_rayP2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	//b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b2body = t_world.GetBodyList(); b2body; b2body = b2body->GetNext()) 
	{
		for (b2Fixture* b2fixture = b2body->GetFixtureList(); b2fixture; b2fixture = b2fixture->GetNext())
		{
			UserData* userData = static_cast<UserData*>(b2body->GetUserData());

			b2RayCastOutput output;
			if (!b2fixture->RayCast(&output, input, 0))
				continue;
			if (output.fraction < closestFraction) 
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

void Shielder::render(sf::RenderWindow & t_window)
{
	sf::Vector2f m_position;
	m_position.x = (m_b2body->GetPosition().x * SCALE);
	m_position.y = (m_b2body->GetPosition().y * SCALE);


	if (m_direction == -1)
	{
		int x = -m_currentAnim->getOffset().x + m_position.x;
		int y = m_currentAnim->getOffset().y + m_position.y;
		m_currentAnim->setScale(1, 1);
		m_currentAnim->setPosition(sf::Vector2f(x, y));
	}
	else
	{
		int x = m_currentAnim->getOffset().x + m_position.x;
		int y = m_currentAnim->getOffset().y + m_position.y;
		m_currentAnim->setScale(-1, 1);
		m_currentAnim->setPosition(sf::Vector2f(x, y));
	}
	m_currentAnim->setTextureRect(m_currentAnim->getFrame(m_currentAnim->getCurrentFrame()));
	t_window.draw(*m_currentAnim);

#ifdef _DEBUG
	drawDebug(t_window);

	sf::Vertex ray[] =
	{
		sf::Vertex(sf::Vector2f(m_rayP1.x * SCALE, m_rayP1.y * SCALE)),
		sf::Vertex(sf::Vector2f(m_intersectionPoint.x * SCALE, m_intersectionPoint.y * SCALE))
	};

	t_window.draw(ray, 2, sf::Lines);

#endif // _DEBUG
}