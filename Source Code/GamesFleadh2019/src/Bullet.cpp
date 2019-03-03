#include "Bullet.h"
#include "Thor/Vectors.hpp"

Bullet::Bullet(ResourceManager & t_resourceMng, Options & t_options) :
	m_resourceMng(t_resourceMng),
	m_options(t_options)
{
	m_sprite.setTexture(m_resourceMng.getTexture(TextureID::BULLET));
	m_sprite.setOrigin(
		sf::Vector2f(
			m_sprite.getTexture()->getSize().x / 2.0f,
			m_sprite.getTexture()->getSize().y / 2.0f
		)
	);

	m_sprite.setScale(sf::Vector2f(0.05f, 0.05f));

	m_alive = false;
	m_toBeDestroyed = false;
}

Bullet::~Bullet()
{
}

void Bullet::update(float t_deltaTime)
{
	if (m_alive)
	{
		m_sprite.setPosition(m_b2body->GetPosition().x * SCALE, m_b2body->GetPosition().y * SCALE);

		if (m_sprite.getPosition().x < 0.0f - m_sprite.getGlobalBounds().width || 
			m_sprite.getPosition().x > SCREEN_SIZE.x + m_sprite.getGlobalBounds().width ||
			m_sprite.getPosition().y < 0.0f - m_sprite.getGlobalBounds().height ||
			m_sprite.getPosition().y > SCREEN_SIZE.y + m_sprite.getGlobalBounds().height)
		{
			kill();
		}
	}
}

void Bullet::render(sf::RenderWindow & t_window)
{
	if (m_alive)
	{
		t_window.draw(m_sprite);
	}
}

void Bullet::init(sf::Vector2f t_position, sf::Vector2f t_heading, b2World &t_world)
{
	if (!m_alive && m_aliveTimer.isExpired())
	{
		//calculate angle to the player in radians
		float bulletAngle = std::atan2(-t_heading.y, -t_heading.x);
		//convert to degrees
		bulletAngle = thor::toDegree(bulletAngle);

		b2BodyDef bulletDef;
		bulletDef.position.Set(t_position.x / SCALE, t_position.y / SCALE);
		bulletDef.type = b2_dynamicBody;
		bulletDef.fixedRotation = false;
		bulletDef.gravityScale = 0.0f;

		b2Body* bulletBody = t_world.CreateBody(&bulletDef);

		m_userData.data = this;
		m_userData.entity = 3;
		bulletBody->SetUserData(&m_userData);

		//set the body shape
		b2PolygonShape bulletBox;
		float width = m_sprite.getGlobalBounds().width / 2.0f;
		float height = m_sprite.getGlobalBounds().height / 2.0f;
		bulletBox.SetAsBox(width / SCALE, height / SCALE, b2Vec2(0, 0 / SCALE), float32(0));

		b2FixtureDef bulletFixtureDef;
		bulletFixtureDef.density = 2.0f;
		bulletFixtureDef.friction = 0.0f;
		bulletFixtureDef.shape = &bulletBox;
		bulletFixtureDef.filter.categoryBits = _entityCategory::WORLD;
		bulletFixtureDef.filter.maskBits = _entityCategory::WORLD | _entityCategory::PLAYER;
		bulletBody->CreateFixture(&bulletFixtureDef);

		m_b2body = bulletBody;

		sf::Vector2f velocity = (thor::unitVector(t_heading) * 1200.0f);

		m_b2body->SetLinearVelocity(b2Vec2(-velocity.x / SCALE, -velocity.y / SCALE));
		m_b2body->SetTransform(m_b2body->GetPosition(), thor::toRadian(bulletAngle));

		m_sprite.setPosition(t_position);
		m_sprite.setRotation(bulletAngle);
		setB2Body(bulletBody);
		m_alive = true;
		m_toBeDestroyed = false;

		m_aliveTimer.restart(sf::seconds(2.0f));
	}
}

void Bullet::destroy(b2World &t_world)
{
	if (m_b2body != nullptr)
	{
		t_world.DestroyBody(m_b2body);
		m_b2body = nullptr;
		m_alive = false;
	}
}

void Bullet::kill()
{
	if (m_alive)
	{
		m_toBeDestroyed = true;
		m_alive = false;
	}
}

bool Bullet::getAlive() const
{
	return m_alive;
}

bool Bullet::toDestroy() const
{
	return m_toBeDestroyed;
}

void Bullet::setUserData()
{
	m_userData.data = this;
	m_userData.entity = 3;
	m_b2body->SetUserData(&m_userData);
}

void Bullet::setTimer()
{
	m_aliveTimer.restart(sf::seconds(.5f));
}

void Bullet::setB2Body(b2Body * t_body)
{
	m_b2body = t_body;
}

b2Body * Bullet::getB2Body() const
{
	return m_b2body;
}
