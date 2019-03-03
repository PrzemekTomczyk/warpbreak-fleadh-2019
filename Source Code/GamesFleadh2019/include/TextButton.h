#pragma once

#include<SFML\Graphics.hpp>
#include "button.h"
#include "Label.h"
#include <iostream>

class TextButton : public button
{
public:
	TextButton(const std::string& text, sf::Font & font, Widget* parent = nullptr);
	TextButton(const std::string& text, sf::Font & font, int width, Widget* parent = nullptr);
	virtual ~TextButton();
	void setText(const std::string& text);
	void setCharacterSize(unsigned int size);
	void setTextColor(const sf::Color& color);
	void setFillColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	void setOutlineThickness(float thickness);
	virtual sf::Vector2f getSize()const override;
	void select();
	void deselect();

private:
	int m_width;
	sf::RectangleShape m_shape;
	Label m_label;
	void updateShape()override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Color m_fillColor;
	sf::Color m_outlineColor;
};