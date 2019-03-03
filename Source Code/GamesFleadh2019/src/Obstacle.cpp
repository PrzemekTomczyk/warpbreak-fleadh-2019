#include "Obstacle.h"

Obstacle::Obstacle(ResourceManager & t_resources, b2World &t_world, sf::Vector2f t_pos, sf::Vector2f t_size, uint16 categoryBits, float t_friction) :
	m_resourceMng(t_resources),
	m_world(t_world)
{
	b2BodyDef staticBodyDef;
	staticBodyDef.position.Set(t_pos.x / SCALE, t_pos.y / SCALE);
	staticBodyDef.type = b2_kinematicBody;

	m_b2body = m_world.CreateBody(&staticBodyDef);
	m_userData.data = this;
	m_userData.entity = 10;

	m_b2body->SetUserData(&m_userData);

	b2PolygonShape staticBox;
	staticBox.SetAsBox((t_size.x / 2.0f) / SCALE, (t_size.y / 2.0f) / SCALE);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.0f;
	FixtureDef.friction = t_friction;
	FixtureDef.shape = &staticBox;
	FixtureDef.filter.categoryBits = categoryBits;
	m_b2body->CreateFixture(&FixtureDef);

	initialize(t_pos, t_size);
}

void Obstacle::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_body);
}


sf::RectangleShape Obstacle::getBody() const
{
	return m_body;
}


void Obstacle::initialize(sf::Vector2f t_pos, sf::Vector2f t_size)
{
	m_position = t_pos;
	m_body.setSize(t_size);
	m_body.setFillColor(sf::Color(0, 40, 100));
	m_body.setOrigin(t_size / 2.0f);
	m_body.setPosition(m_position);
}

void Obstacle::destroy()
{
	if (m_b2body != nullptr)
	{
		m_world.DestroyBody(m_b2body);
		m_b2body = nullptr;
	}
}

void Obstacle::setUserData()
{
	m_userData.data = this;
	m_userData.entity = 10;
	m_b2body->SetUserData(&m_userData);
}

b2Body * Obstacle::getB2Body()
{
	return m_b2body;
}
