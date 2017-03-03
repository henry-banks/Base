#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"


/*
	The gamestate represents a discrete container of all that is 
	necessary to drive the game experience.

	A factory object is used to manage the creation and deletion of
	objects in the simulation.

	The game state updates the entities within the factory using
	a series of 'systems.'
*/

class GameState : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_ship, spr_bullet, spr_roid, spr_font,
			 spr_char, spr_char_flip, spr_enemy, spr_enemy_flip, spr_back;
	ObjectPool<Entity>::iterator currentCamera, enemyTimer;
	ObjectPool<Entity>::iterator player;

	ObjectPool<Entity> enemies;

	int enemyCount = 0, enemyMax = 100;

public:
	virtual void init()
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet.png");
		spr_space = sfw::loadTextureMap("../res/space.jpg");
		spr_ship = sfw::loadTextureMap("../res/ship.png");
		spr_roid = sfw::loadTextureMap("../res/rock.png");
		spr_font = sfw::loadTextureMap("../res/font.png",32,4);

		spr_char = sfw::loadTextureMap("../res/DEUS VULT.png");
		spr_char_flip = sfw::loadTextureMap("../res/DEUS VULT flip.png");
		spr_enemy = sfw::loadTextureMap("../res/pleg doctor.png");
		spr_enemy_flip = sfw::loadTextureMap("../res/pleg doctor flip.png");
		spr_back = sfw::loadTextureMap("../res/ruins.png");
	}

	virtual void play()
	{
		// delete any old entities sitting around
		for (auto it = factory.begin(); it != factory.end(); it->onFree(), it.free());

		// setup a default camera
		currentCamera = factory.spawnCamera(0, 0, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 0, 0 });

		// call some spawning functions!
		factory.spawnStaticImage(spr_back, 640, 400, 1280, 800);

		player = factory.spawnPlayer(spr_char, spr_font);

		//enemy1 = factory.spawnEnemy(spr_enemy);

		enemyTimer = factory.spawnTimer(2.f, "enemyTimer");

		//factory.spawnBoundary(100, -150, false);
		//factory.spawnBoundary(100, 100, true);
		//factory.spawnBoundary(1070, 10, true);

		//factory.spawnBlank(vec2{ 540,400 });
		

	}

	virtual void stop()
	{

	}

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const { return 0; }


	// update loop, where 'systems' exist
	virtual void step()
	{

		float dt = sfw::getDeltaTime();

		//Update spawn timer
		if (enemyTimer->timer && !enemyTimer->timer->isAlive())
		{
			if (enemyCount < enemyMax)
			{
				enemies.push(*factory.spawnEnemy(spr_enemy));
				enemyCount++;
			}			
			enemyTimer->timer->reset();
		}
		

		// maybe spawn some asteroids here.

		for(auto it = factory.begin(); it != factory.end();) // no++!
		{
			bool del = false; // does this entity end up dying?
			auto &e = *it;    // convenience reference

			// rigidbody update
			if (e.transform && e.rigidbody)
				e.rigidbody->integrate(&e.transform, dt);

			// controller update
			if (e.transform && e.rigidbody && e.player && e.trigger)
			{
				e.player->poll(&e.transform, &e.rigidbody, &e.trigger, &e.trigger2, dt);
				e.sprite->sprite_id = (e.player->isRight) ? spr_char : spr_char_flip;

				//ObjectPool<PlayerController>::iterator p = e.player; //shortcut

				//Attack
				if (e.trigger->isActive || e.trigger2->isActive)
				{
					for (auto it = enemies.begin(); it != enemies.end();)
					{
						//I've never done a ternary inside of an if statement before...
						if (e.trigger->isActive ? (base::BoundsTest(&e.transform, &e.trigger, &it->transform, &it->trigger)) :
							(base::BoundsTest(&e.transform, &e.trigger2, &it->transform, &it->trigger2)))
						{
							//The triggers are boxes so a bounding box test will do
							it->onFree();
							continue;
						}
						it++;
					}
				}
				
			}

			//enemy update
			if (e.transform && e.rigidbody && e.enemy)
			{
				e.enemy->poll(&e.transform, &e.rigidbody, &player->player, &player->transform, dt);
				e.sprite->sprite_id = (e.enemy->isRight) ? spr_enemy: spr_enemy_flip;
				e.rigidbody->velocity.x = 0;
				if (e.enemy->shotRequest) // controller requested a bullet fire
				{
					factory.spawnBullet(spr_bullet, e.transform->getGlobalPosition() + e.transform->getGlobalUp() * 48,
						vec2{ 32,32 }, e.transform->getGlobalAngle(), 200, 1);
				}
			}
			// lifetime decay update
			if (e.lifetime)
			{
				e.lifetime->age(dt);
				if (!e.lifetime->isAlive())
					del = true;
			}

			// timer decay update
			if (e.timer)
			{
				e.timer->age(dt);
				if (e.timer->name == "enemyTimer")
					enemyTimer = it;
			}

			if (sfw::getKey('E'))
			{
				factory.spawnEnemy(spr_enemy);
			}

			// ++ here, because free increments in case of deletions
			if (!del) it++;
			else
			{
				it->onFree();
				it.free();
			}
		}


		// Physics system!
		// You'll want to extend this with custom collision responses

		
		/*for (auto it = factory.begin(); it != factory.end(); it++) { // for each entity
			for (auto bit = it; bit != factory.end(); bit++) {	  // for every other entity
				if (it != bit && it->transform && it->collider && bit->transform && bit->collider)
					// if they aren't the same and they both have collidable bits...
				{
					// test their bounding boxes
					if (base::BoundsTest(&it->transform, &it->collider, &bit->transform, &bit->collider))
					{
						// if true, get the penetration and normal from the convex hulls
						auto cd = base::ColliderTest(&it->transform, &it->collider, &bit->transform, &bit->collider);

						// if there was a collision,
						if (cd.result())
						{
							// condition for dynamic resolution
							if (it->rigidbody && bit->rigidbody)
								base::DynamicResolution(cd, &it->transform, &it->rigidbody, &bit->transform, &bit->rigidbody);

							// condition for static resolution
							else if (it->rigidbody && !bit->rigidbody)
								base::StaticResolution(cd, &it->transform, &it->rigidbody);
						}
					}
				}
			}
		}*/
	}


	virtual void draw()	
	{
		// kind of round about, but this is the camera matrix from the factory's current camera
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		// draw sprites
		for each(auto &e in factory)
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);

		// draw text
		for each(auto &e in factory)
			if (e.transform && e.text)
				e.text->draw(&e.transform, cam);



#ifdef _DEBUG
		for each(auto &e in factory)
			if (e.transform)
				e.transform->draw(cam);

		for each(auto &e in factory)
			if (e.transform && e.collider)
				e.collider->draw(&e.transform, cam);

		for each(auto &e in factory)
			if (e.transform && e.rigidbody)
				e.rigidbody->draw(&e.transform, cam);

		for each(auto &e in factory)
		{
			if (e.transform && e.trigger && e.trigger->isActive)
			{
				e.trigger->draw(&e.transform, cam);
			}
			if (e.transform && e.trigger2 && e.trigger2->isActive)
				e.trigger2->draw(&e.transform, cam);
		}
			
				

		//std::cout << currentCamera->transform->getLocalPosition().x << ", " << currentCamera->transform->getLocalPosition().x << endl;
		//cout << enemy1->transform->getGlobalPosition().x << ", " << enemy1->transform->getGlobalPosition().y << endl;
		cout << player->transform->getLocalScale().y << ", " << player->transform->getGlobalUp().y << endl;
#endif
	}
};