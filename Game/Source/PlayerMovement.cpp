//==================================================================-
/*
/file   PlayerMovement.cpp
/author Jakob McFarland
/date   3/27/2019
/brief

	This is the implementation file for all member functions
	of the class PlayerMovement, as specified in specification
	file PlayerMovement.h.

*/
//==================================================================-

#include "stdafx.h"
#include "PlayerMovement.h"
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

	void PlayerCollisionHandler(GameObject& object, GameObject& other);

	void PlayerMapCollisionHandler(GameObject& object,
		const MapCollision& collision);

	// Constructor
	PlayerMovement::PlayerMovement()
		: Component("PlayerMovement"), PlayerWalkSpeed(2.0f * tileUnit),
		PlayerJumpSpeed(2.0f * tileUnit), gravity(0.0f, -1.5f * tileUnit), maxJumpHeight( 4 * tileUnit ), onGround(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerMovement::Clone() const
	{
		return new PlayerMovement(*this);
	}

	// Saves object data to a file.
	void PlayerMovement::Serialize(Parser& parser) const
	{
		parser.WriteVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.WriteVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.WriteVariable("gravity", gravity);
	}

	// Loads object data from a file.
	void PlayerMovement::Deserialize(Parser& parser)
	{
		parser.ReadVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.ReadVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.ReadVariable("gravity", gravity);
	}

	// Initialize this component (happens at object creation).
	void PlayerMovement::Initialize()
	{
		transform = static_cast<Transform*>( GetOwner()->GetComponent("Transform") );
		physics = static_cast<Physics*>( GetOwner()->GetComponent("Physics") );

		Collider* collider = static_cast<Collider*>( GetOwner()->GetComponent("Collider") );

		collider->SetMapCollisionHandler( PlayerMapCollisionHandler );
		collider->SetCollisionHandler( PlayerCollisionHandler );
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerMovement::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		MoveHorizontal();
		MoveVertical();

		Graphics& graphics = Graphics::GetInstance();

		Camera& camera = graphics.GetCurrentCamera();
		camera.SetTranslation(transform->GetTranslation());
	}

	// Map collision handler for Player objects.
	// Params:
	//   object = The Player object.
	//   collision = Which sides the Player collided on.
	void PlayerMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		if (collision.bottom)
		{
			static_cast<PlayerMovement*>(object.GetComponent("PlayerMovement"))->onGround = true;
		}
	}

	// Collision handler for Player.
	// Params:
	//   object = The Player.
	//   other  = The object the Player is colliding with.
	void PlayerCollisionHandler(GameObject& object, GameObject& other)
	{
		//if the object is named collectable, destroy it
		if (other.GetName()._Equal("Collectable") )
		{
			other.Destroy();
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
	void PlayerMovement::MoveHorizontal()
	{
		Vector2D move = physics->GetVelocity();
		move.x = 0.0f;

		if (Input::GetInstance().CheckHeld(VK_RIGHT))
		{
			move.x += PlayerWalkSpeed;

			if (playerState == PlayerState::idleRt)
			{
				playerState = PlayerState::runRt;

			}

			if (playerState == PlayerState::idleRtUp)
			{
				playerState = PlayerState::runRtUp;

			}
		}
		else if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			move.x -= PlayerWalkSpeed;

			if (playerState == PlayerState::idleRt)
			{
				playerState = PlayerState::runLt;

			}

			if (playerState == PlayerState::idleRtUp)
			{
				playerState = PlayerState::runLtUp;
			
			}
		}
		else
		{
			if (playerState == PlayerState::runLt)
			{
				playerState = PlayerState::idleLt;

			}

			if (playerState == PlayerState::runRt)
			{
				playerState = PlayerState::idleLt;

			}
		}

		if (Input::GetInstance().CheckTriggered (VK_UP))
		{
			if (playerState == PlayerState::idleLt)
			{
				playerState = PlayerState::idleLtUp;

			}

			if (playerState == PlayerState::runLt)
			{
				playerState = PlayerState::runLtUp;

			}
	
			if (playerState == PlayerState::idleRt)
			{
				playerState = PlayerState::idleRtUp;
			
			}

			if (playerState == PlayerState::runRt)
			{
				playerState = PlayerState::runRtUp;
			
			}
		}

		if (Input::GetInstance().CheckReleased(VK_UP))
		{
			if (playerState == PlayerState::idleLtUp)
			{
				playerState = PlayerState::idleLt;

			}

			if (playerState == PlayerState::runLtUp)
			{
				playerState = PlayerState::runLt;

			}

			if (playerState == PlayerState::idleRtUp)
			{
				playerState = PlayerState::idleRt;

			}

			if (playerState == PlayerState::runRtUp)
			{
				playerState = PlayerState::runRt;

			}
		}

		physics->SetVelocity(move);
	}

	// Moves vertically based on input
	void PlayerMovement::MoveVertical()
	{
		if (physics->GetVelocity().y < -1.0f)
		{
			onGround = false;
		}	

		if (Input::GetInstance().CheckHeld(VK_SPACE))
		{
			if ( transform->GetTranslation().y <= jumpStartY + maxJumpHeight)
			{
				Vector2D move = physics->GetVelocity();
				move.y = PlayerJumpSpeed;
				physics->SetVelocity(move);
				onGround = false;
			}
		}
		
		physics->AddForce(gravity);
	}
}