#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"

class MenuState : public BaseState
{
	Factory factory;
	//sprites
	unsigned b_normal, b_pressed, cursor, cursor_click, screen;
	ObjectPool<Entity>::iterator currentCamera;

	bool isMove;

public:

	virtual void init()
	{
		screen = sfw::loadTextureMap("../res/menu.png");
		b_normal = sfw::loadTextureMap("../res/button.png");
	}

	virtual void play()
	{
		factory.spawnStaticImage(screen, 640, 400, 1280, 800);

		currentCamera = factory.spawnCamera(0, 0, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 0, 0 });

		isMove = false;
	}

	virtual void step()
	{
		if (sfw::getKey('Q')) isMove = true;
	}

	virtual void draw()
	{
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		for each(auto &e in factory)
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);
	}

	virtual size_t next() const { return 0; }

	states next_a() const
	{
		if (isMove) return GAME_START;
		return MENU;
	}
};