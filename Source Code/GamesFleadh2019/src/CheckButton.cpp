#include "CheckButton.h"



CheckButton::CheckButton(const std::string & text, sf::Font & font, int distance, bool checked, Widget * parent) : button(parent), m_label(text, font, this), m_boxDistance(distance), m_check(checked)
{
	m_checkBox.setFillColor(sf::Color::Green);
	m_chackBoxOuter.setFillColor(sf::Color::Black);
	m_chackBoxOuter.setOutlineColor(sf::Color::White);
	m_chackBoxOuter.setOutlineThickness(3);
	m_checkBox.setSize(sf::Vector2f(20, 20));
	m_chackBoxOuter.setSize(sf::Vector2f(24, 24));
	m_width = 0;
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

CheckButton::CheckButton(const std::string & text, sf::Font & font, int distance, bool checked, int width, Widget * parent) : button(parent), m_label(text, font, this), m_width(width), m_boxDistance(distance), m_check(checked)
{
	m_checkBox.setFillColor(sf::Color::Green);
	m_chackBoxOuter.setFillColor(sf::Color::Black);
	m_chackBoxOuter.setOutlineColor(sf::Color::White);
	m_chackBoxOuter.setOutlineThickness(3);
	m_checkBox.setSize(sf::Vector2f(20, 20));
	m_chackBoxOuter.setSize(sf::Vector2f(24, 24));
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

CheckButton::~CheckButton()
{
}

void CheckButton::setText(const std::string & text)
{
	m_label.setText(text);
	updateShape();
}

void CheckButton::setCharacterSize(unsigned int size)
{
	m_label.setCharacterSize(size);
	updateShape();
}

void CheckButton::setTextColor(const sf::Color & color)
{
	m_label.setTextColor(color);
	updateShape();
}

void CheckButton::setFillColor(const sf::Color & color)
{
	m_fillColor = color;
	m_shape.setFillColor(m_fillColor);
	updateShape();
}

void CheckButton::setOutlineColor(const sf::Color & color)
{
	m_outlineColor = color;
	m_shape.setOutlineColor(m_outlineColor);
	updateShape();
}

void CheckButton::setOutlineThickness(float thickness)
{
	m_shape.setOutlineThickness(thickness);
	updateShape();
}

sf::Vector2f CheckButton::getSize() const
{
	sf::FloatRect rect = m_shape.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void CheckButton::flip()
{
	m_check = !m_check;
	updateShape();
}

void CheckButton::setChecked(bool c)
{
	m_check = c;
}

bool CheckButton::getChecked()
{
	return m_check;
}

void CheckButton::select()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(m_outlineColor.r*light, m_outlineColor.g*light, m_outlineColor.b*light));
	m_shape.setFillColor(sf::Color(m_fillColor.r*light, m_fillColor.b*light, m_fillColor.b*light));
}

void CheckButton::deselect()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}

void CheckButton::updateShape()
{
	sf::Vector2f label_size = m_label.getSize();
	unsigned int char_size = m_label.getCharacterSize();
	if (m_width != 0)
	{
		m_shape.setSize(sf::Vector2f(m_width, char_size * 2 + label_size.y));
	}
	else
	{
		m_shape.setSize(sf::Vector2f(char_size * 2 + label_size.x + m_boxDistance + m_chackBoxOuter.getGlobalBounds().width, char_size * 2 + label_size.y));
	}
	m_label.setPosition(char_size, char_size);
	m_chackBoxOuter.setPosition(sf::Vector2f(char_size + label_size.x + m_boxDistance, char_size));
	m_checkBox.setPosition(sf::Vector2f(char_size + label_size.x + m_boxDistance + 2, char_size + 2));
	Widget::updateShape();
}

void CheckButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.translate(m_position);
	
	target.draw(m_shape, states);
	target.draw(m_label, states);
	target.draw(m_chackBoxOuter, states);
	if (m_check)
	{
		target.draw(m_checkBox, states);
	}
}

void CheckButton::onMouseEntered()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(m_outlineColor.r*light, m_outlineColor.g*light, m_outlineColor.b*light));
	m_shape.setFillColor(sf::Color(m_fillColor.r*light, m_fillColor.b*light, m_fillColor.b*light));
}

void CheckButton::onMouseLeft()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}
