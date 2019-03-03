#include "Minimap.h"

Minimap::Minimap(ResourceManager &t_resourceMng) :
	m_resourceMng(t_resourceMng)
{
	init();
}

Minimap::~Minimap()
{
}

void Minimap::init()
{
	int positionScalar = 0;
	for (int i = 0; i < NO_OF_ROOMS; i++)
	{
		m_roomVisited[i] = false;

		m_roomRects[i].setSize(sf::Vector2f(40.0f, 20.0f));
		m_roomRects[i].setFillColor(sf::Color(70, 70, 70));

		m_roomRects[i].setPosition(
			sf::Vector2f(
				(SCREEN_SIZE.x - m_roomRects[i].getSize().x * 5) + (m_roomRects[i].getSize().x * positionScalar),
				5
			)
		);

		if (i == 0)
		{
			sf::Vector2f position;
			position.x = m_roomRects[i].getPosition().x;
			position.y = 5;
			m_roomRects[i].setPosition(position);
		}
		else if (i == 2)
		{
			m_roomRects[i].setPosition(
				m_roomRects[i].getPosition().x,
				m_roomRects[0].getPosition().y + m_roomRects[i].getSize().y
			);
		}
		else if (i == 3)
		{
			m_roomRects[i].setPosition(
				m_roomRects[i].getPosition().x + m_roomRects[i].getSize().x,
				m_roomRects[0].getPosition().y + m_roomRects[i].getSize().y
			);
		}

		if (i != 1 && i !=2)
		{
			positionScalar++;
		}
	}

	m_orbRoom.setTexture(m_resourceMng.getTexture(TextureID::ORB));
	m_orbRoom.setTextureRect(sf::IntRect(677, 0, 113, 113));
	float rectWidth = m_orbRoom.getTextureRect().width;
	float orbScale = (m_roomRects[3].getSize().y / rectWidth);
	m_orbRoom.setScale(orbScale, orbScale);
	m_orbRoom.setOrigin(m_orbRoom.getTextureRect().width / 2.0f, m_orbRoom.getTextureRect().height / 2.0f);
	m_orbRoom.setPosition(m_roomRects[3].getPosition() + (m_roomRects[3].getSize() / 2.0f));

	m_endRoom.setTexture(m_resourceMng.getTexture(TextureID::GOAL));
	float goalWidth = m_endRoom.getTextureRect().width;
	float goalScale = (m_roomRects[5].getSize().y / goalWidth);
	m_endRoom.setScale(goalScale, goalScale);
	m_endRoom.setOrigin(m_endRoom.getTexture()->getSize().x / 2.0f, m_endRoom.getTexture()->getSize().y / 2.0f);
	m_endRoom.setPosition(m_roomRects[5].getPosition() + (m_roomRects[5].getSize() / 2.0f));
}

void Minimap::update(int t_currentRoom, bool t_hasOrb)
{
	m_roomVisited[t_currentRoom - 1] = true;
	for (int i = 0; i < NO_OF_ROOMS; i++)
	{
		if (m_roomVisited[i])
		{
			//light grey if visited
			m_roomRects[i].setFillColor(sf::Color(160, 160, 160));
		}
		else
		{
			//light black if not visited yet
			m_roomRects[i].setFillColor(sf::Color(70, 70, 70));
		}
	}
	//very light grey for current room
	m_roomRects[t_currentRoom - 1].setFillColor(sf::Color(255, 255, 255));

	if (t_hasOrb)
	{
		m_orbRoom.setTexture(m_resourceMng.getTexture(TextureID::CHECK));
		//change rect to be the full texture
		m_orbRoom.setTextureRect(
			sf::IntRect(
				0, 
				0, 
				m_resourceMng.getTexture(TextureID::CHECK).getSize().x, 
				m_resourceMng.getTexture(TextureID::CHECK).getSize().y
			)
		);

		float rectWidth = m_orbRoom.getTextureRect().width;
		float rectHeight = m_orbRoom.getTextureRect().height;
		float xScale = (m_roomRects[0].getSize().x / rectWidth);
		float yScale = (m_roomRects[0].getSize().y / rectHeight);
		m_orbRoom.setScale(xScale, yScale);
		m_orbRoom.setOrigin(m_orbRoom.getTexture()->getSize().x / 2.0f, m_orbRoom.getTexture()->getSize().y / 2.0f);
		m_orbRoom.setPosition(m_roomRects[3].getPosition() + (m_roomRects[3].getSize() / 2.0f));
	}
}

void Minimap::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < NO_OF_ROOMS; i++)
	{
		t_window.draw(m_roomRects[i]);
	}
	t_window.draw(m_roomRects[0]);

	t_window.draw(m_orbRoom);
	t_window.draw(m_endRoom);
}
