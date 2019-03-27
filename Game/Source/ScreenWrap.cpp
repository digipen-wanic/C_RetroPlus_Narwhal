//==================================================================-
/*
/file   ScreenWrap.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class ScreenWrap, as specified in specification
file ScreenWrap.h.

*/
//==================================================================-

#include "stdafx.h"
#include "ScreenWrap.h"
#include "Component.h"
#include <Parser.h>
#include <GameObjectFactory.h>

#include <Graphics.h>
#include "Space.h"
#include "Transform.h"
#include "Physics.h"

namespace Behaviors
{
	//==================================================================-
	// Public Functions:
	//==================================================================-

	// Default constructor
	ScreenWrap::ScreenWrap()
		: Component("ScreenWrap")
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* ScreenWrap::Clone() const
	{
		return new ScreenWrap(*this);
	}

	// Initialize this component (happens at object creation).
	void ScreenWrap::Initialize()
	{
		transform = static_cast<Transform*>( GetOwner()->GetComponent("Transform") );
		physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics") );
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void ScreenWrap::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Vector2D translation = transform->GetTranslation();
		Vector2D scale = transform->GetScale();
		Vector2D velocity = physics->GetVelocity();

		BoundingRectangle screenEdge = Graphics::GetInstance().GetScreenWorldDimensions();

		//Top-Bottom

		//If the object is moving up and its position is greater than the top side of the screen + the object's scale
		if (velocity.y > 0 && translation.y > screenEdge.top + scale.y)
		{
			//Set the object's translation to the bottom side of the screen - the object's scale
			transform->SetTranslation(Vector2D(translation.x, screenEdge.bottom - scale.y));
		}

		//If the object is moving down and its position is greater than the right side of the screen - the object's scale
		if (velocity.y < 0 && translation.y < screenEdge.bottom - scale.y)
		{
			//Set the object's translation to the top side of the screen + the object's scale
			transform->SetTranslation(Vector2D(translation.x, screenEdge.top + scale.y));
		}

		//Left-Right

		//If the object is moving right and its position is greater than the right side of the screen + the object's scale
		if (velocity.x > 0 && translation.x > screenEdge.right + scale.x)
		{
			//Set the object's translation to the left side of the screen - the object's scale
			transform->SetTranslation( Vector2D(screenEdge.left - scale.x, translation.y));
		}

		//If the object is moving left and its position is less than the left side of the screen - the object's scale
		if (velocity.x < 0 && translation.x < screenEdge.left - scale.x)
		{
			//Set the object's translation to the right side of the screen + the object's scale
			transform->SetTranslation(Vector2D(screenEdge.right + scale.x, translation.y));
		}
	}
}
