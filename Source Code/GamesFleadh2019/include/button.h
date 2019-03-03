#pragma once
#include "Widget.h"
class button : public Widget
{
public:
	//using FuncType = std::function<void(const sf::Event& event, Button& self)>;
	//static FuncType defaultFunc;
	button(Widget* parent = nullptr);
	virtual ~button();
	//FuncType onClick;
protected:
	virtual bool processEvent(const sf::Event& event, const sf::Vector2f& parent_pos) override;
	virtual void onMouseEntered();
	virtual void onMouseLeft();
private:
	enum Status { None = 0, Hover = 1 };
	int m_status;
};