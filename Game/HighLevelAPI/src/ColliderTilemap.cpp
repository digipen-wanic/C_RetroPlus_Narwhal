//------------------------------------------------------------------------------
//
// File Name:	ColliderTilemap.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderTilemap.h"

// Systems
#include <Engine.h>
#include <Shapes2D.h>

// Components
#include "GameObject.h" // GetTransform
#include "Transform.h"	// GetTranslation
#include "ColliderRectangle.h" // GetExtents
#include "Physics.h"	// GetOldTranslation

// Systems
#include <Parser.h> // Read/Write Variable
#include <Space.h>	// GetResourceManager

// Resources
#include "Tilemap.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Amount to nudge objects if they are stuck
const float nudgeTolerance = 0.1f;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Constructor for circle collider.
ColliderTilemap::ColliderTilemap()
	: Collider(ColliderTypeTilemap), map(nullptr)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderTilemap::Clone() const
{
	return new ColliderTilemap(*this);
}

// Debug drawing for colliders.
void ColliderTilemap::Draw()
{
	// TO-DO: Figure out how the heck to draw tilemap collider
}

// Check for collision between a circle and an arbitrary collider.
// Params:
//	 other = Reference to the second circle collider component.
bool ColliderTilemap::IsCollidingWith(const Collider& other) const
{
	// Create bounding rectangle for object
	Transform* transformOther = static_cast<const ColliderTilemap&>(other).transform;
	BoundingRectangle rectangle = BoundingRectangle(transformOther->GetTranslation(),
		transformOther->GetScale() / 2.0f);

	switch (other.GetType())
	{
	case ColliderTypeRectangle:
	{	
		// Create rectangle using custom extents if the object is an actual rectangle
		const ColliderRectangle& colliderRectangle = static_cast<const ColliderRectangle&>(other);
		rectangle = BoundingRectangle(transformOther->GetTranslation(),
			colliderRectangle.GetExtents());
		break;
	}
	default:
		break;
	}

	// Check for collisions
	const MapCollision collisions = MapCollision(
		IsSideColliding(rectangle, SideBottom),
		IsSideColliding(rectangle, SideTop),
		IsSideColliding(rectangle, SideLeft),
		IsSideColliding(rectangle, SideRight)
	);

	// Resolve collisions
	Physics* physicsOther = static_cast<const ColliderTilemap&>(other).physics;
	if(physicsOther)
		ResolveCollisions(rectangle, transformOther, physicsOther, collisions);

	// If there was a collision
	if (collisions.bottom || collisions.top || collisions.left || collisions.right)
	{
		// Send map collision event to other object
		if (other.GetMapCollisionHandler() != nullptr)
		{
			other.GetMapCollisionHandler()(*other.GetOwner(), collisions);
		}
		return true;
	}

	return false;
}
const Tilemap* ColliderTilemap::GetTilemap() const
{
	return map;
}

Vector2D ColliderTilemap::GetTileIndices(Vector2D translation) const
{
	Vector2D point(translation.x, translation.y);

	point = transform->GetInverseMatrix() * point;

	float column = point.x + 0.5f;
	float row = -point.y + 0.5f;

	return Vector2D(column, row);
}

// Sets the tilemap to use for this collider.
// Params:
//   map = A pointer to the tilemap resource.
void ColliderTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file we want to read from.
void ColliderTilemap::Deserialize(Parser & parser)
{
	Collider::Deserialize(parser);
	std::string tilemapName;
	parser.ReadVariable("tilemap", tilemapName);
	SetTilemap(GetOwner()->GetSpace()->GetResourceManager().GetTilemap(tilemapName));
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file we want to write to.
void ColliderTilemap::Serialize(Parser & parser) const
{
	Collider::Serialize(parser);
	parser.WriteVariable("tilemap", map->GetName());
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
	// Create hot spots based on rectangle
	float x1 = rectangle.center.x - rectangle.extents.x * (2.0f / 3.0f);
	float x2 = rectangle.center.x + rectangle.extents.x * (2.0f / 3.0f);
	float y1 = rectangle.center.y - rectangle.extents.y * (2.0f / 3.0f);
	float y2 = rectangle.center.y + rectangle.extents.y * (2.0f / 3.0f);

	const unsigned numHotspots = 2;
	Vector2D hotspots[numHotspots];

	switch (side)
	{
	case SideBottom:
		hotspots[0] = Vector2D(x1, rectangle.bottom);
		hotspots[1] = Vector2D(x2, rectangle.bottom);
		break;
	case SideTop:
		hotspots[0] = Vector2D(x1, rectangle.top);
		hotspots[1] = Vector2D(x2, rectangle.top);
		break;
	case SideLeft:
		hotspots[0] = Vector2D(rectangle.left, y1);
		hotspots[1] = Vector2D(rectangle.left, y2);
		break;
	case SideRight:
		hotspots[0] = Vector2D(rectangle.right, y1);
		hotspots[1] = Vector2D(rectangle.right, y2);
		break;
	default:
		break;
	}

	for (unsigned i = 0; i < numHotspots; ++i)
	{
		if (IsCollidingAtPosition(hotspots[i].x, hotspots[i].y))
			return true;
	}

	return false;
}

// Determines whether a point is within a collidable cell in the tilemap.
// Params:
//   x = The x component of the point, in world coordinates.
//   y = The y component of the point, in world coordinates.
// Returns:
//   False if the point is outside the map or the map is empty at that position, 
//   or true if there is a tile at that position.
bool ColliderTilemap::IsCollidingAtPosition(float x, float y) const
{
	// Undo map transform
	Vector2D transformedPoint = transform->GetInverseMatrix() * Vector2D(x, y);

	// Figure out which cell we need by adding 0.5 and converting to integers
	int column = static_cast<int>(transformedPoint.x + 0.5f);
	int row = static_cast<int>(-transformedPoint.y + 0.5f);

	// Return the value at the given cell > 0
	return map->GetCellValue(column, row) > 0;
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
void ColliderTilemap::ResolveCollisions(const BoundingRectangle & objectRectangle, 
	Transform * objectTransform, Physics * objectPhysics, const MapCollision & collisions) const
{
	// Get translation, old translation, velocity
	Vector2D translation = objectTransform->GetTranslation();
	Vector2D velocity = objectPhysics->GetVelocity();

	float nudgeAmount;

	// If colliding on top or bottom
	if (collisions.bottom || collisions.top)
	{
		nudgeAmount = 0.0f;

		// Figure out how much to nudge
		if (collisions.bottom)
		{
			nudgeAmount = GetNextTileCenter(SideBottom, objectRectangle.bottom) - objectRectangle.bottom;
		}
		else
		{
			nudgeAmount = GetNextTileCenter(SideTop, objectRectangle.top) - objectRectangle.top;
		}

		// Nudge object up/down
		translation.y += nudgeAmount;
		// Stop velocity in y direction
		velocity.y = 0.0f;
	}

	// If colliding on right or left
	if (collisions.right || collisions.left)
	{
		nudgeAmount = 0.0f;

		// Figure out how much to nudge
		if (collisions.left)
		{
			nudgeAmount = GetNextTileCenter(SideLeft, objectRectangle.left) - objectRectangle.left;
		}
		else
		{
			nudgeAmount = GetNextTileCenter(SideRight, objectRectangle.right) - objectRectangle.right;
		}

		// Nudge object left/right
		translation.x += nudgeAmount;
		// Stop velocity in x direction
		velocity.x = 0.0f;
	}

	// Modify translation and velocity
	objectTransform->SetTranslation(translation);
	objectPhysics->SetVelocity(velocity);
}

// Returns the center of the closest tile on the x-axis or y-axis.
// Used for assisting in collision resolution on a particular side.
// Params:
//   side = Which side the collision is occurring on.
//   sidePosition = The x or y value of that side.
// Returns:
//   The center of the closest tile to the given position on the given side.
float ColliderTilemap::GetNextTileCenter(RectangleSide side, float sidePosition) const
{
	// Create world space point
	Vector2D point;
	float* result = nullptr;
	if (side == SideBottom || side == SideTop)
	{
		point.y = sidePosition;
		result = &point.y;
	}
	if (side == SideLeft || side == SideRight)
	{
		point.x = sidePosition;
		result = &point.x;
	}

	// Turn point into tile-space point
	point = transform->GetInverseMatrix() * point;
	point.y = -point.y; // Flip y-axis
	point += Vector2D(0.5f, 0.5f); // Move completely into cell

	// Floor/ceil point based on desired side
	if (side == SideLeft || side == SideTop)
		*result = ceil(*result);
	else
		*result = floor(*result);

	// Put result back in world space
	point -= Vector2D(0.5f, 0.5f);
	point.y = -point.y;
	point = transform->GetMatrix() * point;

	// Return result
	return *result;
}
