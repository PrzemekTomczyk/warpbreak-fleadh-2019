#pragma once
#include "Utility.h"
#include "ResourceManager.h"

class Minimap
{
public:
	Minimap(ResourceManager &t_resourceMng);
	~Minimap();

	void init();
	void update(int t_currentRoom, bool t_hasOrb);
	void render(sf::RenderWindow &t_window);
private:
	sf::RectangleShape m_roomRects[6];
	sf::Sprite m_orbRoom;
	sf::Sprite m_endRoom;

	const int NO_OF_ROOMS = 6;
	bool m_roomVisited[6];

	ResourceManager &m_resourceMng;
};

