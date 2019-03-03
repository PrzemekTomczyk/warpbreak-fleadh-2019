#include "TextButton.h"


TextButton::TextButton(const std::string & text, sf::Font & font, Widget * parent) : button(parent), m_label(text, font, this)
{
	m_width = 0;
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

TextButton::TextButton(const std::string & text, sf::Font & font, int width, Widget * parent) : button(parent), m_label(text, font, this), m_width(width)
{
	setFillColor(sf::Color(86, 20, 19));
	setOutlineThickness(5);
	setOutlineColor(sf::Color(146, 20, 19));
}

TextButton::~TextButton()
{
}

void TextButton::setText(const std::string & text)
{
	m_label.setText(text);
	updateShape();
}

void TextButton::setCharacterSize(unsigned int size)
{
	m_label.setCharacterSize(size);
	updateShape();
}

void TextButton::setTextColor(const sf::Color & color)
{
	m_label.setTextColor(color);
	updateShape();
}

void TextButton::setFillColor(const sf::Color & color)
{
	m_fillColor = color;
	m_shape.setFillColor(m_fillColor);
	updateShape();
}

void TextButton::setOutlineColor(const sf::Color & color)
{
	m_outlineColor = color;
	m_shape.setOutlineColor(m_outlineColor);
	updateShape();
}

void TextButton::setOutlineThickness(float thickness)
{
	m_shape.setOutlineThickness(thickness);
	updateShape();
}

sf::Vector2f TextButton::getSize() const
{
	sf::FloatRect rect = m_shape.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void TextButton::select()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(m_outlineColor.r*light, m_outlineColor.g*light, m_outlineColor.b*light));
	m_shape.setFillColor(sf::Color(m_fillColor.r*light, m_fillColor.b*light, m_fillColor.b*light));
}

void TextButton::deselect()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}

void TextButton::updateShape()
{
	sf::Vector2f label_size = m_label.getSize();
	//std::cout << m_label.getText.x << std::endl;
	unsigned int char_size = m_label.getCharacterSize();
	if (m_width != 0)
	{
		m_shape.setSize(sf::Vector2f(m_width, char_size * 2 + label_size.y));
	}
	else
	{
		m_shape.setSize(sf::Vector2f(char_size * 2 + label_size.x , char_size * 2 + label_size.y));
	}
	m_label.setPosition(char_size, char_size);
	Widget::updateShape();
}

void TextButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.translate(m_position);
	target.draw(m_shape, states);
	target.draw(m_label, states);
}


