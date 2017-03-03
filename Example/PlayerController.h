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

	float sMax;
	float sTimer = sMax = 5.f;

public:

	float attLen = .1f;
	//Resets at -attLen, not 0 (for cooldown)
	float attTimer = -attLen;

	void poll(base::Transform *T, base::Rigidbody *rb, base::Trigger *trR, base::Trigger *trL, float dt)
	{
		if (sfw::getKey('W') && !isGravityEnabled)
		{
			rb->addImpulse(T->getGlobalUp() * jumpForce);
			logPos(T);
		}

		if (sfw::getKey('A') && rb->velocity.x > -maxSpeed)
		{
			move(T, rb, false);
			//tr->update(vec2{ -200, 0 });
		}
		if (sfw::getKey('D') && rb->velocity.x < maxSpeed)
		{
			move(T, rb, true);
			//tr->update(vec2{ 200, 0 });
		}
		if (sfw::getKey('S') && sTimer >= sMax)
		{
			//T->setLocalScale(vec2{ T->getLocalScale().x * -1,T->getLocalScale().y * -1 });
		}

		if (!sfw::getKey('A') && !sfw::getKey('D'))
			rb->velocity.x = 0.f;

		//attack
		if (sfw::getMouseButton(0) && attTimer <= -attLen)
		{
			attTimer = attLen;
			isRight ? trR->isActive = true : trL->isActive = true;
		}

		if (attTimer > -attLen)
		{
			if (attTimer <= 0)
			{
				trL->isActive = false;
				trR->isActive = false;
			}
			attTimer -= dt;
		}

		//I'm using pseudo-collision because boundaries are broken and this is fast.

		//floor pseudo-collision; turn off gravity
		pollGrav(T, rb, dt);

		//floor pseudo-collision
		pollWall(T, rb, dt);

		if (sTimer > 0)
			sTimer--;
		else
			sTimer = sMax;

		/*if(T->getGlobalUp().y < 0)
			T->setLocalScale(vec2{ T->getLocalScale().x,T->getLocalScale().y * -1 });
		if (T->getLocalScale().y < 0)
			T->setLocalScale(vec2{ T->getLocalScale().x * -1,T->getLocalScale().y });*/
	}

};