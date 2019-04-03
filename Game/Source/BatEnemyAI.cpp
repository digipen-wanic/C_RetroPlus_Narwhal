//------------------------------------------------------------------------------
//
// File Name:	BatEnemyAI.cpp
// Author(s):	Jakob McFarland
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


#include "stdafx.h"
#include "BatEnemyAI.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "PlayerController.h"
#include "ColliderTilemap.h"

namespace Behaviors
{
	// Constructor
	BatEnemyAI::BatEnemyAI( Transform* playerTransform_, float range_, float attackSpeed, float deathTime_)
		: Component("Bat"), currentState( States::StateIdle ), playerTransform(playerTransform_),
		range(range_), attackVelocity( Vector2D(0.0f, -attackSpeed) ), deathTime(deathTime_), deathTimer(0)
	{
	}

	// Return a new copy of the component.
	Component* BatEnemyAI::Clone() const
	{

	}

	// Initialize data for this object.
	void BatEnemyAI::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void BatEnemyAI::Update(float dt)
	{
		if (currentState == States::StateIdle)
		{
			if ( std::abs(transform->GetTranslation().x - playerTransform->GetTranslation().x) < range )
			{
				currentState = States::StateAttack;

				physics->SetVelocity( attackVelocity );
			}
		}
		//else if (currentState == States::StateAttack)
		//{

		//}
		else if (currentState == States::StateLanded)
		{
			deathTimer += dt;

			if (deathTimer >= deathTime)
			{
				GetOwner()->Destroy();
			}
		}
	}

	// Collision handler for enemy.
	// Params:
	//   object = The enemy.
	//   other  = The object the monkey is colliding with.
	void EnemyCollisionHandler(GameObject& object, GameObject& other)
	{
		
	}

	// Map collision handler for enemy objects.
	// Params:
	//   object = The enemy object.
	//   collision = Which sides the enemy collided on.
	void EnemyMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		if ( collision.bottom )
		{
			object.GetComponent<BatEnemyAI>()->currentState = States::StateLanded;
		}
	}
}