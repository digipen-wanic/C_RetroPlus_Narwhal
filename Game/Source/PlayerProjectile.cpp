//==================================================================-
/*
/file   PlayerProjectile.cpp
/author Jakob McFarland
/date   10/15/2018
/brief

This is the implementation file for all member functions
of the class PlayerProjectile, as specified in specification
file PlayerProjectile.h.

*/
//==================================================================-

#include "stdafx.h"
#include "PlayerProjectile.h"
#include "PlayerShip.h"
#include "Asteroid.h"

#include <Space.h>
#include <Parser.h>
#include <Engine.h>
#include <ColliderCircle.h>

namespace Behaviors
{
	//==================================================================-
	// Public Functions:
	//==================================================================-

	void CollisionHandlerBullet(GameObject& bullet, GameObject& other);

	// Default constructor
	PlayerProjectile::PlayerProjectile()
		: Component("PlayerProjectile")
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerProjectile::Initialize()
	{
		//register our collision event
		ColliderCircle* collider = GetOwner()->GetComponent<ColliderCircle>();
		if (collider != nullptr) collider->SetCollisionHandler(CollisionHandlerBullet);
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerProjectile::Clone() const
	{
		return new PlayerProjectile(*this);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void PlayerProjectile::Serialize(Parser& parser) const
	{
		UNREFERENCED_PARAMETER(parser);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void PlayerProjectile::Deserialize(Parser& parser)
	{
		UNREFERENCED_PARAMETER(parser);
	}

	// Set the object that spawned this object
	void PlayerProjectile::SetSpawner(PlayerShip* player_)
	{
		player = player_;
	}

	//==================================================================-
	// Private Functions:
	//==================================================================-

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// The collision handling function for Asteroids.
	// Params:
	//	 bullet = The bullet object.
	//   other = The other object.
	void CollisionHandlerBullet(GameObject& bullet, GameObject& other)
	{
		if (other.GetName() == "Asteroid")
		{
			bullet.GetComponent<PlayerProjectile>()->player->IncreaseScore( other.GetComponent<Asteroid>()->GetPointValue());

			bullet.Destroy();
		}
	}
}