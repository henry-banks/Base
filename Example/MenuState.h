#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"

class MenuState : public BaseState
{
	Factory factory;
	//sprites
	unsigned b_normal, b_pressed, cursor, cursor_click, logo;

public:

	virtual void init()
	{

	}

	virtual void play()
	{
		
	}

	virtual void step()
	{

	}

	virtual void draw()
	{

	}

	virtual size_t next() const { return 0; }

	states next_a() const
	{
		return MENU;
	}
};