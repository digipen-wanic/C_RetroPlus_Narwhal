//------------------------------------------------------------------------------
//
// File Name:	ColliderPoint.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ColliderPoint : public Collider
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for point collider.
	ColliderPoint();

	// Clone an collider and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a collider.
	Component* Clone() const;

	// Saves object data to a file.
	virtual void Serialize(Parser& parser) const override;

	// Loads object data from a file.
	virtual void Deserialize(Parser& parser) override;

	// Debug drawing for colliders.
	void Draw() override;

	// Check for collision between a point and another arbitrary collider.
	// Params:
	//	 other = Reference to the second point collider component.
	// Returns:
	//	 Return the results of the collision check.
	bool IsCollidingWith(const Collider& other) const override;
};

//------------------------------------------------------------------------------
