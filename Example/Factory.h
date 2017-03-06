#pragma once

#include "Entity.h"

class Factory
{	
	ObjectPool<Entity>	  entities;
	
	// The factory will store an object pool for each component type
	ObjectPool<Transform>	transforms;
	ObjectPool<Rigidbody>	rigidbodies;
	ObjectPool<Collider>	colliders;
	ObjectPool<Sprite>		sprites;
	ObjectPool<Lifetime>	lifetimes;
	ObjectPool<Camera>		cameras;
	ObjectPool<Text>		texts;
	ObjectPool<PlayerController>	players;
	ObjectPool<EnemyController>		enemies;
	ObjectPool<Trigger>		triggers;
	ObjectPool<Timer>		timers;

public:

	// iterators to access the entity pool
	ObjectPool<Entity>::iterator begin() { return entities.begin(); }
	ObjectPool<Entity>::iterator end() { return entities.end(); }

	// for now, they're all the same size
	Factory(size_t size = 512)
								: entities(size), transforms(size), rigidbodies(size),
								  colliders(size), sprites(size), lifetimes(size),
								  cameras(size), players(size), texts(size), enemies(size),
								  triggers(size), timers(size)
	{
	}

	//general-purpose getter
	template<typename T>
	ObjectPool<T> getObjectPool(const std::string &name) const
	{
		switch (name)
		{
		case "entities":
			return entities; break;
		case "transforms":
			return transforms; break;
		case "rigidbodies":
			return rigidbodies; break;
		case "colliders":
			return colliders; break;
		case "sprites":
			return sprites; break;
		case "lifetimes":
			return lifetimes; break;
		case "cameras":
			return cameras; break;
		case "texts":
			return texts; break;
		case "players":
			return players; break;
		case "enemies":
			return enemies; break;
		case "triggers":
			return triggers; break;
		case "timers":
			return timers; break;
		default:
			return entities;
			break;
		}
	}

	// What follows are specialized spawning functions
	// just observe the steps taken and replicate for your own usages

	ObjectPool<Entity>::iterator spawnCamera(float w2, float h2, float zoom)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->camera = cameras.push();
		e->camera->offset = vec2{ w2,h2 };
		e->camera->scale = vec2{ zoom,zoom };

		return e;
	}

	ObjectPool<Entity>::iterator spawnStaticImage(unsigned sprite, float x, float y, float w, float h)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->sprite = sprites.push();
		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{w,h};
		e->transform->setLocalPosition(vec2{ x,y });	
		return e;
	}

	ObjectPool<Entity>::iterator spawnBullet(unsigned sprite, vec2 pos, vec2 dim, float ang, float impulse, unsigned faction, bool isRight)
	{
		auto e = entities.push();

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->lifetime = lifetimes.push();

		float x = 1.f;
		float y = .25f;
		float offsetX = 0.f;
		float offsetY = 0.f;
		vec2 v[4];
		v[0] = vec2{ x,  y};
		v[1] = vec2{-x,  y};
		v[2] = vec2{-x, -y};
		v[3] = vec2{ x, -y};
		e->trigger = triggers.push(Trigger(v,4,"bullet"));
		e->trigger->isActive = true;

		e->transform->setLocalPosition(pos);
		e->transform->setLocalScale(dim);
		e->transform->setLocalAngle(ang);

		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{2.f, .5f};

		if(isRight)
			e->rigidbody->addImpulse(vec2{ impulse, 0 });
		else
			e->rigidbody->addImpulse(vec2{ -impulse, 0 });

		e->lifetime->lifespan = 1.6f;
		
		return e;
	}

	ObjectPool<Entity>::iterator spawnPlayer(unsigned sprite, unsigned font)
	{
		auto e = entities.push();

		e->transform = transforms.push();
		e->transform->setGlobalPosition(vec2{ 520, 250 });
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();


		float x = .1f, y = .25f;
		vec2 v[4] = { vec2{ x, y }, vec2{ -x, y }, vec2{ -x, -y }, vec2{ x, -y } };
		e->collider = colliders.push(Collider(v, 4));

		x =.25f;
		y = .3f;
		float offsetX = 2 * x;
		float offsetY = .2f;
		v[0] = vec2{  x + offsetX,  y + offsetY };
		v[1] = vec2{ -x + offsetX,  y + offsetY };
		v[2] = vec2{ -x + offsetX, -y + offsetY };
		v[3] = vec2{  x + offsetX, -y + offsetY };
		e->trigger = triggers.push(Trigger(v, 4, "player_att"));
		e->trigger->isActive = false;
		v[0] = vec2{  x - offsetX,  y + offsetY };
		v[1] = vec2{ -x - offsetX,  y + offsetY };
		v[2] = vec2{ -x - offsetX, -y + offsetY };
		v[3] = vec2{  x - offsetX, -y + offsetY };
		e->trigger2 = triggers.push(Trigger(v, 4, "player_att"));
		e->trigger->isActive = false;

		e->player = players.push();
		e->text = texts.push();

		e->text->sprite_id = font;
		e->text->offset = vec2{ -24,-24 };
		e->text->off_scale = vec2{.5f,.5f};
		e->text->setString("");

		e->transform->setLocalScale(vec2{150,199});

		e->sprite->sprite_id = sprite;

		return e;
	}


	ObjectPool<Entity>::iterator spawnEnemy(unsigned sprite)
	{
		auto e = entities.push();

		//determines if the enemy spawns on the left or right side of the screen.
		vec2 newPos = ((rand() % 2) == 0) ? vec2{ 100, 200 } : vec2{ 1200, 200 };

		e->transform = transforms.push();
		e->transform->setGlobalPosition(newPos);
		e->transform->setLocalScale(vec2{ 110,199 });

		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();

		float x = .35f, y = .45f;
		vec2 v[4] = { vec2{ x, y }, vec2{ -x, y }, vec2{ -x, -y }, vec2{ x, -y } };
		e->trigger = triggers.push(Trigger(v, 4, "enemy"));
		e->trigger->isActive = true;
		e->enemy = enemies.push();
		e->sprite->sprite_id = sprite;

		return e;
	}


	ObjectPool<Entity>::iterator spawnAsteroid(unsigned sprite)
	{
		auto e = entities.push();

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->collider = colliders.push();

		e->transform->setLocalScale(vec2{ 48,48 });

		e->transform->setGlobalPosition(vec2::fromAngle(randRange(0, 360)*DEG2RAD)*(rand01() * 200 + 64));

		e->rigidbody->addSpin(rand01()*12-6);

		e->sprite->sprite_id = sprite;

		return e;
	}

	ObjectPool<Entity>::iterator spawnBlank(vec2 pos)
	{
		auto e = entities.push();

		e->transform = transforms.push();
		e->transform->setGlobalPosition(pos);

		return e;
	}

	ObjectPool<Entity>::iterator spawnTimer(float time, std::string name = "name")
	{
		auto e = entities.push();
		e->timer = timers.push(Timer(time, name));

		return e;
	}

	ObjectPool<Entity>::iterator spawnText(unsigned font, std::string text_a, vec2 pos, vec2 scl = {1,1}, unsigned tint = WHITE)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->transform->setGlobalPosition(pos);

		e->text = texts.push();
		e->text->sprite_id = font;
		e->text->setString(text_a.c_str());
		e->text->off_scale = scl;
		e->text->tint = tint;

		return e;
	}
};


