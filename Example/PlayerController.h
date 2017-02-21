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
	void logPos(Transform *T) const { cout << T->getGlobalPosition().x << ", " << T->getGlobalPosition().y << endl; }

	void attack(Transform *T)
	{

	}

public:

	void poll(base::Transform *T, base::Rigidbody *rb, float dt)
	{
		if (sfw::getKey('W') && !isGravityEnabled)
		{
			rb->addImpulse(T->getGlobalUp() * jumpForce);
			logPos(T);
		}

		if (sfw::getKey('A') && rb->velocity.x > -maxSpeed)	move(rb, false);
		if (sfw::getKey('D') && rb->velocity.x < maxSpeed)	move(rb, true);

		if (!sfw::getKey('A') && !sfw::getKey('D'))
			rb->velocity.x = 0.f;

		shotTimer -= dt;
		if (sfw::getKey(' ') && shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = 0.86f;
		}
		else shotRequest = false;

		//I'm using pseudo-collision because boundaries are broken and this is fast.

		//floor pseudo-collision; turn off gravity
		pollGrav(T, rb, dt);

		//floor pseudo-collision
		pollWall(T, rb, dt);
	}

};