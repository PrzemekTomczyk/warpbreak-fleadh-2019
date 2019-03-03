#include <Input.h>

Input::Input() 
{
	m_previous = m_current;
}
Input::~Input() {}

void Input::update()
{
	m_previous = m_current;
}