//==================================================================-
/*
/file   ColliderTilemap.cpp
/author Jakob McFarland
/date   12/4/2018
/brief

This is the implementation file for all member functions
of the class ColliderTilemap as specified in specification
file ColliderTilemap.h.

*/
//==================================================================-

#include "stdafx.h"
#include "ColliderTilemap.h"
#include <Parser.h>

#include "GameObject.h"
#include "TileMap.h"
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"
#include "ColliderRectangle.h"
#include "DebugDraw.h"
#include "Matrix2DStudent.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for Tilemap collider.
ColliderTilemap::ColliderTilemap()
	: Collider(ColliderType::ColliderTypeTilemap)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderTilemap::Clone() const
{
	return new ColliderTilemap(*this);
}

// Saves object data to a file.
void ColliderTilemap::Serialize(Parser& parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

// Loads object data from a file.
void ColliderTilemap::Deserialize(Parser& parser)
{
	UNREFERENCED_PARAMETER(parser);
}

// Debug drawing for colliders.
void ColliderTilemap::Draw()
{
	//Stub
}

// Check for collision between a tilemap and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderTilemap::IsCollidingWith(const Collider& other) const
{
	//rectangle collision
	if (other.GetType() == ColliderType::ColliderTypeRectangle)
	{
		const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
		Transform* otherTransform = static_cast<Transform*>(other.GetOwner()->GetComponent("Transform"));
		Physics* otherPhysics = static_cast<Physics*>(other.GetOwner()->GetComponent("Physics"));

		BoundingRectangle rectangle(otherTransform->GetTranslation(), Vector2D(otherTransform->GetScale() * 0.5f));

		MapCollision mapCollision
		(
			IsSideColliding(rectangle, RectangleSide::SideBottom),
			IsSideColliding(rectangle, RectangleSide::SideTop),
			IsSideColliding(rectangle, RectangleSide::SideLeft),
			IsSideColliding(rectangle, RectangleSide::SideRight)
		);

		//simple collision resolution
		if (otherPhysics != nullptr)
		{
			ResolveCollisions(rectangle, static_cast<Transform*>(other.GetOwner()->GetComponent("Transform")),
				static_cast<Physics*>(other.GetOwner()->GetComponent("Physics")), mapCollision);
		}

		if (mapCollision.bottom || mapCollision.top || mapCollision.left || mapCollision.right)
		{
			MapCollisionEventHandler eventHandler = rect.GetMapCollisionHandler();

			if( eventHandler != nullptr ) eventHandler(*other.GetOwner(), mapCollision);

			return true;
		}
	}

	return false;
}

// Sets the tilemap to use for this collider.
// Params:
//   map = A pointer to the tilemap resource.
void ColliderTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Checks whether the specified side of a given rectangle is colliding with the tilemap.
// Params:
//   rectangle = The bounding rectangle for an object.
//   side = The side that needs to be tested for collision.
// Returns:
//   True if there is a collision, false otherwise.
bool ColliderTilemap::IsSideColliding(const BoundingRectangle& rectangle, RectangleSide side) const
{

	Vector2D translation = transform->GetTranslation();
	Vector2D scale = transform->GetScale();

	Vector2D hotPointOne;
	Vector2D hotPointTwo;

	//bottom
	if (side == RectangleSide::SideBottom)
	{
		hotPointOne = Vector2D( rectangle.left + (rectangle.extents.x * 0.5f), rectangle.bottom );
		hotPointTwo = Vector2D( rectangle.left + (rectangle.extents.x * 1.5f), rectangle.bottom );
	} 
	//top 
	else if (side == RectangleSide::SideTop)
	{
		hotPointOne = Vector2D( rectangle.left + (rectangle.extents.x * 0.5f), rectangle.top );
		hotPointTwo = Vector2D( rectangle.left + (rectangle.extents.x * 1.5f), rectangle.top );
	}
	//left
	else if (side == RectangleSide::SideLeft)
	{
		hotPointOne = Vector2D(rectangle.left, rectangle.bottom + (rectangle.extents.y * 0.5f));
		hotPointTwo = Vector2D(rectangle.left, rectangle.bottom + (rectangle.extents.y * 0.5f));
	}
	//right
	if (side == RectangleSide::SideRight)
	{
		hotPointOne = Vector2D( rectangle.right, rectangle.bottom + (rectangle.extents.y * 0.5f));
		hotPointTwo = Vector2D( rectangle.right, rectangle.bottom + (rectangle.extents.y * 1.5f));
	}

	return IsCollidingAtPosition(hotPointOne.x, hotPointOne.y) || IsCollidingAtPosition(hotPointTwo.x, hotPointTwo.y);
}

// Determines whether a point is within a collidable cell in the tilemap.
// Params:
//   x = The x component of the point, in world coordinates.
//   y = The y component of the point, in world coordinates.
// Returns:
//   -1 if the point is outside the map, 0 if the map is empty at that position, 
//   or a positive integer if there is a tile at that position.
bool ColliderTilemap::IsCollidingAtPosition(float x, float y) const
{
	Vector2D point( x, y);
	point = transform->GetInverseMatrix() * point;
	
	unsigned col = static_cast<unsigned>( point.x + 0.5f);
	unsigned row = static_cast<unsigned>( -point.y + 0.5f);

	if (col > map->GetWidth() || row > map->GetHeight())
	{
		return false;
	}

	return static_cast<bool>( map->GetCellValue(col, row) );
}

// Moves an object and sets its velocity based on where it collided with the tilemap.
// Params:
//   objectRectangle = A bounding rectangle that encompasses the object.
//   objectTransform = Pointer to the object's transform component.
//   objectPhysics = Pointer to the object's physics component.
//   collisions = True/false values from map collision checks.
// Returns:
//   False if the point is outside the map or the map is empty at that position, 
//   or true if there is a tile at that position.
void ColliderTilemap::ResolveCollisions(const BoundingRectangle& objectRectangle, Transform* objectTransform,
	Physics* objectPhysics, const MapCollision& collisions) const
{
	// Get the translation and velocity from the object and store them in variables
	Vector2D translation = objectTransform->GetTranslation();
	Vector2D velocity = objectPhysics->GetVelocity();

	// We need to determine how much to move the object
	float nudgeAmount;
	// We will do this by finding the center of the next tile, 
	// then finding the distance from the corresponding side to that position
	float nextTileCenter;

	if (collisions.bottom || collisions.top)
	{
		if (collisions.bottom)
		{
			// Find tile above the object's bottom side
			nextTileCenter = GetNextTileCenter(SideBottom, objectRectangle.bottom);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.bottom;
		}
		else
		{
			// Find tile below object's top side
			nextTileCenter = GetNextTileCenter(SideTop, objectRectangle.top);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.top;
		}

		// Nudge object up/down
		translation.y += nudgeAmount;
		// Stop velocity in y direction
		velocity.y = 0;
	}
	
	if (collisions.left || collisions.right)
	{
		if (collisions.left)
		{
			// Find tile to the right the object's left side
			nextTileCenter = GetNextTileCenter(SideLeft, objectRectangle.left);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.left;
		}
		else
		{
			// Find tile to the left the object's right side
			nextTileCenter = GetNextTileCenter(SideRight, objectRectangle.right);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.right;
		}

		// Nudge object left/right
		translation.x += nudgeAmount;
		// Stop velocity in x direction
		velocity.x = 0;
	}

	// Modify object's actual translation and velocity
	objectTransform->SetTranslation(translation);
	objectPhysics->SetVelocity(velocity);
}

// Find the center of the closest tile on the x-axis or y-axis.
// Used for assisting in collision resolution on a particular side.
// Params:
//   side = Which side the collision is occurring on.
//   sidePosition = The x or y value of that side.
// Returns:
//   The center of the closest tile to the given position on the given side.
float ColliderTilemap::GetNextTileCenter(RectangleSide side, float sidePosition) const
{
	// Create a world space point
	Vector2D point;
	// Create a pointer/reference variable to store the result
	float* result;

	if (side == RectangleSide::SideBottom || side == RectangleSide::SideTop)
	{
		// Use the given position for the y-axis
		// Since we only care about one axis, the other will be 0
		point = Vector2D(0, sidePosition);
		// Remember which part we want to modify
		result = &point.y;
	}
	else
	{
		// Use the given position for the x-axis
		// Since we only care about one axis, the other will be 0
		point = Vector2D(sidePosition, 0);
		// Remember which part we want to modify
		result = &point.x;
	}

	// Transform the world space point into tile space
	point = transform->GetInverseMatrix() * point;
	// Flip the y-axis
	point.y = -point.y;
	// Move completely into cell
	point += Vector2D(0.5, 0.5);

	// Find the next tile index (higher or lower depending on the side)
	if (side == RectangleSide::SideLeft || side == RectangleSide::SideTop)
	{
		*result = std::ceilf(*result);
	}
	else
	{
		*result = std::floorf(*result);
	}
		
	// Re-adjust object position
	point -= Vector2D(0.5, 0.5);
	// Flip the y-axis
	point.y = -point.y;
	// Transform point back into world space
	point = transform->GetMatrix() * point;	

	return *result;
}

