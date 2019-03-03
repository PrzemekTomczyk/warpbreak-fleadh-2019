#include "Exit.h"



Exit::Exit(b2World &t_world, sf::Vector2f t_pos, sf::Vector2f t_size, int t_levelNo, sf::Vector2f t_playerSpawn, uint16 categoryBits) :
	m_world(t_world),
	m_toLevel(t_levelNo),
	m_playerSpawn(t_playerSpawn)
{
	b2BodyDef staticBodyDef;
	staticBodyDef.position.Set(t_pos.x / SCALE, t_pos.y / SCALE);
	staticBodyDef.type = b2_staticBody;

	m_b2body = m_world.CreateBody(&staticBodyDef);
	m_userData.data = this;
	m_userData.entity = 12;

	m_b2body->SetUserData(&m_userData);

	b2PolygonShape staticBox;
	staticBox.SetAsBox((t_size.x / 2.0f) / SCALE, (t_size.y / 2.0f) / SCALE);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.0f;
	FixtureDef.friction = 0.0f;
	FixtureDef.shape = &staticBox;
	FixtureDef.filter.categoryBits = categoryBits;
	m_b2body->CreateFixture(&FixtureDef);

}


Exit::~Exit()
{
}

int Exit::getLevel() {
	return m_toLevel;
}

sf::Vector2f Exit::getPlayerSpawn() const
{
	return m_playerSpawn;
}

void Exit::destroy()
{
	if (m_b2body != nullptr)
	{
		m_world.DestroyBody(m_b2body);
		m_b2body = nullptr;
	}
}

void Exit::hit()
{
	m_exit = true;
}

bool Exit::hasExited()
{
	return m_exit;
}

void Exit::debugDraw(sf::RenderWindow & t_window)
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
}

void Exit::setUserData() {
	m_userData.data = this;
	m_userData.entity = 12;
	m_b2body->SetUserData(&m_userData);
}
