#ifndef INPUT_H
#define INPUT_H

#include <iostream>

struct InputState
{
	bool W{ false };
	bool A{ false };
	bool S{ false };
	bool D{ false };
	bool E{ false };
	bool Q{ false };
	bool F{ false };
	bool Space{ false };
	bool Return{ false };
	bool one{ false };
	bool two{ false };
	bool three{ false };
	bool four{ false };
	bool five{ false };
	bool six{ false };
};

class Input
{
public:
	Input();
	~Input();

	InputState m_current;
	InputState m_previous;

	void update();
};
#endif