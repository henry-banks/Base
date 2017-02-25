#pragma once

#include "Base.h"

#include "PlayerController.h"
#include "EnemyController.h"

using namespace base;


class Entity
{
public:
	ObjectPool<Transform>::iterator transform;
	ObjectPool<Rigidbody>::iterator rigidbody;
	ObjectPool<Collider>::iterator  collider;
	ObjectPool<Lifetime>::iterator lifetime;
	ObjectPool<Sprite>::iterator sprite;
	ObjectPool<Camera>::iterator camera;
	ObjectPool<Text>::iterator text;

	// example of a component in this project
	ObjectPool<PlayerController>::iterator player;
	ObjectPool<EnemyController>::iterator enemy;
	ObjectPool<Trigger>::iterator trigger;
	ObjectPool<Trigger>::iterator trigger2;	//I need 2

	void onFree()
	{
		transform.free();
		rigidbody.free();
		collider.free();
		lifetime.free();
		sprite.free();
		camera.free();
		text.free();

		player.free();
		enemy.free();
	}
};