#include "Crate.h"



Crate::Crate(ResourceManager & t_resources, b2World &t_world, sf::Vector2f t_pos, sf::Vector2f t_size, uint16 t_categoryBits, uint16 t_maskBits) :
	m_resourceMng(t_resources),
	m_world(t_world)
{
	b2BodyDef dynamicBodyDef;
	dynamicBodyDef.position.Set(t_pos.x / SCALE, t_pos.y / SCALE);
	dynamicBodyDef.type = b2_dynamicBody;

	m_b2body = m_world.CreateBody(&dynamicBodyDef);
	m_userData.data = this;
	m_userData.entity = 10;

	m_b2body->SetUserData(&m_userData);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((t_size.x / 2.0f) / SCALE, (t_size.y / 2.0f) / SCALE);

	b2FixtureDef FixtureDef;
	FixtureDef.density = (t_size.x / 64.0f) * 5;
	FixtureDef.friction = 0.50f;
	FixtureDef.shape = &dynamicBox;
	FixtureDef.filter.categoryBits = t_categoryBits;
	FixtureDef.filter.maskBits = t_maskBits;
	m_b2body->CreateFixture(&FixtureDef);

	initialize(t_pos, t_size);
}


Crate::~Crate()
{
}

void Crate::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_body);
}

void Crate::update()
{
	m_position = sf::Vector2f(m_b2body->GetPosition().x * SCALE, m_b2body->GetPosition().y * SCALE);

	m_body.setPosition(m_position);
	float angle = ((180 / b2_pi) * m_b2body->GetAngle());
	m_body.setRotation(angle);
}

sf::RectangleShape Crate::getBody() const
{
	return m_body;
}

void Crate::setUserData()
{
	m_userData.data = this;
	m_userData.entity = 10;
	m_b2body->SetUserData(&m_userData);
}

void Crate::initialize(sf::Vector2f t_pos, sf::Vector2f t_size)
{
	m_position = t_pos;
	m_body.setSize(t_size);
	m_body.setFillColor(sf::Color(63, 38, 153));
	m_body.setOrigin(t_size / 2.0f);
	m_body.setPosition(m_position);
	m_b2body->SetFixedRotation(false);
}

void Crate::destroy()
{
	if (m_b2body != nullptr)
	{
		m_world.DestroyBody(m_b2body);
		m_b2body = nullptr;
	}
}
