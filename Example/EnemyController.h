#pragma once
#include "Base.h"
#include "Controller.h"
#include "PlayerController.h"

using namespace base;

class EnemyController : public Controller
{

public:

	void poll(Transform *T, Rigidbody *rb, PlayerController *pc, Transform *pT, float dt)
	{
		//shortcuts
		vec2 pos = T->getGlobalPosition(), pPos = pT->getGlobalPosition();
		int dist = 100;

		//move left (enemy should be within 10 pixels of character)
		if (pPos.x > pos.x + dist)
			move(rb, true);
		//move right (enemy should be within 10 pixels of character)
		if (pPos.x < pos.x - dist)
			move(rb, false);
		//stop if within range of player
		if (pPos.x > pos.x - dist && pPos.x < pos.x + dist)
			rb->velocity.x = 0;

		//jump

		//floor pseudo-collision; turn off gravity
		pollGrav(T, rb, dt);
		
		//floor pseudo-collision
		pollWall(T, rb, dt);

		logPos(T);
	}
};