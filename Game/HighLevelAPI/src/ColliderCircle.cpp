//==================================================================-
/*
/file   ColliderCircle.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class ColliderCircle, as specified in specification
file ColliderCircle.h.

*/
//==================================================================-

#include "stdafx.h"
#include "ColliderCircle.h"

#include "Intersection2D.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "Graphics.h"
#include "Transform.h"
#include <Parser.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for circle collider.
// Params:
//   radius = The radius of the circle.
ColliderCircle::ColliderCircle(float radius)
	: Collider(ColliderType::ColliderTypeCircle), radius(radius)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderCircle::Clone() const
{
	return new ColliderCircle(*this);
}

// Saves object data to a file.
void ColliderCircle::Serialize(Parser& parser) const
{
	parser.WriteVariable("radius", radius);
}

// Loads object data from a file.
void ColliderCircle::Deserialize(Parser& parser)
{
	parser.ReadVariable("radius", radius);
}

// Debug drawing for colliders.
void ColliderCircle::Draw()
{
	DebugDraw::GetInstance().AddCircle(transform->GetTranslation(), radius,
		Graphics::GetInstance().GetDefaultCamera(), Colors::Green);
}

// Get the circle collider's radius.
// Returns:
//	 The circle collider's radius.
float ColliderCircle::GetRadius() const
{
	return radius;
}

// Set the circle collider's radius.
// Params:
//   radius = The circle collider's new radius.
void ColliderCircle::SetRadius(float radius_)
{
	radius = radius_;
}

// Check for collision between a circle and another arbitrary collider.
// Params:
//	 other = Reference to the second circle collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderCircle::IsCollidingWith(const Collider& other) const
{
	if (other.GetType() == ColliderType::ColliderTypePoint)
	{
		return other.IsCollidingWith(*this);
	}
	else if (other.GetType() == ColliderType::ColliderTypeCircle)
	{
		const ColliderCircle& circle = static_cast<const ColliderCircle&>(other);
		Transform* otherTransform = static_cast<Transform*>(circle.GetOwner()->GetComponent("Transform"));
		return CircleCircleIntersection(
			Circle(transform->GetTranslation(), radius),
			Circle(otherTransform->GetTranslation(), circle.GetRadius()));
	}
	else if (other.GetType() == ColliderType::ColliderTypeRectangle)
	{
		return other.IsCollidingWith(*this);
	}
	//line collision
	else if (other.GetType() == ColliderType::ColliderTypeLines)
	{
		return other.IsCollidingWith(*this);
	}

	return false;
}