#include "Enemy.h"



Enemy::Enemy(ResourceManager & t_resources, Options & t_options) :
	m_resourceMng(t_resources),
	m_options(t_options),
	m_health(3),
	m_state(EnemyState::Patrol)
{
}


Enemy::~Enemy()
{
}

void Enemy::init(sf::Vector2f t_pos, bool t_rotation, sf::Vector2f t_size, b2World &t_world, int t_entityType, int t_direction )
{
	std::cout << "Initializing enemy" << std::endl;
	m_stunned = false;
	m_stunCD = 0.0f;

	setupEnemy(
		t_world, 
		t_pos, 
		t_rotation, 
		t_size, 
		t_entityType, 
		t_direction, 
		_entityCategory::ENEMY,	
		_entityCategory::ENEMY | 
		_entityCategory::ORB | 
		_entityCategory::PHASE_WALL | 
		_entityCategory::PLAYER | 
		_entityCategory::WORLD
	);
}

void Enemy::clear()
{
	m_alive = false;
	m_health = 0;
	if (m_b2body != nullptr) {
		m_b2body->GetWorld()->DestroyBody(m_b2body);
		m_b2body = nullptr;
	}
}

void Enemy::render(sf::RenderWindow & t_window)
{
	drawDebug(t_window);
}

void Enemy::setupEnemy(b2World &t_world, sf::Vector2f t_pos, bool t_fixedRotaion, sf::Vector2f t_size, int t_entityType, int t_direction, uint16 categoryBits, uint16 maskBits)
{
	m_direction = t_direction;

	m_alive = true;
	// add enemy to world
	b2BodyDef enemyDef;
	enemyDef.position.Set(t_pos.x / SCALE, t_pos.y / SCALE);
	enemyDef.type = b2_dynamicBody;
	enemyDef.fixedRotation = t_fixedRotaion;

	b2Body* enemyBody = t_world.CreateBody(&enemyDef);

	m_userData.data = this;
	m_userData.entity = t_entityType;

	enemyBody->SetUserData(&m_userData);

	//set the body shape
	b2PolygonShape enemyBox;
	enemyBox.SetAsBox(t_size.x / SCALE, t_size.y / SCALE, b2Vec2(0, 0 / SCALE), float32(0));

	b2FixtureDef enemyFixtureDef;
	enemyFixtureDef.density = 2.0f;
	enemyFixtureDef.friction = 0.10f;
	enemyFixtureDef.shape = &enemyBox;
	enemyFixtureDef.filter.categoryBits = categoryBits;
	enemyFixtureDef.filter.maskBits = maskBits;
	enemyBody->CreateFixture(&enemyFixtureDef);

	setB2Body(enemyBody);
	m_width = t_size.x;
	m_height = t_size.y;
	m_b2body->SetGravityScale(10.5f);
	m_b2body->SetUserData(&m_userData);
}

void Enemy::setB2Body(b2Body * t_body)
{
	m_b2body = t_body;
}

bool Enemy::getAlive() const
{
	return m_alive;
}

void Enemy::kill()
{
	m_health = 0;
}

int Enemy::getHealth()
{
	return m_health;
}

bool Enemy::handleRaycast(b2World &t_world)
{
	float rayLength = m_height * 2;
	bool hitPlayer = false;

	m_rayAngle = m_direction * thor::toRadian(WALKING_STICK_ANGLE);

	// start point of ray on either left or right side of the enemy depending which way he's facing
	m_rayP1.x = m_b2body->GetPosition().x + (m_direction * (m_width / SCALE));
	// start point on the y axis (half of enemy's height)
	m_rayP1.y = m_b2body->GetPosition().y;
	m_rayP2 = m_rayP1 + (rayLength / SCALE) * b2Vec2(std::sinf(m_rayAngle), std::cosf(m_rayAngle));

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
			if (output.fraction < closestFraction && (userData->entity != 0 && userData->entity != 11 && userData->entity != 4)) {
				closestFraction = output.fraction;
			}
		}
	}

	m_intersectionPoint = m_rayP1 + closestFraction * (m_rayP2 - m_rayP1);

	float distanceOfRay = std::sqrt(std::pow(m_intersectionPoint.x - m_rayP1.x, 2) + std::pow(m_intersectionPoint.y - m_rayP1.y, 2));

	// expected distance to the ground
	float expectedDistance = (rayLength / 2.0f) / std::cos(std::abs(m_rayAngle));
	distanceOfRay *= SCALE;

	if (std::abs(distanceOfRay - expectedDistance) > 4.0f && 
		m_wallTimer <= 0.0f)
	{
		return true;
	}
	return false;
}

void Enemy::drawDebug(sf::RenderWindow &t_window)
{
	for (b2Fixture* f = m_b2body->GetFixtureList(); f; f = f->GetNext())
	{
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

		sf::ConvexShape convex;
		convex.setOutlineColor(sf::Color::Green);
		convex.setOutlineThickness(1.0f);
		convex.setFillColor(sf::Color::Transparent);
		convex.setPosition(sf::Vector2f(m_b2body->GetPosition().x * SCALE, m_b2body->GetPosition().y * SCALE));
		convex.setPointCount(poly->m_count);

		for (int i = 0; i < poly->m_count; i++)
		{
			convex.setPoint(i, sf::Vector2f(poly->m_vertices[i].x * SCALE, poly->m_vertices[i].y * SCALE));
		}
		t_window.draw(convex);
	}

	sf::Vertex ray[] =
	{
		sf::Vertex(sf::Vector2f(m_rayP1.x * SCALE, m_rayP1.y * SCALE)),
		sf::Vertex(sf::Vector2f(m_intersectionPoint.x * SCALE, m_intersectionPoint.y * SCALE))
	};

	t_window.draw(ray, 2, sf::Lines);
}

bool Enemy::takeDamage(bool left)
{
	m_health--;

	if (m_health <= 0)
	{
		return true;
	}
	return false;
}

void Enemy::setAnimRect(AnimatedSprite & anim, int frames, int framesPerLine, int width, int height, int pos)
{
	int count = 0;
	int line = 0;
	while (count < frames - 1)
	{
		for (int i = 0; i < framesPerLine; i++)
		{
			IntRect rec(0 + width * i, pos + height * line, width, height);
			anim.addFrame(rec);
			count++;
			if (count >= frames - 1)
			{
				break;
			}
		}
		line++;
	}
}
