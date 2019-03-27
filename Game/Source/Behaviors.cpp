//==================================================================-
/*
/file   Behaviors.cpp
/author Jakob McFarland
/date   11/7/2018
/brief

This is the implementation file for all member functions
of the class Behaviors, as specified in specification
file Behaviors.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Behaviors.h"

#include "Input.h"
#include "Transform.h"
#include "Physics.h"
#include <Graphics.h>

namespace Behaviors
{
	const float ShipDrag = 0.002f;

	const float monkeyWalkSpeed = 1.0f;

	const float monkeyJumpSpeed = 0.01f;

	const float groundHeight = -200.0f;

	const float gravity = 1.0f;

	// Movement behavior for the triangle/ship.
	// Params:
	//	 transform = The transform component of the ship.
	//   physics = The physics component of the ship.
	void UpdateShip(Transform* transform, Physics* physics)
	{
		//get the to mouse vector
		Graphics& graphics = Graphics::GetInstance();
		Vector2D mouse = graphics.ScreenToWorldPosition( Input::GetInstance().GetCursorPosition() );

		//set the ships velocity with an artificial drag
		physics->SetVelocity( (mouse - transform->GetTranslation()) * ShipDrag);

		//quick perpindicularisation of the ship direction
		Vector2D ship = Vector2D::FromAngleRadians(transform->GetRotation());
		Vector2D shipPerp = Vector2D(ship.y, -ship.x);

		//get the directional bearing of the angle between the
		//ship direction vectorand the the to mouse vector
		float angleBearing = -mouse.DotProduct(shipPerp);
		float angularVelocity = angleBearing;

		physics->SetAngularVelocity(angularVelocity);
	}

	// Movement behavior for the monkey.
	// Params:
	//	 transform = The transform component of the monkey.
	//   physics = The physics component of the monkey.
	void UpdateMonkey(Transform* transform, Physics* physics)
	{
		UNREFERENCED_PARAMETER(transform);
		UNREFERENCED_PARAMETER(physics);

		Vector2D move;
		move.x = 0.0f;
		move.y = physics->GetVelocity().y;

		//horizontal movement
		if (Input::GetInstance().CheckHeld(VK_RIGHT))
		{
			move.x += monkeyWalkSpeed;
		}
		else if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			move.x -= monkeyWalkSpeed;
		}

		physics->SetVelocity(move);

		//jumping and gravity
		if (transform->GetTranslation().y <= groundHeight && physics->GetVelocity().y <= 0)
		{	
			if (Input::GetInstance().CheckHeld(VK_SPACE))
			{
				move.y = monkeyJumpSpeed;
			}
			else
			{
				//landing on "ground"
				move.y = 0;
				Vector2D translation = transform->GetTranslation();
				translation.y = groundHeight;
				transform->SetTranslation(translation);
			}

			physics->SetVelocity(move);
		}
		else
		{
			physics->AddForce(Vector2D(0.0f, -gravity));
		}
	}
}