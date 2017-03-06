#pragma once


class BaseState
{

public:
	virtual void init() {} // called once, on application startup.
	virtual void term() {} // called once, on shutdown.

	virtual void play() {} // called on enter transition
	virtual void stop() {} // called on exit transition

	virtual size_t next() const = 0; // called every frame
	
	virtual void step() {} // called every frame
	virtual void draw() {} // called every frame
};

enum states
{
	MENU_START,
	MENU,
	OPTIONS_START,
	OPTIONS,
	GAME_START,
	GAME,
	END_START,
	END,
	TERMINATE
};