#include "button.h"



//Button::FuncType Button::defaultFunc = [](const sf::Event&, Button&)->void {};

button::button(Widget * parent) : Widget(parent), m_status(Status::None) //see pdf
{
}

button::~button()
{
}

bool button::processEvent(const sf::Event & event, const sf::Vector2f & parent_pos)
{
	bool res = false;
	if (event.type == sf::Event::MouseButtonReleased)
	{
		const sf::Vector2f pos = m_position + parent_pos;
		const sf::Vector2f size = getSize();
		sf::FloatRect rect;
		rect.left = pos.x;
		rect.top = pos.y;
		rect.width = size.x;
		rect.height = size.y;
		if (rect.contains(event.mouseButton.x, event.mouseButton.y))
		{
			//onClick(event, *this);
			res = true;
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		const sf::Vector2f pos = m_position + parent_pos;
		const sf::Vector2f size = getSize();
		sf::FloatRect rect;
		rect.left = pos.x;
		rect.top = pos.y;
		rect.width = size.x;
		rect.height = size.y;
		int old_status = m_status;
		m_status = Status::None;
		const sf::Vector2f mouse_pos(event.mouseMove.x, event.mouseMove.y);
		if (rect.contains(mouse_pos))
			m_status = Status::Hover;
		if ((old_status & Status::Hover) && !(m_status & Status::Hover))
			onMouseLeft();
		else if (!(old_status & Status::Hover) && (m_status & Status::Hover))
			onMouseEntered();
	}
	return res;
}

void button::onMouseEntered()
{

}

void button::onMouseLeft()
{
}
