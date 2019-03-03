#include "RadioButton.h"




RadioButton::RadioButton(const std::string & text, sf::Font & font, int distance, Widget * parent) : button(parent), m_label(text, font, this), m_boxDistance(distance)
{
	m_currChecked = 1;
	m_optionsAmount = 3;
	m_boxseperation = 20;
	m_checkBox.setFillColor(sf::Color::Green);
	m_checkBox.setSize(sf::Vector2f(20, 20));
	for (int i = 0; i < m_optionsAmount; i++)
	{
		m_chackBoxOuter[i].setFillColor(sf::Color::Black);
		m_chackBoxOuter[i].setOutlineColor(sf::Color::White);
		m_chackBoxOuter[i].setOutlineThickness(3);
		m_chackBoxOuter[i].setSize(sf::Vector2f(24, 24));
	}
	m_width = 0;
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

RadioButton::RadioButton(const std::string & text, sf::Font & font, int distance, int width, Widget * parent) : button(parent), m_label(text, font, this), m_width(width), m_boxDistance(distance)
{
	m_currChecked = 1;
	m_optionsAmount = 3;
	m_boxseperation = 20;
	m_checkBox.setFillColor(sf::Color::Green);
	m_checkBox.setSize(sf::Vector2f(20, 20));
	for (int i = 0; i < m_optionsAmount; i++)
	{
		m_chackBoxOuter[i].setFillColor(sf::Color::Black);
		m_chackBoxOuter[i].setOutlineColor(sf::Color::White);
		m_chackBoxOuter[i].setOutlineThickness(3);
		m_chackBoxOuter[i].setSize(sf::Vector2f(24, 24));
	}
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

RadioButton::~RadioButton()
{
}

void RadioButton::setText(const std::string & text)
{
	m_label.setText(text);
	updateShape();
}

void RadioButton::setCharacterSize(unsigned int size)
{
	m_label.setCharacterSize(size);
	updateShape();
}

void RadioButton::setTextColor(const sf::Color & color)
{
	m_label.setTextColor(color);
	updateShape();
}

void RadioButton::setFillColor(const sf::Color & color)
{
	m_fillColor = color;
	m_shape.setFillColor(m_fillColor);
	updateShape();
}

void RadioButton::setOutlineColor(const sf::Color & color)
{
	m_outlineColor = color;
	m_shape.setOutlineColor(m_outlineColor);
	updateShape();
}

void RadioButton::setOutlineThickness(float thickness)
{
	m_shape.setOutlineThickness(thickness);
	updateShape();
}

sf::Vector2f RadioButton::getSize() const
{
	sf::FloatRect rect = m_shape.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void RadioButton::setChecked(int c)
{
	m_currChecked = c;
	updateShape();
}

int RadioButton::getChecked()
{
	return m_currChecked;
}

void RadioButton::select()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(m_outlineColor.r*light, m_outlineColor.g*light, m_outlineColor.b*light));
	m_shape.setFillColor(sf::Color(m_fillColor.r*light, m_fillColor.b*light, m_fillColor.b*light));
}

void RadioButton::deselect()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}

void RadioButton::updateShape()
{
	sf::Vector2f label_size = m_label.getSize();
	unsigned int char_size = m_label.getCharacterSize();
	int boxWidth = m_chackBoxOuter[1].getGlobalBounds().width;
	if (m_width != 0)
	{
		m_shape.setSize(sf::Vector2f(m_width, char_size * 2 + label_size.y));
	}
	else
	{
		m_shape.setSize(sf::Vector2f(char_size * 2 + label_size.x + m_boxDistance + boxWidth * 3 + m_boxseperation * 2, char_size * 2 + label_size.y));
	}
	m_label.setPosition(char_size, char_size);

	for (int i = 0; i < m_optionsAmount; i++)
	{
		int d = (boxWidth + m_boxseperation) * i;
		m_chackBoxOuter[i].setPosition(sf::Vector2f(char_size + label_size.x + m_boxDistance + d, char_size));
	}

	int d = (boxWidth + m_boxseperation) * (m_currChecked - 1);
	m_checkBox.setPosition(sf::Vector2f(char_size + label_size.x + m_boxDistance + 2 + d, char_size + 2));
	Widget::updateShape();
}

void RadioButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.translate(m_position);

	target.draw(m_shape, states);
	target.draw(m_label, states);
	for (int i = 0; i < m_optionsAmount; i++)
	{
		target.draw(m_chackBoxOuter[i], states);
	}
	target.draw(m_checkBox, states);
}

void RadioButton::onMouseEntered()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(m_outlineColor.r*light, m_outlineColor.g*light, m_outlineColor.b*light));
	m_shape.setFillColor(sf::Color(m_fillColor.r*light, m_fillColor.b*light, m_fillColor.b*light));
}

void RadioButton::onMouseLeft()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}
