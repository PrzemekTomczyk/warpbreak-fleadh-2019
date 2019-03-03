#include "Player.h"

#define COLOR_NORMAL sf::Color(41, 128, 185)
#define COLOR_HIT sf::Color(231, 76, 60)

Player::Player(ResourceManager & t_resources, Input & t_input, Xbox360Controller &t_controller, sf::RenderWindow & t_window, ParticlePool & t_pool, Options & t_options, b2World& t_world) :
	m_resourceMng(t_resources),
	m_particlePool(t_pool),
	m_hasOrb(false),
	MASS(10),
	SIZE(32),
	SPEED(50),
	MAX_SPEED(SPEED * PIXELS_TO_METERS),
	STARTING_POS(SCREEN_CENTER.x, PLANE_POS.y - (PLANE_SIZE.y / 2.0f)),
	JUMP_VELO(-35),
	MAX_FRAMES(29),
	TIME_PER_FRAME(160.0f),
	m_orbCooldown(0.0f),
	m_orbDelay(0.0f),
	m_input(t_input),
	m_controller(t_controller),
	m_options(t_options),
	m_orb(t_resources),
	m_world(t_world)
{
	fsm.start();
	m_punchCooldown = 0;

	m_font.loadFromFile(".//assets//fonts//ariblk.ttf");

	//------------------Idle------------------------
	fsm.getIdleRef()->setTexture(m_resourceMng.getTexture(TextureID::PLAYER_IDLE));
	setAnimRect(*fsm.getIdleRef(), 29, 11, 128, 128, 0);
	fsm.getIdleRef()->setTime(Time(seconds(1.0f / 30.0f)));
	fsm.getIdleRef()->setOrigin(128 / 2, 128 / 2);
	fsm.getIdleRef()->setOffset(sf::Vector2f(-20, -10));
	fsm.getIdleRef()->setLooping(true);

	//------------------Run-------------------------
	fsm.getRunRef()->setTexture(m_resourceMng.getTexture(TextureID::PLAYER_RUN));
	setAnimRect(*fsm.getRunRef(), 24, 11, 128, 128, 0);
	fsm.getRunRef()->setTime(Time(seconds(1.0f / 30.0f)));
	fsm.getRunRef()->setOrigin(128 / 2, 128 / 2);
	fsm.getRunRef()->setOffset(sf::Vector2f(-15, -10));
	fsm.getRunRef()->setLooping(true);

	//------------------Jump------------------------
	fsm.getJumpRef()->setTexture(m_resourceMng.getTexture(TextureID::PLAYER_JUMP));
	setAnimRect(*fsm.getJumpRef(), 8, 11, 128, 128, 0);
	fsm.getJumpRef()->setTime(Time(seconds(1.0f / 30.0f)));
	fsm.getJumpRef()->setOrigin(128 / 2, 128 / 2);
	fsm.getJumpRef()->setOffset(sf::Vector2f(-15, -10));
	fsm.getJumpRef()->setLooping(false);

	//------------------Falling---------------------
	fsm.getFallRef()->setTexture(m_resourceMng.getTexture(TextureID::PLAYER_JUMP));
	setAnimRect(*fsm.getFallRef(), 11, 11, 128, 128, 128);
	fsm.getFallRef()->setTime(Time(seconds(1.0f / 30.0f)));
	fsm.getFallRef()->setOrigin(128 / 2, 128 / 2);
	fsm.getFallRef()->setOffset(sf::Vector2f(-15, -10));
	fsm.getFallRef()->setLooping(false);

	//------------------Punch-----------------------
	fsm.getPunchRef()->setTexture(m_resourceMng.getTexture(TextureID::PLAYER_PUNCH));
	setAnimRect(*fsm.getPunchRef(), 20, 11, 128, 128, 0);
	fsm.getPunchRef()->setTime(Time(seconds(1.0f / 60.0f)));
	fsm.getPunchRef()->setOrigin(128 / 2, 128 / 2);
	fsm.getPunchRef()->setOffset(sf::Vector2f(-15, -10));
	fsm.getPunchRef()->setLooping(true);

	updateSize();

	m_lives = 10;

	m_stepSound.setBuffer(m_resourceMng.getBuffer(SoundBufferID::STEP));
	m_landSound.setBuffer(m_resourceMng.getBuffer(SoundBufferID::JUMP_LAND));
	m_warpSound.setBuffer(m_resourceMng.getBuffer(SoundBufferID::WARP));
	m_pickupSound.setBuffer(m_resourceMng.getBuffer(SoundBufferID::ORB_PICKUP));

}

void Player::update(sf::Time t_deltaTime)
{
	m_stepSound.setVolume(m_options.soundVol);
	m_warpSound.setVolume(m_options.soundVol);
	m_pickupSound.setVolume(m_options.soundVol * 2);
	
	if (!m_isPunching)
	{
		handleSpriteFacingDirection();
		handleMovement(t_deltaTime.asSeconds());
	}
	if (m_hasOrb)
	{
		handleOrb(t_deltaTime.asSeconds());
	}
	handlePunching(t_deltaTime.asSeconds());

	if (fsm.current_state_ptr->currState == 1 && !m_airborn)
	{
		if (fsm.current_state_ptr->currState == 1 && !m_airborn)
		{
			if (fsm.getAnim()->getCurrentFrame() == 0 || fsm.getAnim()->getCurrentFrame() == 9 || fsm.getAnim()->getCurrentFrame() == 17)
			{
				step();
			}
		}
	}
	fsm.updateAnim();

	setPos(m_b2body->GetPosition());
}

void Player::render(sf::RenderWindow & t_window)
{
	if (m_facingLeft)
	{
		int x = -fsm.getAnim()->getOffset().x + m_position.x;
		int y = fsm.getAnim()->getOffset().y + m_position.y;
		fsm.getAnim()->setScale(1, 1);
		fsm.getAnim()->setPosition(sf::Vector2f(x, y));
	}
	else
	{
		int x = fsm.getAnim()->getOffset().x + m_position.x;
		int y = fsm.getAnim()->getOffset().y + m_position.y;
		fsm.getAnim()->setScale(-1, 1);
		fsm.getAnim()->setPosition(sf::Vector2f(x, y));
	}
	fsm.getAnim()->setTextureRect(fsm.getAnim()->getFrame(fsm.getAnim()->getCurrentFrame()));
	t_window.draw(*fsm.getAnim());
	m_orb.render(t_window);

#ifdef _DEBUG
	debugDraw(t_window);
#endif // _DEBUG

}

bool Player::isFacingLeft()
{
	return m_facingLeft;
}

void Player::movePos(sf::Vector2f t_vec)
{
	m_position.x += t_vec.x;
	m_position.y += t_vec.y;
}

int Player::getFrameWidth()
{
	return fsm.getAnim()->getFrame(fsm.getAnim()->getCurrentFrame()).width;
}

int Player::getFrameHeight()
{
	return fsm.getAnim()->getFrame(fsm.getAnim()->getCurrentFrame()).height;
}

void Player::setAirborn(bool t_bool)
{
	m_airborn = t_bool;
}

void Player::setHasOrb(bool t_orb)
{
	m_hasOrb = t_orb;
	m_pickupSound.play();
}

void Player::resetHasOrb()
{
	m_hasOrb = false;
}

bool Player::isPunching()
{
	return m_isPunching;
}

void Player::playStep()
{
	if (m_stepSound.getStatus() == m_stepSound.Stopped)
	{
		if (!m_options.mute)
		{
			m_stepSound.play();

		}
	}
}

void Player::playLanding()
{	
	m_landSound.stop();

	float scale = std::abs(m_b2body->GetLinearVelocity().y) / std::abs(JUMP_VELO);

	m_landSound.setVolume(m_options.soundVol * scale);

	if (!m_options.mute)
	{
		m_landSound.play();

	}
	Vector2f position = Vector2f(m_position.x, m_position.y + m_height / 2);
	m_particlePool.createLandingParticles(m_resourceMng.getTexture(TextureID::SMOKE), position, 1000 * scale);

	
}

void Player::setB2Body(b2Body* t_body)
{
	m_b2body = t_body;
}

b2Body * Player::getB2Body()
{
	return m_b2body;
}

void Player::setUserData()
{
	m_userData.data = this;
	m_userData.entity = 0;
	m_b2body->SetUserData(&m_userData);
}

float Player::getChargeScale()
{

	float val = m_orbCooldown / 2.0f;
	return 1.0f - val;
}

void Player::setLives(int t_lives)
{
	m_lives = t_lives;
}

float Player::getOrbCooldown() const
{
	return m_orbCooldown;
}

void Player::setupPlayer(b2World &t_world, bool t_fixedRotaion, uint16 categoryBits)
{
	fsm.dispatch(idle);
	if (m_isPunching)
	{
		m_b2body->DestroyFixture(m_punchFixture);
		m_isPunching = false;
	}
	if (m_b2body != nullptr)
	{
		t_world.DestroyBody(m_b2body);
		m_particlePool.clearParticles();
	}

	m_alive = true;

	// add player to world
	b2BodyDef playerDef;
	playerDef.position.Set(m_startPosition.x / SCALE, m_startPosition.y / SCALE);
	playerDef.type = b2_dynamicBody;
	playerDef.fixedRotation = t_fixedRotaion;

	b2Body* playerBody = t_world.CreateBody(&playerDef);

	m_userData.data = this;
	m_userData.entity = 0;

	playerBody->SetUserData(&m_userData);

	//-------------body shape---------------------
	b2PolygonShape playerBox;
	playerBox.SetAsBox(26 / SCALE, 52 / SCALE, b2Vec2(0, 0 / SCALE), float32(0));

	b2FixtureDef playerFixtureDef;
	playerFixtureDef.density = 2.0f;
	playerFixtureDef.friction = 0.0f;
	playerFixtureDef.shape = &playerBox;
	playerFixtureDef.filter.categoryBits = categoryBits;
	playerBody->CreateFixture(&playerFixtureDef);
	setB2Body(playerBody);
	
	m_orb.destroyOrb();

	m_orbCooldown = 0.0f;
	m_orbDelay = 0.5f;
}

void Player::teleportPlayer(b2Vec2 t_pos, uint16 categoryBits)
{
	b2Vec2 velocity = m_b2body->GetLinearVelocity();
	sf::Vector2f temp = sf::Vector2f(t_pos.x * SCALE, t_pos.y * SCALE);
	m_particlePool.createTeleportParticles(m_resourceMng.getTexture(TextureID::TELEPORT_PARTICLE), m_position);
	m_particlePool.createTeleportParticles(m_resourceMng.getTexture(TextureID::TELEPORT_PARTICLE), temp);

	if (m_b2body != nullptr)
	{
		m_world.DestroyBody(m_b2body);
	}

	m_alive = true;

	// add player to world
	b2BodyDef playerDef;
	playerDef.position.Set(t_pos.x, t_pos.y);
	playerDef.type = b2_dynamicBody;
	playerDef.fixedRotation = true;

	b2Body* playerBody = m_world.CreateBody(&playerDef);

	m_userData.data = this;
	m_userData.entity = 0;

	playerBody->SetUserData(&m_userData);

	//-------------body shape---------------------
	b2PolygonShape playerBox;
	playerBox.SetAsBox(26 / SCALE, 52 / SCALE, b2Vec2(0, 0 / SCALE), float32(0));

	b2FixtureDef playerFixtureDef;
	playerFixtureDef.density = 2.0f;
	playerFixtureDef.friction = 0.0f;
	playerFixtureDef.shape = &playerBox;
	playerFixtureDef.filter.categoryBits = categoryBits;
	playerBody->CreateFixture(&playerFixtureDef);
	setB2Body(playerBody);

	m_b2body->SetLinearVelocity(b2Vec2(0.0f, velocity.y));
}

void Player::handleMovement(float t_time)
{
	if (m_input.m_current.A || m_input.m_current.D || m_controller.m_currentState.LeftThumbStick.x > 50 || m_controller.m_currentState.LeftThumbStick.x < -50)
	{
		if (!m_airborn)
		{
			fsm.dispatch(run);
		}
	}
	else
	{
		if (!m_airborn)
		{
			fsm.dispatch(idle);
		}
	}

	if (m_input.m_current.D || m_controller.m_currentState.LeftThumbStick.x > 50)
	{
		m_b2body->SetLinearVelocity(b2Vec2(SPEED * 0.1f, m_b2body->GetLinearVelocity().y));
	}
	
	if (m_input.m_current.A || m_controller.m_currentState.LeftThumbStick.x < -50)
	{
		m_b2body->SetLinearVelocity(b2Vec2(-SPEED * 0.1f, m_b2body->GetLinearVelocity().y));
	}

	if (!m_input.m_current.A && !m_input.m_current.D &&
		m_controller.m_currentState.LeftThumbStick.x < 50 &&
		m_controller.m_currentState.LeftThumbStick.x > -50)
	{
		m_b2body->SetLinearVelocity(b2Vec2(0, m_b2body->GetLinearVelocity().y));
	}

	jumpHandler();
}

void Player::handlePunching(float t_time)
{
	float punchTime = 1.0f / 24.0f * 10.0f;

	if (!m_isPunching)
	{
		if ((m_input.m_current.F || m_controller.m_currentState.B) && !m_airborn)
		{
			createPunchFixture();
			m_punchCooldown = punchTime;
			m_isPunching = true;
			fsm.dispatch(punch);
			m_b2body->SetLinearVelocity(b2Vec2(0, m_b2body->GetLinearVelocity().y));
		}
	}
	else
	{
		m_punchCooldown -= t_time;
		if (m_punchCooldown <= 0)
		{
			m_b2body->DestroyFixture(m_punchFixture);
			m_isPunching = false;
			fsm.dispatch(idle);
		}
	}
}

void Player::handleOrb(float t_time)
{
	m_orb.setFrame(t_time);
	m_orbCooldown -= t_time;
	m_orbDelay -= t_time;

	if (m_orbCooldown < 0)
	{ 
		m_orbCooldown = 0; 
	}
	if (m_orbDelay < 0) 
	{ 
		m_orbDelay = 0; 
	}
	if (m_orbDelay <= 0)
	{

		if ((m_input.m_current.E || m_controller.m_currentState.X) && !m_isPunching)
		{
			if (!m_orb.getAlive())
			{
				if (m_input.m_current.S || m_controller.m_currentState.LeftThumbStick.y > 20)
				{
					m_orb.createOrb(m_world, m_position, m_facingLeft, true, _entityCategory::ORB, _entityCategory::ENEMY | _entityCategory::WORLD | _entityCategory::EXIT);
				}
				else {
					m_orb.createOrb(m_world, m_position, m_facingLeft, false, _entityCategory::ORB, _entityCategory::ENEMY | _entityCategory::WORLD | _entityCategory::EXIT);
				}
				m_orbDelay = 0.5f;
			}
			else if (m_orbCooldown <= 0)
			{
				teleportPlayer(m_orb.getTeleportPos(64 / SCALE), _entityCategory::PLAYER);
				m_orb.destroyOrb();
				m_orbCooldown = 2.0f;
				m_orbDelay = 0.5f;


				if (m_warpSound.getStatus() == m_warpSound.Stopped)
				{
					if (!m_options.mute)
					{
						m_warpSound.play();

					}
				}
			}
		}
		if ((m_input.m_current.Q || m_controller.m_currentState.Y) && m_orb.getAlive())
		{
			m_orb.destroyOrb();
			m_orbDelay = 0.5f;
			m_pickupSound.play();
		}
	}

	if (m_orb.getAlive())
	{
		m_orb.setSpriteAngle();
	}
}

void Player::createPunchFixture()
{
	b2FixtureDef sensorFixtureDef;
	sensorFixtureDef.isSensor = true;

	if (!m_facingLeft)
	{
		b2PolygonShape sensorBox;
		sensorBox.SetAsBox((20) / SCALE, (10) / SCALE, b2Vec2((26 + 20) / SCALE, -15 / SCALE), float32(0));

		sensorFixtureDef.shape = &sensorBox;
		m_punchFixture = m_b2body->CreateFixture(&sensorFixtureDef);
	}
	else
	{
		b2PolygonShape sensorBox2;
		sensorBox2.SetAsBox((20) / SCALE, (10) / SCALE, b2Vec2((-26 - 20) / SCALE, -15 / SCALE), float32(0));
		sensorFixtureDef.shape = &sensorBox2;
		m_punchFixture = m_b2body->CreateFixture(&sensorFixtureDef);
	}

}

void Player::handleSpriteFacingDirection()
{
	if (m_input.m_current.D || m_controller.m_currentState.LeftThumbStick.x > 50 && !(m_input.m_current.A || m_controller.m_currentState.LeftThumbStick.x < -50))
	{
		m_facingLeft = false;
	}

	if (m_input.m_current.A || m_controller.m_currentState.LeftThumbStick.x < -50 && !(m_input.m_current.D || m_controller.m_currentState.LeftThumbStick.x > 50))
	{
		m_facingLeft = true;
	}
}

void Player::setAnimRect(AnimatedSprite & anim, int frames, int framesPerLine, int width, int height, int pos)
{
	int count = 0;
	int line = 0;
	while (count < frames -1)
	{
		for (int i = 0; i < framesPerLine; i++)
		{
			IntRect rec(0 + width * i, pos + height*line, width, height);
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

void Player::updateSize()
{
	m_width = fsm.getAnim()->getFrame(fsm.getAnim()->getCurrentFrame()).width;
	m_height = fsm.getAnim()->getFrame(fsm.getAnim()->getCurrentFrame()).height;
}

void Player::jumpHandler()
{
	if (!m_airborn)
	{
		if (m_input.m_current.Space || m_controller.m_currentState.A)
		{
			fsm.dispatch(jump);
			m_b2body->ApplyLinearImpulse(b2Vec2(0, JUMP_VELO), m_b2body->GetWorldCenter(), true);
			m_airborn = true;
		}
	}
	else
	{
		if (m_b2body->GetLinearVelocity().y > 0)
		{
			fsm.dispatch(fall);
		}
	}
}

void Player::step()
{
	Vector2f temp = Vector2f(m_position.x, m_position.y + m_height / 2.8);
	m_particlePool.createPuff(m_resourceMng.getTexture(TextureID::SMOKE), temp);
	playStep();
}

void Player::debugDraw(sf::RenderWindow & t_window)
{
	for (b2Fixture* f = m_b2body->GetFixtureList(); f; f = f->GetNext())
	{
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

		sf::ConvexShape convex;
		convex.setOutlineColor(sf::Color::Green);
		convex.setOutlineThickness(1.0f);
		convex.setFillColor(sf::Color::Transparent);
		convex.setPosition(m_position);
		convex.setPointCount(poly->m_count);

		for (int i = 0; i < poly->m_count; i++)
		{
			convex.setPoint(i, sf::Vector2f(poly->m_vertices[i].x * SCALE, poly->m_vertices[i].y * SCALE));
		}

		t_window.draw(convex);
		if (f->IsSensor())
		{
		}
	}
}
