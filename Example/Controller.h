#pragma once

//Abstract base class for players and enemies to derive from

#include "Base.h"
#include <iostream>

using namespace std;
using namespace base;

#define FLOOR_Y 200

class Controller
{
protected:
	//Helper functions

	//Degub logging
	void logPos(Transform *T) const { cout << T->getGlobalPosition().x << ", " << T->getGlobalPosition().y << endl; }

	void move(Transform *T, Rigidbody *rb, bool isPos)
	{
	// vec2 dim = T->getLocalScale();
		// dim.x *= -1;
		// T->setLocalScale(dim);

		
		//collider->offset.x *= -1;
		if (isPos && rb->velocity.x < maxSpeed)
		{
			rb->addForce(vec2{ speed,0 });
			//if (!isRight)
			//{
			//	float dim = T->getLocalScale().x;
			//	float y = T->getLocalScale().y;
			//	dim *= -1;
			//	T->setLocalScale(vec2{ T->getLocalScale().x * -1,T->getLocalScale().y });
			//	//cout << T->getLocalScale().x << ", " << T->getLocalScale().y << endl;
			//}
			isRight = true;
		}

		if (!isPos && rb->velocity.x > -maxSpeed)
		{
			rb->addForce(vec2{ -speed,0 });
			//if (isRight)
			//{
			//	float dim = T->getLocalScale().x;
			//	float y = T->getLocalScale().y;
			//	dim *= -1;
			//	T->setLocalScale(vec2{ T->getLocalScale().x * -1,T->getLocalScale().y});
			//	//cout << T->getLocalScale().x << ", " << T->getLocalScale().y << endl;
			//}
			isRight = false;
		}
	}

	//Gravity stuff
	void pollGrav(base::Transform *T, base::Rigidbody *rb, float dt)
	{			

		//floor pseudo-collision; turn off gravity
		if (T->getGlobalPosition().y <= FLOOR_Y && isGravityEnabled)
		{
			isGravityEnabled = false;
			rb->velocity.y = 0;
			logPos(T);
		}
		//I subtract 10 because I want this to fire when the player is substantially below the floor
		if (T->getGlobalPosition().y < FLOOR_Y - 10)
		{
			T->setGlobalPosition(vec2{ T->getGlobalPosition().x, FLOOR_Y + 1 });
			logPos(T);
		}

		//Activate gravity when player is above the floor
		if (T->getGlobalPosition().y > FLOOR_Y && !isGravityEnabled)
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

	//Wall pseudo-collision
	void pollWall(base::Transform *T, base::Rigidbody *rb, float dt)
	{	
		if (T->getGlobalPosition().x <= 0)
		{
			rb->velocity.x = -rb->velocity.x;
			T->setGlobalPosition(vec2{ 10,  T->getGlobalPosition().y });
		}
		if (T->getGlobalPosition().x >= 1280)
		{
			rb->velocity.x = -rb->velocity.x;
			T->setGlobalPosition(vec2{ 1270,  T->getGlobalPosition().y });
		}
	}

	virtual void attack() {}

public:

	float speed = 10000, maxSpeed = 400;
	float turnSpeed = 1, jumpForce = 700;
	float gravity = 1000;
	float gravMax = 10000;
	bool isGravityEnabled = true;
	bool isRight = true;	//true if player is facing right
	bool isAttack = false;

};