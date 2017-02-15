#pragma once

#include "Base.h"
#include <iostream>

using namespace std;
using namespace base;

class PlayerController
{
public:
	float speed = 10000, maxSpeed = 500;
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

		if (sfw::getKey('A') && rb->velocity.x > -maxSpeed)
		{
			rb->addForce(vec2{ -speed,0 });
			//rb->velocity.x = -speed;
			//T->setLocalPosition(T->getLocalPosition() - vec2{ speed, 0 });
		}

		if (sfw::getKey('D') && rb->velocity.x < maxSpeed)
		{
			rb->addForce(vec2{ speed,0 });
			//rb->velocity.x = speed;
			//T->setLocalPosition(T->getLocalPosition() + vec2{ speed, 0 });
		}

		if (!sfw::getKey('A') && !sfw::getKey('D'))
			rb->velocity.x = 0.f;

		if (rb->velocity.x > maxSpeed)
			rb->velocity.x = maxSpeed;
		else if(rb->velocity.x < -maxSpeed)
			rb->velocity.x = -maxSpeed;

		shotTimer -= dt;
		if (sfw::getKey(' ') && shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = 0.86f;
		}
		else shotRequest = false;

		//pseudo-collision because boundaries are broken and this is fast
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

		if (T->getGlobalPosition().x <= 0 || T-> getGlobalPosition().x >= 1280)
		{
			rb->velocity.x = -rb->velocity.x;
			T->setGlobalPosition(vec2{ (T->getGlobalPosition().x + (rb->velocity.x / 50)),  T->getGlobalPosition().y });
		}
			
		//Player falls until gravity is turned off
		if (isGravityEnabled && rb->velocity.y > -(gravMax))
		{
			rb->addForce(-(T->getGlobalUp() * gravity));
		}
	}

};