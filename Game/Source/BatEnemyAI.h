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
		State
	};

	class BatEnemyAI : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		BatEnemyAI();

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Sets the next state of the enemy.
		// Params:
		//   nextState = The next state the enemy should be in.
		void SetState(unsigned nextState);

		// Collision handler for enemy.
		// Params:
		//   object = The enemy.
		//   other  = The object the monkey is colliding with.
		friend void EnemyCollisionHandler(GameObject& object, GameObject& other);

		// Map collision handler for Monkey objects.
		// Params:
		//   object = The monkey object.
		//   collision = Which sides the monkey collided on.
		friend void EnemyMapCollisionHandler(GameObject& object,
			const MapCollision& collision);


	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		unsigned currentState;  // The current state of the enemy. Ex: idle, wander, chase
		unsigned nextState;		// The next state of the enemy.
		unsigned innerState; // The inner state of the current state. Ex: enter, update, exit
		Transform* transform;
		Transform* playerTransform;
		Physics* physics;
		//float timer; // Used for delaying state changes, etc.
		//float stateChangeTime; // Amount of time to wait before changing states.
	};
}