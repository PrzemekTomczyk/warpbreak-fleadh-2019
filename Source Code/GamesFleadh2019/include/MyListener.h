#pragma once
#include "Box2D/Dynamics/b2WorldCallbacks.h"
#include <iostream>
#include "Exit.h"

class MyListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		b2Body* body1 = fixtureA->GetBody();
		b2Body* body2 = fixtureB->GetBody();
		UserData* userData1 = static_cast<UserData*>(fixtureA->GetBody()->GetUserData());
		UserData* userData2 = static_cast<UserData*>(fixtureB->GetBody()->GetUserData());

		if (userData1->entity == 0)
		{
			handlePlayerBeginCollisions(contact, fixtureA, fixtureB);
		}
		if (userData2->entity == 0)
		{
			handlePlayerBeginCollisions(contact, fixtureB, fixtureA);
		}
		if (userData1->entity >= 1 && userData1->entity <= 2) {
			handleEnemyBeginCollisions(fixtureA, fixtureB);
		}
		if (userData2->entity >= 1 && userData2->entity <= 2) {
			handleEnemyBeginCollisions(fixtureB, fixtureA);
		}
		if (userData1->entity == 11)
		{
			handleOrbCollisions(fixtureA, fixtureB);
		}
		else if (userData2->entity == 11)
		{
			handleOrbCollisions(fixtureB, fixtureA);
		}

		if (userData1->entity == 3)
		{
			Bullet* bullet = static_cast<Bullet*>(userData1->data);
			bullet->kill();

			if (userData2->entity == 0)
			{
				Player* player = static_cast<Player*>(userData2->data);
				player->setAlive(false);
			}
		}
		else if (userData2->entity == 3)
		{
			Bullet* bullet = static_cast<Bullet*>(userData2->data);
			bullet->kill();

			if (userData1->entity == 0)
			{
				Player* player = static_cast<Player*>(userData1->data);
				player->setAlive(false);
			}
		}
	}
	void EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		b2Body* body1 = fixtureA->GetBody();
		b2Body* body2 = fixtureB->GetBody();
		auto data1 = body1->GetUserData();
		auto data2 = body1->GetUserData();
		UserData* userData1 = static_cast<UserData*>(body1->GetUserData());
		UserData* userData2 = static_cast<UserData*>(body2->GetUserData());

		if (userData1->entity == 0)
		{
			handlePlayerEndCollisions(contact, fixtureA, userData2);
		}
		if (userData2->entity == 0)
		{
			handlePlayerEndCollisions(contact, fixtureB, userData1);
		}
	}

	void handlePlayerBeginCollisions(b2Contact* contact, b2Fixture* playerFixture, b2Fixture* otherFixture)
	{
		UserData* playerData = static_cast<UserData*>(playerFixture->GetBody()->GetUserData());
		UserData* otherData = static_cast<UserData*>(otherFixture->GetBody()->GetUserData());
		Player* player = static_cast<Player*>(playerData->data);
 		if (!playerFixture->IsSensor() && (otherData->entity == 1 || otherData->entity == 2))
		{
			player->setAlive(false);
		}
		else if (otherData->entity == 12)
		{
			Exit* exit = static_cast<Exit*>(otherData->data);

			exit->hit();
			std::cout << "Hit exit" << std::endl;
		}
		if (contact->GetManifold()->localNormal.y > 0 && otherData->entity == 10)
		{
			player->setAirborn(false);
			player->playLanding(); // when landing on a surface
		}
		else if (contact->GetManifold()->localNormal.y < 0 && otherData->entity == 10)
		{
			std::cout << "Hit ceiling" << std::endl;
		}
	}

	void handlePlayerEndCollisions(b2Contact* contact, b2Fixture* playerFixture, UserData* otherData)
	{
		UserData* playerData = static_cast<UserData*>(playerFixture->GetBody()->GetUserData());
		Player* player = static_cast<Player*>(playerData->data);
		if (contact->GetManifold()->localNormal.y > 0 && !playerFixture->IsSensor())
		{
			player->setAirborn(true);
		}
	}

	void handleEnemyBeginCollisions(b2Fixture* enemyFixture, b2Fixture* otherFixture) {
		UserData* enemyData = static_cast<UserData*>(enemyFixture->GetBody()->GetUserData());
		UserData* otherData = static_cast<UserData*>(otherFixture->GetBody()->GetUserData());
		if (otherData->entity == 12)
		{
			Enemy* enemy = static_cast<Enemy*>(enemyData->data);
			std::cout << "Enemy hit door" << std::endl;
			enemy->kill();
		}
	}

	void handleOrbCollisions(b2Fixture* orbFixture, b2Fixture* otherFixture) {
		UserData* orbData = static_cast<UserData*>(orbFixture->GetBody()->GetUserData());
		UserData* otherData = static_cast<UserData*>(otherFixture->GetBody()->GetUserData());

		if (otherData->entity != 0)
		{
			Orb* orb = static_cast<Orb*>(orbData->data);
			orb->playCollision();
		}
	}
};

