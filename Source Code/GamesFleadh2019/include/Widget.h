#pragma once

#include <SFML\Graphics.hpp>

class Widget : public sf::Drawable
{
public:
	Widget(Widget* parent=nullptr);
	
	virtual ~Widget(); // virtual destructor for polymorphism

	void setPosition(const sf::Vector2f& t_position);
	void setPosition(float x, float y);
	const sf::Vector2f& getPosition() const;

	virtual sf::Vector2f getSize() const = 0;

	Widget* m_parent;
	// friend classes here

	// virtual functions here
	virtual bool processEvent(const sf::Event& event, const sf::Vector2f& t_parentPos);
	virtual void processEvents(const sf::Vector2f& t_parentPos);
	virtual void updateShape();

	virtual void select();
	virtual void deselect();

	std::string m_up;
	std::string m_down;
	std::string m_left;
	std::string m_right;
	std::string m_a;

	void setUp(std::string s);
	void setDown(std::string s);
	void setLeft(std::string s);
	void setRight(std::string s);
	void setA(std::string s);

	std::string getUp();
	std::string getDown();
	std::string getLeft();
	std::string getRight();
	std::string getA();

	sf::Vector2f m_position;
};

