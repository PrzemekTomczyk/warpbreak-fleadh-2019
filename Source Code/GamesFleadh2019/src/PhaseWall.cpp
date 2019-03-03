#include "PhaseWall.h"

PhaseWall::PhaseWall(ResourceManager & t_resources, b2World & t_world, sf::Vector2f t_pos, sf::Vector2f t_size, b2Vec2 t_direction, float t_duration) :
	m_resourceMng(t_resources),
	m_world(t_world),
	m_size(t_size),
	m_direction(t_direction),
	m_duration(t_duration),
	m_timer(t_duration)
{
	m_startPos = b2Vec2(t_pos.x / SCALE, t_pos.y / SCALE);
	makeBody(_entityCategory::PHASE_WALL);
	initialize(t_pos, t_size);
}

void PhaseWall::update(float t_deltaTime)
{
	m_timer -= t_deltaTime;
	if (m_timer <= 0)
	{
		m_timer = m_duration;
		destroy();
		makeBody(_entityCategory::PHASE_WALL);
	}
	m_b2body->SetLinearVelocity(m_direction);
}

void PhaseWall::render(sf::RenderWindow & t_window)
{
	m_body.setPosition(m_b2body->GetPosition().x * SCALE, m_b2body->GetPosition().y * SCALE);
	t_window.draw(m_body);
}

void PhaseWall::initialize(sf::Vector2f t_pos, sf::Vector2f t_size)
{
	m_body.setSize(t_size);
	m_body.setFillColor(sf::Color(40, 40, 150, 100));
	m_body.setOutlineThickness(5);
	m_body.setOutlineColor(sf::Color(40, 40, 200, 150));
	m_body.setOrigin(t_size / 2.0f);
}

void PhaseWall::makeBody(uint16 categoryBits)
{
	b2BodyDef staticBodyDef;
	staticBodyDef.position.Set(m_startPos.x, m_startPos.y);
	staticBodyDef.type = b2_kinematicBody;

	b2Body* phaseBody = m_world.CreateBody(&staticBodyDef);
	m_userData.data = this;
	m_userData.entity = 10;

	phaseBody->SetUserData(&m_userData);

	b2PolygonShape staticBox;
	staticBox.SetAsBox((m_size.x / 2.0f) / SCALE, (m_size.y / 2.0f) / SCALE);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.0f;
	FixtureDef.friction = 0.9;
	FixtureDef.shape = &staticBox;
	FixtureDef.filter.categoryBits = categoryBits;
	phaseBody->CreateFixture(&FixtureDef);

	m_b2body = phaseBody;


	setB2Body(phaseBody);
}

void PhaseWall::setUserData()
{
	m_userData.data = this;
	m_userData.entity = 10;
	m_b2body->SetUserData(&m_userData);
}

void PhaseWall::destroy()
{
	if (m_b2body != nullptr)
	{
		m_world.DestroyBody(m_b2body);
		m_b2body = nullptr;
	}
}

void PhaseWall::setB2Body(b2Body * t_body)
{
	m_b2body = t_body;
}
