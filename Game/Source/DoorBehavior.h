//------------------------------------------------------------------------------
//
// File Name:	DoorBehavior.h
// Author(s):	Nicole Smith
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Sprite;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class DoorBehavior : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   normalColor = The color that the object reverts to when not colliding.
		//   collidedColor = The color that the object changes to when colliding.
		//   collidedColorTime = The amount of time the object will retain the collided color.
		DoorBehavior(float doorOpenTime = 0.25f, float doorCloseTime = 0.5f);

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		void TransportToLevel();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Collision handler for ColorChange objects.
		// Params:
		//   object = The first object.
		//   other  = The other object the first object is colliding with.
		friend void DoorCollisionHandler(GameObject& object, GameObject& other);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Sprite* sprite;

		// Properties
		float doorOpenTime;
		float doorCloseTime;

		// Other variables
		float timer;
		bool open;
	};
}
