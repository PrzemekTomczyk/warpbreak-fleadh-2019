#include "Orb.h"

Orb::Orb(ResourceManager & t_resources):
	m_resourceMng(t_resources),
	m_radius(16)
{
	m_shape.setRadius(m_radius); //probs to big
	m_shape.setFillColor(sf::Color::Magenta);
	m_shape.setOrigin(m_radius, m_radius);

	m_sprite.setTexture(t_resources.getTexture(TextureID::ORB));
	m_sprite.setTextureRect(sf::IntRect(0, 0, 113, 113));
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.0f, m_sprite.getTextureRect().height / 2.0f);
}


Orb::~Orb()
{
}

void Orb::render(sf::RenderWindow & t_window)
{
	if (m_alive)
	{
		float rectWidth = m_sprite.getTextureRect().width;
		float orbScale = ((m_radius * 2.0f) / rectWidth);
		m_sprite.setScale(sf::Vector2f(orbScale, orbScale));

		sf::Vector2f position = sf::Vector2f(m_b2body->GetPosition().x * SCALE, m_b2body->GetPosition().y * SCALE);
		m_sprite.setPosition(position);

		m_shape.setPosition(position);
		//t_window.draw(m_shape);
		t_window.draw(m_sprite);
	}

	if (m_colSounds.size() > 0)
	{
		for (int i = 0; i < m_colSounds.size(); i++)
		{
			if (m_colSounds[i] != nullptr)
			{
				if (m_colSounds[i]->getStatus() == sf::Sound::Stopped) {
					delete m_colSounds[i];
					m_colSounds[i] = nullptr;
					m_colSounds.erase(m_colSounds.begin() + i);
				}
			}
		}
	}
}

void Orb::createOrb(b2World & t_world, sf::Vector2f t_pos, bool t_left, bool t_drop, uint16 categoryBits, uint16 maskBits)
{
	m_alive = true;

	// add player to world
	b2BodyDef bodyDef;
	bodyDef.position.Set(t_pos.x / SCALE, t_pos.y / SCALE);
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = false;

	m_b2body = t_world.CreateBody(&bodyDef);

	m_userData.data = this;
	m_userData.entity = 11;

	m_b2body->SetUserData(&m_userData);

	//-------------body shape---------------------
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = m_radius / SCALE;

	b2FixtureDef orbFixtureDef;
	orbFixtureDef.density = 2.0f;
	orbFixtureDef.friction = 0.01f;
	orbFixtureDef.restitution = 0.45f;
	orbFixtureDef.shape = &circleShape;
	orbFixtureDef.filter.categoryBits = categoryBits;
	orbFixtureDef.filter.maskBits = maskBits;
	m_b2body->CreateFixture(&orbFixtureDef);

	if (t_drop)
	{
		m_b2body->ApplyForce(b2Vec2(0, 0), m_b2body->GetPosition(), true);
	}
	else if (t_left)
	{
		m_b2body->ApplyForce(b2Vec2(-200, -200), m_b2body->GetPosition(), true);
	}
	else
	{
		m_b2body->ApplyForce(b2Vec2(200, -200), m_b2body->GetPosition(), true);
	}
}

void Orb::destroyOrb()
{
	if (m_b2body != nullptr && m_alive)
	{
		m_alive = false;
		m_b2body->GetWorld()->DestroyBody(m_b2body);
	}
}

void Orb::setFrame(float t_deltaTime)
{
	if (m_alive)
	{
		m_frameTime -= t_deltaTime;
		if (m_frameTime <= 0.0f)
		{
			m_frameTime = FRAME_DURATION;
			m_currentFrame++;
			if (m_currentFrame > 12)
			{
				m_currentFrame = 0;
			}
			m_sprite.setTextureRect(sf::IntRect(113 * m_currentFrame, 0, 113, 113));
		}
	}
}

void Orb::setSpriteAngle()
{
	float angle = ((180 / b2_pi) * m_b2body->GetAngle());
	m_sprite.setRotation(angle);
}

void Orb::playCollision()
{
	bool needsPushing = true;
	sf::Sound* sound = new sf::Sound;
	sound->setBuffer(m_resourceMng.getBuffer(SoundBufferID::ORB_BOUNCE));
	sound->play();

	for (int i = 0; i < m_colSounds.size(); i++)
	{
		if (m_colSounds[i] == nullptr)
		{
			m_colSounds[i] = sound;
			needsPushing = false;
		}
	}

	if (needsPushing)
	{
		m_colSounds.push_back(sound);
	}
}

b2Vec2 Orb::getTeleportPos(float t_pHeight)
{
	b2Vec2 temp = m_b2body->GetPosition();
	temp.y -= t_pHeight;
	return temp;
}
