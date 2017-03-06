#pragma once
#include "Base.h"
#include "Controller.h"
#include "PlayerController.h"

using namespace base;

class EnemyController : public Controller
{

public:

	float shotMax;
	float shotTimer = shotMax = 4.0f;
	bool shotRequest = false;

	EnemyController() : Controller()
	{
		maxSpeed /= 1.5;
	}

	void poll(Transform *T, Rigidbody *rb, PlayerController *pc, Transform *pT, float dt)
	{
		//shortcuts
		vec2 pos = T->getGlobalPosition(), pPos = pT->getGlobalPosition();
		int dist = 200;

		//move left (enemy should be within 10 pixels of character)
		if (pPos.x > pos.x + dist)
			move(T, rb, true, dt);
		//move right (enemy should be within 10 pixels of character)
		if (pPos.x < pos.x - dist)
			move(T, rb, false, dt);
		//stop if within range of player
		if (pPos.x > pos.x - dist && pPos.x < pos.x + dist)
			rb->velocity.x = 0;

		//Maybe don't use this?
		//No I'm using this.  It's fast.
		shotTimer -= dt;
		if (shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = shotMax;
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