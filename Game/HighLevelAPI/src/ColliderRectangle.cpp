//==================================================================-
/*
/file   ColliderRectangle.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class ColliderRectangle, as specified in specification
file ColliderRectangle.h.

*/
//==================================================================-

#include "stdafx.h"
#include "ColliderRectangle.h"
#include <Parser.h>

#include "Intersection2D.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "Graphics.h"
#include "Transform.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for rectangle collider.
// Params:
//   extents = The rectangle's extents (half-width, half-height).
ColliderRectangle::ColliderRectangle(const Vector2D& extents)
	: Collider(ColliderType::ColliderTypeRectangle), extents(extents)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderRectangle::Clone() const
{
	return new ColliderRectangle(*this);
}

// Saves object data to a file.
void ColliderRectangle::Serialize(Parser& parser) const
{
	parser.WriteVariable("extents", extents);
}

// Loads object data from a file.
void ColliderRectangle::Deserialize(Parser& parser)
{
	parser.ReadVariable("extents", extents);
}

// Debug drawing for colliders.
void ColliderRectangle::Draw()
{
	DebugDraw::GetInstance().AddRectangle(transform->GetTranslation(), extents,
		Graphics::GetInstance().GetDefaultCamera(), Colors::Green);
}

// Get the rectangle collider's extents (half-width, half-height).
// Returns:
//	 The extents of the rectangle.
const Vector2D& ColliderRectangle::GetExtents() const
{
	return extents;
}

// Set the rectangle collider's extents (half-width, half-height).
// Params:
//   extents = The new extents of the rectangle.
void ColliderRectangle::SetExtents(const Vector2D& extents_)
{
	extents.x = std::abs( extents_.x );
	extents.y = std::abs(extents_.y);
}

// Check for collision between a circle and another arbitrary collider.
// Params:
//	 other = Reference to the second circle collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderRectangle::IsCollidingWith(const Collider& other) const
{
	//point collision
	if (other.GetType() == ColliderType::ColliderTypePoint)
	{
		return other.IsCollidingWith(*this);
	}
	//circle collision
	else if (other.GetType() == ColliderType::ColliderTypeCircle)
	{
		const ColliderCircle& circle = static_cast<const ColliderCircle&>(other);
		Transform* otherTransform = static_cast<Transform*>(circle.GetOwner()->GetComponent("Transform"));
		return RectangleCircleIntersection(
			BoundingRectangle(transform->GetTranslation(), Vector2D(transform->GetScale() * 0.5f)),
			Circle(otherTransform->GetTranslation(), circle.GetRadius() ));
	}
	//rectangle collision
	else if (other.GetType() == ColliderType::ColliderTypeRectangle)
	{
		const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
		Transform* otherTransform = static_cast<Transform*>(rect.GetOwner()->GetComponent("Transform"));
		return RectangleRectangleIntersection(
			BoundingRectangle( transform->GetTranslation(), Vector2D(transform->GetScale() * 0.5f) ),
			BoundingRectangle( otherTransform->GetTranslation(), Vector2D(otherTransform->GetScale() * 0.5f) ));
	}
	//line collision
	else if (other.GetType() == ColliderType::ColliderTypeLines)
	{
		return other.IsCollidingWith(*this);
	}

	return false;
}