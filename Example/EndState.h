#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"

class EndState : public BaseState
{
	Factory factory;
	//sprites
	unsigned b_normal, b_pressed, cursor, cursor_click, screen, font;
	ObjectPool<Entity>::iterator currentCamera;

	ObjectPool<Entity>::iterator scoreText, quitText;

	bool isMove;

public:

	virtual void init()
	{
		font = sfw::loadTextureMap("../res/font.png", 32, 4);
		screen = sfw::loadTextureMap("../res/ded.png");
		b_normal = sfw::loadTextureMap("../res/button.png");
	}

	virtual void play(int inScore)
	{
		factory.spawnStaticImage(screen, 640, 400, 1280, 800);

		currentCamera = factory.spawnCamera(0, 0, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 0, 0 });

		scoreText = factory.spawnText(font, "Score: " + std::to_string(inScore), vec2{ 400, 200 }, vec2{ 50,50 }, WHITE);
		quitText = factory.spawnText(font, "Press Q to quit.", vec2{ 400, 100 }, vec2{ 50,50 }, WHITE);

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
			if (e.transform && e.text)
				e.text->draw(&e.transform, cam);
	}

	virtual size_t next() const { return 0; }

	states next_a() const
	{
		if (isMove) return TERMINATE;
		return END;
	}
};