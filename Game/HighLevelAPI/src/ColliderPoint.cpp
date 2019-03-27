//==================================================================-
/*
/file   ColliderPoint.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class ColliderPoint, as specified in specification
file ColliderPoint.h.

*/
//==================================================================-

#include "stdafx.h"
#include "ColliderPoint.h"
#include <Parser.h>

#include "Intersection2D.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "Graphics.h"
#include "Transform.h"
#include "ColliderCircle.h"
#include "ColliderRectangle.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for point collider.
ColliderPoint::ColliderPoint()
	: Collider(ColliderType::ColliderTypePoint)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderPoint::Clone() const
{
	return new ColliderPoint(*this);
}

// Saves object data to a file.
void ColliderPoint::Serialize(Parser& parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

// Loads object data from a file.
void ColliderPoint::Deserialize(Parser& parser)
{
	UNREFERENCED_PARAMETER(parser);
}

// Debug drawing for colliders.
void ColliderPoint::Draw()
{	
	DebugDraw::GetInstance().AddCircle(transform->GetTranslation(), 1.0f,
		Graphics::GetInstance().GetDefaultCamera(), Colors::Green);
}

// Check for collision between a circle and another arbitrary collider.
// Params:
//	 other = Reference to the second circle collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderPoint::IsCollidingWith(const Collider& other) const
{
	if (other.GetType() == ColliderType::ColliderTypePoint)
	{
		const ColliderPoint& point = static_cast<const ColliderPoint&>(other);
		const Vector2D& otherTranslation = static_cast<Transform*>(point.GetOwner()->GetComponent("Transform"))->GetTranslation();
		const Vector2D& translation = transform->GetTranslation();

		return translation.x == otherTranslation.x && translation.y == otherTranslation.y;
	}
	else if (other.GetType() == ColliderType::ColliderTypeCircle)
	{
		const ColliderCircle& otherCircle = static_cast<const ColliderCircle&>(other);
		Transform* otherTransform = static_cast<Transform*>(otherCircle.GetOwner()->GetComponent("Transform"));
		return PointCircleIntersection(transform->GetTranslation(), Circle(otherTransform->GetTranslation(), otherCircle.GetRadius()));
	}
	else if (other.GetType() == ColliderType::ColliderTypeRectangle)
	{
		const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
		Transform* otherTransform = static_cast<Transform*>(rect.GetOwner()->GetComponent("Transform"));
		return PointRectangleIntersection(
			transform->GetTranslation(), BoundingRectangle(otherTransform->GetTranslation(), Vector2D(otherTransform->GetScale() * 0.5f) ));
	}
	//line collision
	else if (other.GetType() == ColliderType::ColliderTypeLines)
	{
		return other.IsCollidingWith(*this);
	}

	return false;
}