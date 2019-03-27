//==================================================================-
/*
/file   Intersection2D.cpp
/author Jakob McFarland
/date   2/27/2019
/brief

This is the implementation file for all member functions
of the class Intersection2D, as specified in specification
file Intersection2D.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Intersection2D.h"
#include "Transform.h"
#include "Physics.h"

#include "DebugDraw.h"
#include "Graphics.h"

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Check whether a point and circle intersect.
// Params:
//  point = The point in the intersection.
//  circle = The circle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointCircleIntersection(const Vector2D& point, const Circle& circle)
{
	if (point.DistanceSquared(circle.center) <= circle.radius * circle.radius)
	{
		return true;
	}

	return false;
}

// Check whether a point and rectangle intersect.
// Params:
//  point = The point in the intersection.
//  rect  = The rectangle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect)
{
	if (point.x < rect.left) return false;
	if (point.x > rect.right) return false;
	if (point.y < rect.bottom) return false;
	if (point.y > rect.top) return false;
	return true;
}

// Check whether two circles intersect.
// Params:
//  circle1 = The first circle.
//  circle2 = The second circle.
// Returns:
//   True if intersection, false otherwise.
bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2)
{
	if (circle1.center.DistanceSquared(circle2.center) <= 
		(circle1.radius + circle2.radius) * (circle1.radius + circle2.radius))
	{
		return true;
	}

	return false;
}

// Check whether two rectangles intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2)
{
	if (rect1.right < rect2.left) return false;
	if (rect1.left > rect2.right) return false;
	if (rect1.top < rect2.bottom) return false;
	if (rect1.bottom > rect2.top) return false;
	return true;
}

// Check whether a rectangle and circle intersect.
// Params:
//  rect = The rectangle.
//  circle  = The circle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle)
{
	Vector2D point;

	point.x = clamp(circle.center.x, rect.left, rect.right);
	point.y = clamp(circle.center.y, rect.bottom, rect.top);

	if (point.DistanceSquared(circle.center) <= circle.radius * circle.radius)
	{
		return true;
	}

	return false;
}

// Check whether a moving point and line intersect.
// Params:
//  staticLine   = Start and end of first line segment.
//  movingPoint = Start and end of second line segment (usually a moving point).
//  intersection = Intersection point, if any.
//  t = The t value from the intersection calculation.
// Returns:
//   True if intersection, false otherwise.
bool MovingPointLineIntersection(const LineSegment& staticLine, const LineSegment& movingPoint, Vector2D& intersection, float& t)
{
	// COMBINED EQs							
	// normal dot start + normal dot velocity * t = normal dot p
	// v
	// t = normal dot p - normal dot start / normal dot velocity
	 
	Vector2D velocity = movingPoint.end - movingPoint.start;

	if ( AlmostEqual( staticLine.normal.DotProduct(velocity), 0 ) )
	{
		return false;
	}

	t = ( staticLine.normal.DotProduct( staticLine.start ) -
		 staticLine.normal.DotProduct( movingPoint.start ) ) / staticLine.normal.DotProduct( velocity );

	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	intersection = movingPoint.start + velocity * t;

	//DebugDraw::GetInstance().AddCircle(intersection, 4, Graphics::GetInstance().GetDefaultCamera());

	if( !PointIsBetweenLines(intersection, staticLine.start, staticLine.end, staticLine.direction) )
	{
		return false;
	}

	//std::cout << t << std::endl;
	//std::cout << velocity << std::endl;

	//std::cout << "crossing" << std::endl;

	return true;
}

// Modifies object's position, velocity, and rotation using simple point-line reflection.
// Params:
//  transform = Transform of the object that is being reflected.
//  physics   = Physics of the object being reflected.
//  staticLine   = Start and end of first line segment.
//  movingPoint = Start and end of second line segment (usually a moving point).
//  intersection = Intersection point of the line and circle. 
void MovingPointLineReflection(Transform& transform, Physics& physics, const LineSegment& staticLine, const LineSegment& movingPoint, const Vector2D& intersection)
{
	// 1. Find correct position of object by reflecting its end point over the line.
	// (Use the ReflectPointOverLine function!)
	// Once found, set the object's translation to this reflected point.

	Vector2D reflectedPoint = ReflectPointOverLine(movingPoint.end, staticLine);
	transform.SetTranslation(reflectedPoint);

	// 2. Use the reflected position and intersection point to find the
	// direction of the reflected velocity. Make sure to normalize this vector!

	Vector2D newDirection = (reflectedPoint - intersection).Normalized();

	// 3. Set the velocity of the object to this direction times
	// the magnitude of the current velocity.

	physics.SetVelocity(newDirection * physics.GetVelocity().Magnitude());

	// 4. Find the object's new rotation by using atan2 with the reflected direction.
	// Once found, set the rotation of the object to this angle.

	transform.SetRotation( std::atan2( newDirection.y, newDirection.x ) );
}

// Checks whether a point is between two parallel lines.
// Params:
//   point = The point in question.
//   firstLine = A point on the first line.
//   secondLine = A point on the second line.
//   normal = Normal vector to the two lines.
// Returns:
//   True if the point is between the two lines, false otherwise.
bool PointIsBetweenLines(const Vector2D& point, const Vector2D& firstLine, const Vector2D& secondLine, const Vector2D& normal)
{
	const float distPoint = normal.DotProduct(point);
	const float dist1 = normal.DotProduct(firstLine);
	const float dist2 = normal.DotProduct(secondLine);

	if (dist1 < distPoint && dist2 < distPoint)
	{
		return false;
	}

	if (dist1 > distPoint && dist2 > distPoint)
	{
		return false;
	}
	
	return true;
}

// Reflects a point over a line.
// Params:
//   point = The point being reflected.
//   line = The line the point will be reflected over.
// Returns:
//   The reflected point.
Vector2D ReflectPointOverLine(const Vector2D& point, const LineSegment& line)
{
	Vector2D point_ = point - line.start;

	Vector2D reflection = point_ - 2 * line.normal * point_.DotProduct( line.normal );

	return line.start + reflection;
} 

// Clamp a value.
// Params:
//  val = value to be clamped.
//  min  = minimum value after clamp.
//  max  = maximum value after clamp.
// Returns:
//   clamped value.
float clamp(float val, float min, float max)
{
	if (val > max) val = max;
	else if (val < min) val = min;
	return val;
}

//------------------------------------------------------------------------------
