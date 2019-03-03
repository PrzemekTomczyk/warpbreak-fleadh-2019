#pragma once

#include "Container.h"
#include <SFML\Graphics.hpp>

class Frame : public Container
{
public:
	Frame(sf::RenderWindow& window);
	virtual ~Frame();
	void processEvents();
	bool processEvent(const sf::Event& event);
	//void bind(int key, const FuncType& callback);
	//void unbind(int key);
	void draw();
	virtual sf::Vector2f getSize()const override;
private:
	sf::RenderWindow& m_window;
	virtual bool processEvent(const sf::Event& event, const sf::Vector2f& parent_pos)override;
	virtual void processEvents(const sf::Vector2f& parent_pos)override;
};

