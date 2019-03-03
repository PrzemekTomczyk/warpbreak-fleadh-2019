#include "Charger.h"

Charger::Charger(ResourceManager & t_resourceMng, Options & t_options) :
	Enemy(t_resourceMng, t_options)
{
	m_health = 1;
	m_alertSound.setBuffer(m_resourceMng.getBuffer(SoundBufferID::ALERT));
	m_alertSound.setVolume(m_options.soundVol);

	//------------------Idle------------------------
	m_idleAnim.setTexture(m_resourceMng.getTexture(TextureID::CHARGER_IDLE));
	setAnimRect(m_idleAnim, 29, 11, 128, 128, 0);
	m_idleAnim.setTime(Time(seconds(1.0f / 30.0f)));
	m_idleAnim.setOrigin(128 / 2, 128 / 2);
	m_idleAnim.setOffset(sf::Vector2f(-20, -10));
	m_idleAnim.setLooping(true);

	//------------------Walk------------------------
	m_walkAnim.setTexture(m_resourceMng.getTexture(TextureID::CHARGER_WALK));
	setAnimRect(m_walkAnim, 34, 11, 128, 128, 0);
	m_walkAnim.setTime(Time(seconds(1.0f / 30.0f)));
	m_walkAnim.setOrigin(128 / 2, 128 / 2);
	m_walkAnim.setOffset(sf::Vector2f(-20, -10));
	m_walkAnim.setLooping(true);

	//------------------Run------------------------
	m_runAnim.setTexture(m_resourceMng.getTexture(TextureID::CHARGER_RUN));
	setAnimRect(m_runAnim, 24, 11, 128, 128, 0);
	m_runAnim.setTime(Time(seconds(1.0f / 60.0f)));
	m_runAnim.setOrigin(128 / 2, 128 / 2);
	m_runAnim.setOffset(sf::Vector2f(-20, -10));
	m_runAnim.setLooping(true);


	m_currentAnim = &m_walkAnim;
}

Charger::~Charger()
{
}

void Charger::update(float t_deltaTime, b2Body * t_player, b2World & t_world)
{
	handleCurrentAnim();

	m_wallTimer -= t_deltaTime;
	m_stunCD -= t_deltaTime;

	if (m_stunCD <= 0.0f)
	{
		m_stunned = false;
	}

	if (m_hitWall && m_wallTimer <= 0.0f && m_state == EnemyState::Patrol)
	{
		m_hitWall = false;
		m_direction *= -1;
	}
	else if (m_state == EnemyState::Charging) {
		m_hitWall = false;

		if (m_chargeDelay.isExpired())
		{
			m_speed = BOOST_SPEED;
		}
	}

	if (!m_hitWall && !m_stunned)
	{
		m_b2body->SetLinearVelocity(b2Vec2(m_direction * m_speed, m_b2body->GetLinearVelocity().y));

		if (handleRaycast(t_world) && m_state == EnemyState::Patrol)
		{
			m_hitWall = true;
			m_wallTimer = 2.0f;
			m_b2body->SetLinearVelocity(b2Vec2(0.0f, m_b2body->GetLinearVelocity().y));
		}
	}	

	handleCharge(t_world);
}

void Charger::handleCharge(b2World &t_world)
{
	float rayLength = SCREEN_SIZE.x * 0.25;
	bool pushingShielder = false;

	// start point of ray on either left or right side of the enemy depending which way he's facing
	m_rayP1.x = m_b2body->GetPosition().x + (m_direction * (m_width / SCALE));
	// start point on the y axis (half of charger's height)
	m_rayP1.y = m_b2body->GetPosition().y;
	m_rayP2 = m_rayP1 + (rayLength / SCALE) * b2Vec2(std::sinf(m_direction * CHARGE_ANGLE_RAD), std::cosf(m_direction * CHARGE_ANGLE_RAD));

	//set up input
	b2RayCastInput input;
	input.p1 = m_rayP1;
	input.p2 = m_rayP2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	//b2Vec2 intersectionNormal(0, 0);
	for (b2Body* b2body = t_world.GetBodyList(); b2body; b2body = b2body->GetNext()) {
		for (b2Fixture* b2fixture = b2body->GetFixtureList(); b2fixture; b2fixture = b2fixture->GetNext()) {
			UserData* userData = static_cast<UserData*>(b2body->GetUserData());

			b2RayCastOutput output;
			if (!b2fixture->RayCast(&output, input, 0))
				continue;

			//if ray hit player
			if (output.fraction < closestFraction && (userData->entity == 0)) {				
				if (m_state != EnemyState::Charging)
				{
					m_b2body->ApplyForce(b2Vec2(0, -1500.0f), m_b2body->GetPosition(), true);
					m_chargeDelay.restart(sf::seconds(0.15));
					if (m_playAlert && m_alertSound.getStatus() == m_alertSound.Stopped) {
						m_alertSound.stop();
					}
					m_alertSound.play();
				}
				m_state = EnemyState::Charging;
				m_wallTimer = -1.0f;
			}
			if (output.fraction < closestFraction && 
				(userData->entity != 0 && // not shielder
				userData->entity != 11 && 
				userData->entity != m_userData.entity)) 
			{
				if (userData->entity == 1)
				{
					// set hitting pushing shielder
					pushingShielder = true;
				}
				else
				{
					closestFraction = output.fraction;
				}
			}
		}
	}

	m_intersectionPoint = m_rayP1 + closestFraction * (m_rayP2 - m_rayP1);

	float distanceOfRay = std::sqrt(std::pow(m_intersectionPoint.x - m_rayP1.x, 2) + std::pow(m_intersectionPoint.y - m_rayP1.y, 2));

	// expected distance to the ground
	distanceOfRay *= SCALE;

	bool slammedWall = false;
	// if pushing a shielder
	if (!pushingShielder)
	{
		// if no then check if charged into the wall
		if (distanceOfRay <= 1.0f && m_wallTimer <= 0.0f)
		{
			slammedWall = true;		
		}
	}
	// else push shielder into the wall
	else
	{
		//check if hit wall with shielder
		if ((distanceOfRay <= (rayLength * 0.01f + (32.0f * 2.0f))) && m_wallTimer <= 0.0f)
		{
			slammedWall = true;
		}
	}

	if (slammedWall)
	{
		m_playAlert = true;
		m_hitWall = true;
		m_wallTimer = 3.0f;
		m_state = EnemyState::Patrol;
		m_speed = PATROL_SPEED;

		m_stunned = true;
		m_stunCD = 0.4f;

		m_b2body->SetLinearVelocity(b2Vec2(0.0f, m_b2body->GetLinearVelocity().y));
		m_b2body->ApplyForce(b2Vec2(m_direction * -2500.0f, -2500.0f), m_b2body->GetPosition(), true);
	}
}

void Charger::setUserData()
{
	m_userData.data = this;
	m_userData.entity = 2;
	m_b2body->SetUserData(&m_userData);
}

bool Charger::takeDamage(bool left)
{
	m_health--;	
	std::cout << "Charger's health " << m_health << std::endl;

	if (m_health <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Charger::handleCurrentAnim()
{
	m_currentAnim->update();

	if (m_state == EnemyState::Charging)
	{
		if (m_currentAnim != &m_runAnim)
		{
			m_currentAnim->reset();
			m_currentAnim = &m_runAnim;
		}
	}
	else if (m_state == EnemyState::Patrol && (m_hitWall || m_stunned))
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

void Charger::render(sf::RenderWindow & t_window)
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
