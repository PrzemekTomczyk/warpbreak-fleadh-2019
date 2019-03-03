#pragma once

#include<SFML\Graphics.hpp>
#include "button.h"
#include "Label.h"

class CheckButton : public button
{
public:
	CheckButton(const std::string& text, sf::Font & font, int distance, bool checked, Widget* parent = nullptr);
	CheckButton(const std::string& text, sf::Font & font, int distance, bool checked, int width, Widget* parent = nullptr);
	~CheckButton();
	void setText(const std::string& text);
	void setCharacterSize(unsigned int size);
	void setTextColor(const sf::Color& color);
	void setFillColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	void setOutlineThickness(float thickness);
	virtual sf::Vector2f getSize()const override;
	void flip();
	void setChecked(bool c);
	bool getChecked();
	void select();
	void deselect();
private:
	int m_width;
	int m_boxDistance;
	bool m_check;
	sf::RectangleShape m_shape;
	sf::RectangleShape m_checkBox;
	sf::RectangleShape m_chackBoxOuter;
	Label m_label;
	void updateShape()override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Color m_fillColor;
	sf::Color m_outlineColor;
	virtual void onMouseEntered()override;
	virtual void onMouseLeft()override;
};

