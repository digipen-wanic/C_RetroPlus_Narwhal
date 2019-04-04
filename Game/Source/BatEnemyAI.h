#pragma once

//------------------------------------------------------------------------------
//
// File Name:	BatEnemyAI.h
// Author(s):	Jakob McFarland
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class
#include <Color.h>	 // Color
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "Tilemap.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Sprite;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	enum States
	{
		StateIdle,
		StateAttack,
		StateLanded
	};

	class BatEnemyAI : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		BatEnemyAI( Transform* playerTransform, float range, float attackSpeed, float deathTime);

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Collision handler for enemy.
		// Params:
		//   object = The enemy.
		//   other  = The object the monkey is colliding with.
		friend void BatCollisionHandler(GameObject& object, GameObject& other);

		// Map collision handler for enemy objects.
		// Params:
		//   object = The enemy object.
		//   collision = Which sides the enemy collided on.
		friend void BatMapCollisionHandler(GameObject& object,
			const MapCollision& collision);

		unsigned currentState;  // The current state of the enemy. Ex: idle, wander, chase

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		Transform* transform;
		Transform* playerTransform;
		Physics* physics;

		float range;
		float strafeSpeed;
		Vector2D attackVelocity;

		float deathTimer;
		float deathTime;
		//float timer; // Used for delaying state changes, etc.
		//float stateChangeTime; // Amount of time to wait before changing states.
	};
}