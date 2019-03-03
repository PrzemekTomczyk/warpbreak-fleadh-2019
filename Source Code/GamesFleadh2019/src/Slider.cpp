#include "Slider.h"




Slider::Slider(const std::string & text, sf::Font & font, int distance, int width, Widget * parent) : button(parent), m_label(text, font, this), m_width(width), m_boxDistance(distance)
{
	m_value = 1;
	m_maxValue = 100;
	m_slideBox.setFillColor(sf::Color::Green);
	m_slideBox.setSize(sf::Vector2f(20, 20));
	m_slideBoxOuter.setFillColor(sf::Color::Black);
	m_slideBoxOuter.setOutlineColor(sf::Color::White);
	m_slideBoxOuter.setOutlineThickness(3);
	m_slideBoxOuter.setSize(sf::Vector2f(124, 24));
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

Slider::~Slider()
{
}

void Slider::setText(const std::string & text)
{
	m_label.setText(text);
	updateShape();
}

void Slider::setCharacterSize(unsigned int size)
{
	m_label.setCharacterSize(size);
	updateShape();
}

void Slider::setTextColor(const sf::Color & color)
{
	m_label.setTextColor(color);
	updateShape();
}

void Slider::setFillColor(const sf::Color & color)
{
	m_fillColor = color;
	m_shape.setFillColor(m_fillColor);
	updateShape();
}

void Slider::setOutlineColor(const sf::Color & color)
{
	m_outlineColor = color;
	m_shape.setOutlineColor(m_outlineColor);
	updateShape();
}

void Slider::setOutlineThickness(float thickness)
{
	m_shape.setOutlineThickness(thickness);
	updateShape();
}

sf::Vector2f Slider::getSize() const
{
	sf::FloatRect rect = m_shape.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void Slider::setValue(int c)
{
	m_value = c;
	if (m_value < 0)
	{
		m_value = 0;
	}
	if (m_value > 100)
	{
		m_value = 100;
	}
	updateShape();
}

void Slider::changeValue(int c)
{
	m_value += c;
	if (m_value < 0)
	{
		m_value = 0;
	}
	if (m_value > 100)
	{
		m_value = 100;
	}
	updateShape();
}

int Slider::getValue()
{
	return m_value;
}

void Slider::select()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(m_outlineColor.r*light, m_outlineColor.g*light, m_outlineColor.b*light));
	m_shape.setFillColor(sf::Color(m_fillColor.r*light, m_fillColor.b*light, m_fillColor.b*light));
}

void Slider::deselect()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}

void Slider::updateShape()
{
	sf::Vector2f label_size = m_label.getSize();
	unsigned int char_size = m_label.getCharacterSize();
	int boxWidth = m_slideBoxOuter.getGlobalBounds().width;
	if (m_width != 0)
	{
		m_shape.setSize(sf::Vector2f(m_width, char_size * 2 + label_size.y));
	}
	else
	{
		m_shape.setSize(sf::Vector2f(char_size * 2 + label_size.x + m_boxDistance + boxWidth, char_size * 2 + label_size.y));
	}
	m_label.setPosition(char_size, char_size);

	m_slideBoxOuter.setPosition(sf::Vector2f(char_size + label_size.x + m_boxDistance, char_size));

	m_slideBox.setPosition(sf::Vector2f(char_size + label_size.x + m_boxDistance + 2 + m_value, char_size + 2));
	Widget::updateShape();
}

void Slider::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.translate(m_position);

	target.draw(m_shape, states);
	target.draw(m_label, states);
	target.draw(m_slideBoxOuter, states);
	target.draw(m_slideBox, states);
}

void Slider::onMouseEntered()
{
}

void Slider::onMouseLeft()
{
}
