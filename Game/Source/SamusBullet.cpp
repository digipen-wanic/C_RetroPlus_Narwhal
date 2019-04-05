//==================================================================-
/*
/file   SamusBullet.cpp
/author Jakob McFarland
/date   3/31/2019
/brief

This is the implementation file for all member functions
of the class SamusBullet, as specified in specification
file SamusBullet.h.

*/
//==================================================================-

#include "stdafx.h"
#include "SamusBullet.h"
#include "Health.h"

#include <Space.h>
#include <GameObject.h>
#include "Transform.h"
#include <Animation.h>
#include <Parser.h>
#include <Engine.h>
#include <ColliderCircle.h>

namespace Behaviors
{
	//==================================================================-
	// Public Functions:
	//==================================================================-

	void CollisionHandlerBullet(GameObject& bullet, GameObject& other);
	void BulletMapCollisionHandler(GameObject& object,
		const MapCollision& collision);

	// Default constructor
	SamusBullet::SamusBullet()
		: Component("SamusBullet")
	{
	}

	// Initialize this component (happens at object creation).
	void SamusBullet::Initialize()
	{
		//register our collision event
		ColliderCircle* collider = GetOwner()->GetComponent<ColliderCircle>();
		if (collider != nullptr)
		{
			collider->SetCollisionHandler(CollisionHandlerBullet);
			collider->SetMapCollisionHandler(BulletMapCollisionHandler);
		}
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* SamusBullet::Clone() const
	{
		return new SamusBullet(*this);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void SamusBullet::Serialize(Parser& parser) const
	{
		UNREFERENCED_PARAMETER(parser);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void SamusBullet::Deserialize(Parser& parser)
	{
		UNREFERENCED_PARAMETER(parser);
	}

	// Set the object that spawned this object
	void SamusBullet::SetSpawner(PlayerController* player_)
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
		if (other.GetName() == "Bat" || other.GetName() == "Crawler")
		{
			//bullet.GetComponent<SamusBullet>()->player->IncreaseScore( other.GetComponent<Asteroid>()->GetPointValue());

			bullet.Destroy();

			Health* otherHealth = other.GetComponent<Health>();

			if (otherHealth->adjustHealth(-5.0f))
			{
				GameObject* healthArchetype = new GameObject( *other.GetSpace()->GetObjectManager().GetArchetypeByName("HealthPickup") );
				healthArchetype->GetComponent<Transform>()->SetTranslation( other.GetComponent<Transform>()->GetTranslation());			
				other.GetSpace()->GetObjectManager().AddObject(*healthArchetype);
				healthArchetype->GetComponent<Animation>()->Play(0.05f, true);

				other.Destroy();
			}
		}
		if (other.GetName() == "Door")
			bullet.Destroy();
	}

	// Map collision handler for Player objects.
	// Params:
	//   object = The Player object.
	//   collision = Which sides the Player collided on.
	void BulletMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		UNREFERENCED_PARAMETER(collision);
		object.Destroy();
	}
}