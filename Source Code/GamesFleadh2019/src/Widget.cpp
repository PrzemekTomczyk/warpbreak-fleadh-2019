#include "Widget.h"



Widget::Widget(Widget * parent)
	: m_parent(parent),
	m_up(""),
	m_down(""),
	m_left(""),
	m_right(""),
	m_a("")
{
}

Widget::~Widget()
{
}

void Widget::setPosition(const sf::Vector2f & t_position)
{
	m_position = t_position;
}

void Widget::setPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
}

const sf::Vector2f & Widget::getPosition() const
{
	return m_position;
}

bool Widget::processEvent(const sf::Event & event, const sf::Vector2f & t_parentPos)
{
	return false;
}

void Widget::processEvents(const sf::Vector2f & t_parentPos)
{
}

void Widget::updateShape()
{
	if (m_parent)
	{
		m_parent->updateShape();
	}
}

void Widget::select()
{
}

void Widget::deselect()
{
}

void Widget::setUp(std::string s)
{
	m_up = s;
}

void Widget::setDown(std::string s)
{
	m_down = s;
}

void Widget::setLeft(std::string s)
{
	m_left = s;
}

void Widget::setRight(std::string s)
{
	m_right = s;
}

void Widget::setA(std::string s)
{
	m_a = s;
}

std::string Widget::getUp()
{
	return m_up;
}

std::string Widget::getDown()
{
	return m_down;
}

std::string Widget::getLeft()
{
	return m_left;
}

std::string Widget::getRight()
{
	return m_right;
}

std::string Widget::getA()
{
	return m_a;
}
