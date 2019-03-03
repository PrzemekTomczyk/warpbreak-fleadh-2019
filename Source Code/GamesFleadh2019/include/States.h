#pragma once

enum Menu
{
	StateSplash,
	StateLicense,
	StateMainMenu,
	StateOptions,
	StateGameplay,
	StateEnd,
	StateCredits,
	StateQuit
};

struct Options
{
	int musicVol;
	int soundVol;
	bool mute;
	int animSpeed;
	int fontSize;
};