//==================================================================-
/*
/file   MonkeyMovement.cpp
/author Jakob McFarland
/date   12/4/2018
/brief

	This is the implementation file for all member functions
	of the class MonkeyMovement, as specified in specification
	file MonkeyMovement.h.

*/
//==================================================================-

#include "stdafx.h"
#include "MonkeyMovement.h"
#include <Parser.h>
#include <GameObjectFactory.h>

#include "GameObject.h"
#include "Collider.h"
#include "Input.h"
#include "Transform.h"
#include "Physics.h"
#include "Space.h"
#include "Graphics.h"

namespace Behaviors
{
	//==================================================================-
	// Public methods:
	//==================================================================-

	void MonkeyCollisionHandler(GameObject& object, GameObject& other);

	void MonkeyMapCollisionHandler(GameObject& object,
		const MapCollision& collision);

	// Constructor
	MonkeyMovement::MonkeyMovement()
		: Component("MonkeyMovement"), monkeyWalkSpeed(150.0f),
		monkeyJumpSpeed(400.0f), gravity(0.0f, -300.0f), onGround(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* MonkeyMovement::Clone() const
	{
		return new MonkeyMovement(*this);
	}

	// Saves object data to a file.
	void MonkeyMovement::Serialize(Parser& parser) const
	{
		parser.WriteVariable("monkeyWalkSpeed", monkeyWalkSpeed);
		parser.WriteVariable("monkeyJumpSpeed", monkeyJumpSpeed);
		parser.WriteVariable("gravity", gravity);
	}

	// Loads object data from a file.
	void MonkeyMovement::Deserialize(Parser& parser)
	{
		parser.ReadVariable("monkeyWalkSpeed", monkeyWalkSpeed);
		parser.ReadVariable("monkeyJumpSpeed", monkeyJumpSpeed);
		parser.ReadVariable("gravity", gravity);
	}

	// Initialize this component (happens at object creation).
	void MonkeyMovement::Initialize()
	{
		transform = static_cast<Transform*>( GetOwner()->GetComponent("Transform") );
		physics = static_cast<Physics*>( GetOwner()->GetComponent("Physics") );

		Collider* collider = static_cast<Collider*>( GetOwner()->GetComponent("Collider") );

		collider->SetMapCollisionHandler( MonkeyMapCollisionHandler );
		collider->SetCollisionHandler( MonkeyCollisionHandler );
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyMovement::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		MoveHorizontal();
		MoveVertical();


	}

	// Map collision handler for Monkey objects.
	// Params:
	//   object = The monkey object.
	//   collision = Which sides the monkey collided on.
	void MonkeyMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		if (collision.bottom)
		{
			static_cast<MonkeyMovement*>(object.GetComponent("MonkeyMovement"))->onGround = true;
		}
	}

	// Collision handler for monkey.
	// Params:
	//   object = The monkey.
	//   other  = The object the monkey is colliding with.
	void MonkeyCollisionHandler(GameObject& object, GameObject& other)
	{
		//if the object is named collectable, destroy it
		if (other.GetName()._Equal("Collectable") )
		{
			other.Destroy();

			//check if THIS collectable is the last one
			if (object.GetSpace()->GetObjectManager().GetObjectCount("Collectable") <= 1)
			{
				object.GetSpace()->RestartLevel();
			}
		}

		//if the object is named hazard, kill the player
		if (other.GetName()._Equal("Hazard"))
		{
			object.GetSpace()->RestartLevel();
		}

		//if the object is named enemy, kill the player
		if (other.GetName()._Equal("Enemy"))
		{
			object.GetSpace()->RestartLevel();
		}
	}

	//==================================================================-
	// Private Methods:
	//==================================================================-

	// Moves horizontally based on input
	void MonkeyMovement::MoveHorizontal() const
	{
		Vector2D move = physics->GetVelocity();
		move.x = 0.0f;

		if (Input::GetInstance().CheckHeld(VK_RIGHT))
		{
			move.x += monkeyWalkSpeed;
		}
		else if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			move.x -= monkeyWalkSpeed;
		}

		physics->SetVelocity(move);
	}

	// Moves vertically based on input
	void MonkeyMovement::MoveVertical() 
	{
		if (physics->GetVelocity().y < -1.0f)
		{
			onGround = false;
		}

		if (onGround)
		{
			if (Input::GetInstance().CheckHeld(VK_UP))
			{
				Vector2D move = physics->GetVelocity();
				move.y = monkeyJumpSpeed;
				physics->SetVelocity(move);
				onGround = false;
			}
		}

		physics->AddForce(gravity);
	}
}