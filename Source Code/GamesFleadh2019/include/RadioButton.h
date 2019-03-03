#pragma once

#include<SFML\Graphics.hpp>
#include "button.h"
#include "Label.h"

class RadioButton : public button
{
public:
	RadioButton(const std::string& text, sf::Font & font, int distance, Widget* parent = nullptr);
	RadioButton(const std::string& text, sf::Font & font, int distance, int width, Widget* parent = nullptr);
	~RadioButton();
	void setText(const std::string& text);
	void setCharacterSize(unsigned int size);
	void setTextColor(const sf::Color& color);
	void setFillColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	void setOutlineThickness(float thickness);
	virtual sf::Vector2f getSize()const override;
	void setChecked(int c);
	int getChecked();
	void select();
	void deselect();

private:
	int m_width;
	int m_boxDistance;
	int m_boxseperation;
	int m_optionsAmount;
	int m_currChecked;
	sf::RectangleShape m_shape;
	sf::RectangleShape m_checkBox;
	sf::RectangleShape m_chackBoxOuter[3];
	Label m_label;
	void updateShape()override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Color m_fillColor;
	sf::Color m_outlineColor;
	virtual void onMouseEntered()override;
	virtual void onMouseLeft()override;
};

