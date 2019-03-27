//==================================================================-
/*
/file   ColliderLine.cpp
/author Jakob McFarland
/date   2/27/2019
/brief

This is the implementation file for all member functions
of the class ColliderLine, as specified in specification
file ColliderLine.h.

*/
//==================================================================-


#include "stdafx.h"
#include "ColliderLine.h"

#include "Intersection2D.h"
#include "ColliderCircle.h"
#include "ColliderRectangle.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "Graphics.h"
#include "Transform.h"
#include "Physics.h"
#include "Shapes2D.h"
#include <Parser.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new (line) collider component.
ColliderLine::ColliderLine(bool reflection_)
	: Collider(ColliderType::ColliderTypeLines), reflection(reflection_)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderLine::Clone() const
{
	return new ColliderLine(*this);
}

// Debug drawing for colliders.
void ColliderLine::Draw()
{
	for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
	{
		DebugDraw::GetInstance().AddLineToStrip( i->start, i->end, Colors::Green);
		DebugDraw::GetInstance().AddLineToStrip( i->start.Midpoint(i->end), (i->start.Midpoint(i->end) + i->normal * 20), Colors::Red);
	}
	DebugDraw::GetInstance().EndLineStrip( Graphics::GetInstance().GetDefaultCamera() );
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLine::AddLineSegment(const Vector2D& p0, const Vector2D& p1)
{
	//Transform* transform_ = GetOwner()->GetComponent<Transform>();

	//float xScalar = transform_->GetScale().x; 
	//float yScalar = transform_->GetScale().y;

	//Vector2D p0_ = p0; p0_.x *= xScalar; p0_.y *= yScalar;
	//Vector2D p1_ = p1; p1_.x *= xScalar; p1_.y *= yScalar;

	lineSegments.push_back( LineSegment(p0, p1) );
}

// Check for collision between a line collider and another arbitrary collider.
// Params:
//	 other = Reference to the other collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderLine::IsCollidingWith(const Collider& other) const
{
	if (other.GetType() == ColliderType::ColliderTypePoint)
	{
		Transform* otherTransform = other.GetOwner()->GetComponent<Transform>();
		Physics* otherPhysics = other.GetOwner()->GetComponent<Physics>();

		Vector2D inter;
		float t;

		for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
		{
			LineSegment movement = LineSegment(otherPhysics->GetOldTranslation(),
				otherTransform->GetTranslation() - otherPhysics->GetOldTranslation());

			if ( MovingPointLineIntersection(movement, *i, inter, t) )
			{
				if (reflection)
				{
					MovingPointLineReflection(*otherTransform, *otherPhysics, *i, movement, inter);
				}

				return true;
			}
		}	 
	}
	else if (other.GetType() == ColliderType::ColliderTypeCircle)
	{
		const ColliderCircle& circle = static_cast<const ColliderCircle&>(other);
		Transform* otherTransform = circle.GetOwner()->GetComponent<Transform>();
		Physics* otherPhysics = other.GetOwner()->GetComponent<Physics>();

		Vector2D inter;
		float t;

		for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
		{
			//get a vector from the circle center to the line perpendicular to that line
			//Vector2D originToLine = (i->direction.Normalized().DotProduct(otherTransform->GetTranslation()
			//	- i->start) * i->direction.Normalized() - otherTransform->GetTranslation());

			LineSegment movingPoint = LineSegment(otherPhysics->GetOldTranslation(), otherTransform->GetTranslation());

			if (MovingPointLineIntersection( *i, movingPoint, inter, t))
			{
				std::cout << "intersection" << std::endl;

				std::cout << GetOwner()->GetName() << i->normal << std::endl;

				if (reflection)
				{
					MovingPointLineReflection(*otherTransform, *otherPhysics, *i, movingPoint, inter);
				}

				return true;
			}
		}
	}
	//rectangle collision
	else if (other.GetType() == ColliderType::ColliderTypeRectangle)
	{	
		const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
		Transform* otherTransform = rect.GetOwner()->GetComponent<Transform>();
		Physics* otherPhysics = other.GetOwner()->GetComponent<Physics>();

		Vector2D inter;
		float t;

		for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
		{
			//get a vector from the circle center to the line perpendicular to that line
			//Vector2D originToLine = (i->direction.Normalized().DotProduct(otherTransform->GetTranslation()
			//	- i->start) * i->direction.Normalized() - otherTransform->GetTranslation());

			LineSegment movement = LineSegment(otherPhysics->GetOldTranslation(),
				otherTransform->GetTranslation() - otherPhysics->GetOldTranslation());

			if (MovingPointLineIntersection(movement, *i, inter, t))
			{
				if (reflection)
				{
					MovingPointLineReflection(*otherTransform, *otherPhysics, *i, movement, inter);
				}

				return true;
			}
		}
		
	}
	else if (other.GetType() == ColliderType::ColliderTypeLines)
	{
		ColliderLine* otherLine = other.GetOwner()->GetComponent<ColliderLine>();

		Vector2D inter;
		//float t;

		for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
		{
			for (auto other_ = otherLine->lineSegments.begin(); other_ != otherLine->lineSegments.end(); ++other_)
			{
				/*
				if (MovingPointLineIntersection(*i, *other_, inter, t))
				{
					return true;
				}
				*/
			}
		}

		return false;
	}

	return false;
}

// Save object data to file.
// Params:
//   parser = The parser object used to save the object's data.
void ColliderLine::Serialize(Parser& parser) const
{
	parser.WriteVariable("reflection", reflection);

	for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
	{
		parser.WriteVariable("lineSegments", *i);
	}
}

// Load object data from file
// Params:
//   parser = The parser object used to load the object's data.
void ColliderLine::Deserialize(Parser & parser)
{
	parser.ReadVariable("reflection", reflection);

	for (auto i = lineSegments.begin(); i != lineSegments.end(); ++i)
	{
		parser.ReadVariable("lineSegments", *i);
	}
}

// Gets a line segment that incorporates the transform of the object
// Params:
//   index = The index of the line within the array of line segments.
LineSegment ColliderLine::GetLineWithTransform(unsigned index) const
{
	return lineSegments[index];
}