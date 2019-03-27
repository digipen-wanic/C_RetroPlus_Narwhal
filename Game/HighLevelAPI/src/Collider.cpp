//==================================================================-
/*
/file   Collider.cpp
/author Jakob McFarland
/date   11/28/2018
/brief

This is the implementation file for all member functions
of the class Collider, as specified in specification
file Collider.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Collider.h"

#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"


MapCollision::MapCollision(bool bottom, bool top, bool left, bool right)
	: bottom(bottom), top(top), left(left), right(right) {}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new collider component.
// Params:
//   owner = Reference to the object that owns this component.
Collider::Collider(ColliderType type)
	: Component("Collider"), type(type), handler(nullptr), mapHandler(nullptr)
{
}

// Set component pointers.
void Collider::Initialize()
{
	transform = static_cast<Transform*>( GetOwner()->GetComponent("Transform") );
	physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics"));
}

// Check if two objects are colliding and send collision events.
// Params:
//	 other = Reference to the second collider component.
void Collider::CheckCollision(const Collider& other)
{
	if (IsCollidingWith(other) || other.IsCollidingWith(*this))
	{
		if (handler != nullptr)
		{
			handler(*GetOwner(), *other.GetOwner());		
		}

		if (other.handler != nullptr)
		{
			other.handler(*other.GetOwner(), *GetOwner());
		}
	}
}

// Get the type of this component.
ColliderType Collider::GetType() const
{
	return type;
}

// Sets the collision handler function for the collider.
// Params:
//   handler = A pointer to the collision handler function.
void Collider::SetCollisionHandler(CollisionEventHandler handler_)
{
	handler = handler_;
}

// Sets the map collision handler function for the collider.
// Params:
//   handler = A pointer to the collision handler function.
void Collider::SetMapCollisionHandler(MapCollisionEventHandler mapHandler_)
{
	mapHandler = mapHandler_;
}

// Get the map collision handler function pointer.
MapCollisionEventHandler Collider::GetMapCollisionHandler() const
{
	return mapHandler;
}
