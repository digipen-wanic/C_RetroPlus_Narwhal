//==================================================================-
/*
/file   Physics.cpp
/author Jakob McFarland
/date   11/7/2018
/brief

This is the implementation file for all member functions
of the class Physics, as specified in specification
file Physics.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Physics.h"
#include "Component.h"
#include <Parser.h>

#include "GameObject.h"
#include "Transform.h"
#include <Vector2D.h>

//temp
#include "Input.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new physics component.
// Params:
//   transform - The transform of the object.
Physics::Physics()
	: Component("Physics"), velocity(Vector2D(0.0f, 0.0f)), angularVelocity(0.0f),
	inverseMass(1), forcesSum(Vector2D(0.0f, 0.0f)), acceleration(Vector2D(0.0f, 0.0f)),
	oldTranslation(Vector2D(0.0f, 0.0f)), transform(transform)
{
}

// Clone the physics component, returning a dynamically allocated copy.
Component* Physics::Clone() const 
{
	return new Physics(*this);
}

// Saves object data to a file.
void Physics::Serialize(Parser& parser) const
{
	parser.WriteVariable("acceleration", acceleration);
	parser.WriteVariable("velocity", velocity);
	parser.WriteVariable("angularVelocity", angularVelocity);
	parser.WriteVariable("inverseMass", inverseMass);
}

// Loads object data from a file.
void Physics::Deserialize(Parser& parser)
{
	parser.ReadVariable("acceleration", acceleration );
	parser.ReadVariable("velocity", velocity);
	parser.ReadVariable("angularVelocity", angularVelocity);
	parser.ReadVariable("inverseMass", inverseMass);
}

// Initialize components.
void Physics::Initialize()
{
	transform = static_cast<Transform*>( GetOwner()->GetComponent("Transform") );
}

// Reset acceleration.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Physics::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	acceleration = forcesSum * inverseMass;
	forcesSum = Vector2D(0.0f, 0.0f);
}

// Update velocity and translation.
// Params:
//	 dt = Change in time (in seconds) since the last fixed update.
void Physics::FixedUpdate(float dt)
{
	velocity = velocity + acceleration * dt;
	oldTranslation = transform->GetTranslation();
	Vector2D newTranslation = oldTranslation + velocity * dt;

	float newRotation = transform->GetRotation() + angularVelocity * dt;

	transform->SetTranslation(newTranslation);
	transform->SetRotation(newRotation);
}

// Set the velocity of a physics component.
// Params:
//	 velocity = Pointer to a velocity vector.
void Physics::SetVelocity(const Vector2D& velocity_)
{
	velocity = velocity_;
}

// Get the velocity of a physics component.
// Returns:
//		A reference to the component's velocity structure.
const Vector2D& Physics::GetVelocity() const
{
	return velocity;
}

// Set the angular velocity of a physics component.
// Params:
//	 velocity = New value for the rotational velocity.
void Physics::SetAngularVelocity(float velocity_)
{
	angularVelocity = velocity_;
}

// Get the angular velocity of a physics component.
// Returns:
//	 A float representing the new angular velocity.
float Physics::GetAngularVelocity() const
{
	return angularVelocity;
}

// Set the mass of the physics component.
// Params:
//   mass = The new mass of the physics component.
void Physics::SetMass(float mass_)
{
	inverseMass = 1/mass_;
}

// Add a force to the object.
// Params:
//	 force = A force vector with direction and magnitude.
void Physics::AddForce(const Vector2D& force)
{
	forcesSum += force;
}

// Get the acceleration of a physics component.
// Returns:
//	 A reference to the component's acceleration structure.
const Vector2D& Physics::GetAcceleration() const
{
	return acceleration;
}

// Set the old translation (position) of a physics component.
// Params: 
//   A reference to the component's new oldTranslation structure,
void Physics::SetOldTranslation(const Vector2D& oldTranslation_)
{
	oldTranslation = oldTranslation_;
}

// Get the old translation (position) of a physics component.
// Returns: 
//   A reference to the component's oldTranslation structure,
const Vector2D& Physics::GetOldTranslation() const
{
	return oldTranslation;
}