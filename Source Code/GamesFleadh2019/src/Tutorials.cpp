#include "Hud.h"
#include "..\include\Tutorials.h"

Tutorials::Tutorials(ResourceManager & t_resources) :
	m_resource(t_resources),
	m_currentLevel(1)
{
	m_move.setTexture(m_resource.getTexture(TextureID::TUTORIAL_MOVE));
	m_jump.setTexture(m_resource.getTexture(TextureID::TUTORIAL_JUMP));
	m_punch.setTexture(m_resource.getTexture(TextureID::TUTORIAL_PUNCH));
	m_punchFail.setTexture(m_resource.getTexture(TextureID::TUTORIAL_PUNCH_FAIL));
	m_punchSuccess.setTexture(m_resource.getTexture(TextureID::TUTORIAL_PUNCH_SUCCESS));
	m_orbThrow.setTexture(m_resource.getTexture(TextureID::TUTORIAL_ORB_THROW));
	m_orbTeleport.setTexture(m_resource.getTexture(TextureID::TUTORIAL_ORB_TELEPORT));
	m_orbRecall.setTexture(m_resource.getTexture(TextureID::TUTORIAL_ORB_RECALL));
	m_pedastalEmpty.setTexture(m_resource.getTexture(TextureID::PEDASTAL_EMPTY));
	m_orb.setTexture(m_resource.getTexture(TextureID::ORB));

	m_orb.setTextureRect(sf::IntRect(0, 0, 113, 113));
	m_orb.setOrigin(m_orb.getTextureRect().width / 2.0f, m_orb.getTextureRect().height / 2.0f);

	m_move.setPosition(100, 900);
	m_jump.setPosition(600, 900);
	m_punch.setPosition(400, 450);
	m_punchFail.setPosition(500, 700);
	m_punchSuccess.setPosition(800, 700);
	m_orbThrow.setPosition(600, 880);
	m_orbTeleport.setPosition(1500, 870);
	m_orbRecall.setPosition(50, 800);
	m_pedastalEmpty.setPosition(1100, 950);
	m_orb.setPosition(1152, 950);

	m_move.setScale(0.3, 0.3);

}

Tutorials::~Tutorials()
{
}

void Tutorials::update(int t_level, float t_deltaTime)
{
	m_currentLevel = t_level;

	m_frameTime -= t_deltaTime;
	if (m_frameTime <= 0.0f)
	{
		m_frameTime = FRAME_DURATION;
		m_currentFrame++;
		if (m_currentFrame > 12)
		{
			m_currentFrame = 0;
		}
		m_orb.setTextureRect(sf::IntRect(113 * m_currentFrame, 0, 113, 113));
	}
}

void Tutorials::render(sf::RenderWindow & t_window, bool t_hasOrb)
{
	switch (m_currentLevel)
	{
	case 1:
		t_window.draw(m_move);
		t_window.draw(m_jump);
		break;
	case 2:
		t_window.draw(m_punch);
		t_window.draw(m_punchFail);
		t_window.draw(m_punchSuccess);
		break;
	case 4:
		t_window.draw(m_pedastalEmpty);
		if (!t_hasOrb)
		{
			float rectWidth = m_orb.getTextureRect().width;
			float orbScale = ((16.0f * 2.0f) / rectWidth);
			m_orb.setScale(sf::Vector2f(orbScale, orbScale));
			t_window.draw(m_orb);
		}
		else
		{
			t_window.draw(m_orbThrow);
			t_window.draw(m_orbTeleport);
			t_window.draw(m_orbRecall);
		}
		break;
	default:
		break;
	}
}
