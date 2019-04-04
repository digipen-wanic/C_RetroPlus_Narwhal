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

#include "Space.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "Animation.h"
#include "PlayerController.h"
#include "ColliderTilemap.h"

namespace Behaviors
{
	void BatCollisionHandler(GameObject& object, GameObject& other);
	void BatMapCollisionHandler(GameObject& object,
		const MapCollision& collision);

	// Constructor
	BatEnemyAI::BatEnemyAI( Transform* playerTransform_, float range_, float attackSpeed, float deathTime_)
		: Component("Bat"), currentState( States::StateIdle ), playerTransform(playerTransform_),
		range(range_), attackVelocity( Vector2D(6.0f, -attackSpeed) ), deathTime(deathTime_), deathTimer(0),
		strafeSpeed(4.0f)
	{
	}

	// Return a new copy of the component.
	Component* BatEnemyAI::Clone() const
	{
		return new BatEnemyAI(*this);
	}

	// Initialize data for this object.
	void BatEnemyAI::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

		Collider* collider = static_cast<Collider*>( GetOwner()->GetComponent("Collider") );

		collider->SetMapCollisionHandler(BatMapCollisionHandler);
		collider->SetCollisionHandler(BatCollisionHandler);
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

				GetOwner()->GetComponent<Animation>()->Play(0.07f, true);
			}
		}
		else if (currentState == States::StateAttack)
		{
			Vector2D pos = transform->GetTranslation();
			if (transform->GetTranslation().x - playerTransform->GetTranslation().x < 0.0f)
			{
				pos.x += strafeSpeed;
			}

			if (transform->GetTranslation().x - playerTransform->GetTranslation().x > 0.0f)
			{
				pos.x -= strafeSpeed;
			}

			transform->SetTranslation(pos);

		}
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
	void BatCollisionHandler(GameObject& object, GameObject& other)
	{
		UNREFERENCED_PARAMETER(object);
		UNREFERENCED_PARAMETER(other);
	}

	// Map collision handler for enemy objects.
	// Params:
	//   object = The enemy object.
	//   collision = Which sides the enemy collided on.
	void BatMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		if ( collision.bottom )
		{
			object.GetComponent<BatEnemyAI>()->currentState = States::StateLanded;
		}
	}
}