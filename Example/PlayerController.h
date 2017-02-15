#pragma once

#include "Base.h"
#include <iostream>

using namespace std;
using namespace base;

class PlayerController
{
public:
	float speed = 100, maxSpeed = 100;
	float turnSpeed = 1, jumpForce = 500;
	float gravity = 1000;
	float gravMax = 10000;
	bool isGravityEnabled = true;

	float shotTimer = 0.0f;
	bool shotRequest = false;

	void poll(base::Transform *T, base::Rigidbody *rb, float dt)
	{
		if (sfw::getKey('W') && !isGravityEnabled)
		{
			rb->addImpulse(T->getGlobalUp() * jumpForce);
		}

		if (sfw::getKey('A'))
		{
			rb->addForce(vec2{ -speed,0 });
			//T->setLocalPosition(T->getLocalPosition() - vec2{ speed, 0 });
		}

		if (sfw::getKey('D'))
		{
			rb->addForce(vec2{ speed,0 });
			//T->setLocalPosition(T->getLocalPosition() + vec2{ speed, 0 });
		}
				

		shotTimer -= dt;
		if (sfw::getKey(' ') && shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = 0.86f;
		}
		else shotRequest = false;

		if (T->getGlobalPosition().y < 150 && isGravityEnabled)
		{
			isGravityEnabled = false;
			rb->velocity.y = 0;
		}
			
		if (T->getGlobalPosition().y > 150 && !isGravityEnabled)
		{
			isGravityEnabled = true;
			rb->velocity.y = 0;
		}
			
		//Player falls until gravity is turned off
		if (isGravityEnabled && rb->velocity.y > -(gravMax))
		{
			rb->addForce(-(T->getGlobalUp() * gravity));
		}
	}

};