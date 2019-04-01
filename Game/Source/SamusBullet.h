//------------------------------------------------------------------------------
//
// File Name:	SamusBullet.h
// Author(s):	Jakob McFarland
// date   3/31/2019
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

#include "PlayerController.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class SamusBullet : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor
		SamusBullet();

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Set the object that spawned this object
		void SetSpawner(PlayerController* player);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// The collision handling function for Asteroids.
		// Params:
		//	 bullet = The bullet object.
		//   other = The other object.
		friend void CollisionHandlerBullet(GameObject& bullet, GameObject& other);

		friend void BulletMapCollisionHandler(GameObject& object, const MapCollision& collision);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Who spawned this object?
		Behaviors::PlayerController* player;
	};
}

//------------------------------------------------------------------------------
