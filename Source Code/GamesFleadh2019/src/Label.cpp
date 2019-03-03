#include "Label.h"


Label::Label(const std::string & text, sf::Font & font, Widget * parent)
{
	m_text.setFont(font);
	setText(text);
	setTextColor(sf::Color::White);
}

Label::Label(const std::string & text, Widget * parent)
{
	setText(text);
	setTextColor(sf::Color::White);
}

Label::~Label()
{
}

void Label::setText(const std::string & text)
{
	m_text.setString(text);
	updateShape();
}

std::string Label::getText() const
{
	return m_text.getString();
}

void Label::setCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
	updateShape();
}

unsigned int Label::getCharacterSize() const
{
	return m_text.getCharacterSize();
}

void Label::setTextColor(const sf::Color & colour)
{
	m_text.setFillColor(colour);
}

sf::Vector2f Label::getSize() const
{
	sf::FloatRect rect = this->m_text.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void Label::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.translate(m_position);
	target.draw(m_text, states);
}
