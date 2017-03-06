#pragma once

#include "Base.h"
#include "Controller.h"
#include <iostream>

using namespace std;
using namespace base;

class PlayerController : public Controller
{
	//helper functions

	//Degub logging
	//void logPos(Transform *T) const { cout << T->getGlobalPosition().x << ", " << T->getGlobalPosition().y << endl; }

	void attack(Transform *T)
	{

	}


public:

	float attLen = .1f;
	float attTimer = -attLen;
	float attCooldown = .2f;
	bool canAtt = true;

	int life = 20;
	int score = 0;

	void poll(base::Transform *T, base::Rigidbody *rb, base::Trigger *trR, base::Trigger *trL, float dt)
	{
		if (sfw::getKey('W') && !isGravityEnabled)
		{
			rb->addImpulse(T->getGlobalUp() * jumpForce);
			logPos(T);
		}

		if (sfw::getKey('A') && rb->velocity.x > -maxSpeed)
			move(T, rb, false, dt);
		if (sfw::getKey('D') && rb->velocity.x < maxSpeed)
			move(T, rb, true, dt);

		if (!sfw::getKey('A') && !sfw::getKey('D'))
			rb->velocity.x = 0.f;

		//attack
		if (sfw::getMouseButton(0) && canAtt)
		{
			attTimer = attLen;
			isRight ? trR->isActive = true : trL->isActive = true;
		}

		if (attTimer > -attCooldown)
		{
			if(canAtt) canAtt = false;
			if (attTimer <= 0)
			{
				trL->isActive = false;
				trR->isActive = false;
			}
			attTimer -= dt;
		}
		else canAtt = true;

		//I'm using pseudo-collision because boundaries are broken and this is fast.

		//floor pseudo-collision; turn off gravity
		pollGrav(T, rb, dt);

		//floor pseudo-collision
		pollWall(T, rb, dt);
	}

};