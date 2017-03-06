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
			 spr_char, spr_char_flip, spr_enemy, spr_enemy_flip, spr_back,
			 spr_slash, spr_slash_flip, spr_blank, spr_laser;
	ObjectPool<Entity>::iterator currentCamera, enemyTimer;
	ObjectPool<Entity>::iterator player;
	ObjectPool<Entity>::iterator slashSprite;
	ObjectPool<Entity>::iterator lifeText, scoreText;

	ObjectPool<Entity> enemies;
	ObjectPool<Entity> bullets;

	int enemyCount = 0, enemyMax = 100;

public:
	GameState(size_t size = 512) : enemies(size), bullets(size)
	{}

	virtual void init()
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet.png");
		spr_space = sfw::loadTextureMap("../res/space.jpg");
		spr_ship = sfw::loadTextureMap("../res/ship.png");
		spr_roid = sfw::loadTextureMap("../res/rock.png");
		spr_font = sfw::loadTextureMap("../res/font.png",32,4);

		spr_char = sfw::loadTextureMap("../res/tux_dude.png");
		spr_char_flip = sfw::loadTextureMap("../res/tux_dude_flip.png");
		spr_enemy = sfw::loadTextureMap("../res/storm.png");
		spr_enemy_flip = sfw::loadTextureMap("../res/storm_flip.png");
		spr_back = sfw::loadTextureMap("../res/ruins.png");

		spr_blank = sfw::loadTextureMap("../res/blank.png");
		spr_slash = sfw::loadTextureMap("../res/swish.png");
		spr_slash_flip = sfw::loadTextureMap("../res/swish_flip.png");
		spr_laser = sfw::loadTextureMap("../res/laser.png");
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
		slashSprite = factory.spawnStaticImage(spr_blank, player->transform->getGlobalPosition().x, player->transform->getGlobalPosition().y, 105, 115);
		
		enemyTimer = factory.spawnTimer(2.f, "enemyTimer");
		
		lifeText = factory.spawnText(spr_font, "Life: " + std::to_string(player->player->life), vec2{ 25, 780 }, vec2{ 50,50 });
		scoreText = factory.spawnText(spr_font, "Score: " + std::to_string(player->player->score), vec2{ 625, 780 }, vec2{ 50,50 });

	}

	virtual void stop()
	{
		for each(auto e in enemies)
			e.onFree();
		for each(auto e in bullets)
			e.onFree();

		player->onFree();
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
							(base::BoundsTest(&e.transform, &e.trigger2, &it->transform, &it->trigger)))
						{
							e.player->score++;
							enemyCount--;
							//Update text
							scoreText->text->setString(("Score: " + std::to_string(player->player->score)).c_str());
							//The triggers are boxes so a bounding box test will do
							it->onFree();
							it.free();
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
					if(e.enemy->isRight)
						bullets.push(*factory.spawnBullet(spr_laser, e.transform->getGlobalPosition() + vec2{20,0} +e.transform->getGlobalUp() * 48,
									 vec2{ 50,20 }, e.transform->getGlobalAngle(), 200, 1, e.enemy->isRight));
					else
						bullets.push(*factory.spawnBullet(spr_laser, e.transform->getGlobalPosition() - vec2{ 20,0 } +e.transform->getGlobalUp() * 48,
									 vec2{ 50,20 }, e.transform->getGlobalAngle(), 200, 1, e.enemy->isRight));
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

		//Update slash sprite because it's special
		if (player->trigger->isActive || player->trigger2->isActive)
		{
			float x = player->transform->getGlobalPosition().x;
			float y = player->transform->getGlobalPosition().y;
			float xOffset = 65;
			float yOffset = 45;

			if (player->trigger->isActive)
			{
				x += xOffset;
				y += yOffset;
				slashSprite->transform->setLocalPosition(vec2{ x, y });
				slashSprite->sprite->sprite_id = spr_slash;
			}
			else if (player->trigger2->isActive)
			{
				x -= xOffset;
				y += yOffset;
				slashSprite->transform->setLocalPosition(vec2{ x, y });
				slashSprite->sprite->sprite_id = spr_slash_flip;
			}
		}

		if (!player->trigger->isActive && !player->trigger2->isActive)
			slashSprite->sprite->sprite_id = spr_blank;

		//Check if player is hit
		for (auto it = bullets.begin(); it != bullets.end();)
		{
			//The triggers are boxes so a bounding box test will do
			if (base::BoundsTest(&player->transform, &player->collider, &it->transform, &it->trigger))
			{
				player->player->life--;
				//Update text
				lifeText->text->setString(("Life: " + std::to_string(player->player->life)).c_str());
								
				it->onFree();
				it.free();
				continue;
			}
			it++;
		}

		//Make the spawn timer progressively shorter
		if(enemyTimer->timer->lifespan > 1)
			enemyTimer->timer->lifespan -= dt/50;

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
		//WHY are there a BUNCH of for loops?

		// kind of round about, but this is the camera matrix from the factory's current camera
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		// draw sprites
		for each(auto &e in factory)
		{
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);

			if (e.transform && e.text)
				e.text->draw(&e.transform, cam);
		}



#ifdef _DEBUG
		for each(auto &e in factory)
		{
			if (e.transform)
				e.transform->draw(cam);

			if (e.transform && e.collider)
				e.collider->draw(&e.transform, cam);

			if (e.transform && e.rigidbody)
				e.rigidbody->draw(&e.transform, cam);

			if (e.transform && e.trigger && e.trigger->isActive)
				e.trigger->draw(&e.transform, cam);
			if (e.transform && e.trigger2 && e.trigger2->isActive)
				e.trigger2->draw(&e.transform, cam);

			if (e.transform && e.trigger && !e.player)
				e.trigger->draw(&e.transform, cam);
		}
			

		/*for each(auto &e in factory)
			

		for each(auto &e in factory)
			

		for each(auto &e in factory)
		{
			
		}

		for each(auto &e in factory)*/
			
			
				

		//std::cout << currentCamera->transform->getLocalPosition().x << ", " << currentCamera->transform->getLocalPosition().x << endl;
		//cout << enemy1->transform->getGlobalPosition().x << ", " << enemy1->transform->getGlobalPosition().y << endl;
		//cout << player->transform->getLocalScale().y << ", " << player->transform->getGlobalUp().y << endl;
		cout << enemyTimer->timer->lifespan << endl;
#endif
	}

	states next_a() const
	{
		if (player->player->life <= 0) return TERMINATE;
		return GAME;
	}

	//For end state purposes
	int getScore() const { return player->player->score; }

};