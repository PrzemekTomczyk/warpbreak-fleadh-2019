#pragma once
#include "Widget.h"
#include <iostream>
#include <SFML\Graphics.hpp>

class Label : public Widget
{
public:
	Label(const std::string& text, sf::Font & font, Widget* parent = nullptr);
	Label(const std::string & text, Widget * parent = nullptr);
	virtual ~Label();

	void setText(const std::string& text);
	std::string getText()const;
	void setCharacterSize(unsigned int size);
	unsigned int getCharacterSize()const;
	void setTextColor(const sf::Color& colour);
	virtual sf::Vector2f getSize()const override;

private:
	sf::Text m_text;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};