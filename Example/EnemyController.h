#pragma once
#include "Base.h"
#include "Controller.h"
#include "PlayerController.h"

using namespace base;

class EnemyController : public Controller
{

public:

	float shotTimer = 0.0f;
	bool shotRequest = false;

	void poll(Transform *T, Rigidbody *rb, PlayerController *pc, Transform *pT, float dt)
	{
		//shortcuts
		vec2 pos = T->getGlobalPosition(), pPos = pT->getGlobalPosition();
		int dist = 200;

		//move left (enemy should be within 10 pixels of character)
		if (pPos.x > pos.x + dist)
			move(T, rb, true);
		//move right (enemy should be within 10 pixels of character)
		if (pPos.x < pos.x - dist)
			move(T, rb, false);
		//stop if within range of player
		if (pPos.x > pos.x - dist && pPos.x < pos.x + dist)
			rb->velocity.x = 0;

		//Maybe don't use this?
		shotTimer -= dt;
		if (sfw::getKey(' ') && shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = 0.86f;
		}
		else shotRequest = false;


		//jump

		//floor pseudo-collision; turn off gravity
		pollGrav(T, rb, dt);
		
		//floor pseudo-collision
		pollWall(T, rb, dt);

		//logPos(T);
	}
};