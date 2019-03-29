//==================================================================-
/*
/file   PlayerController.cpp
/author Jakob McFarland
/date   3/28/2019
/brief

	This is the implementation file for all member functions
	of the class PlayerController, as specified in specification
	file PlayerController.h.

*/
//==================================================================-

#include "stdafx.h"
#include "PlayerController.h"
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
	PlayerController::PlayerController()
		: Component("PlayerController"), PlayerWalkSpeed(2.0f * tileUnit),
		PlayerJumpSpeed(4.0f * tileUnit), gravity(0.0f, -3.0f * tileUnit),
		maxJumpHeight( 4 * tileUnit ), firingSpeed(0.25), firingTimer(0), 
		bulletSpeed(tileUnit * 4), onGround(false), jumping(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerController::Clone() const
	{
		return new PlayerController(*this);
	}

	// Saves object data to a file.
	void PlayerController::Serialize(Parser& parser) const
	{
		parser.WriteVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.WriteVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.WriteVariable("gravity", gravity);
	}

	// Loads object data from a file.
	void PlayerController::Deserialize(Parser& parser)
	{
		parser.ReadVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.ReadVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.ReadVariable("gravity", gravity);
	}

	// Initialize this component (happens at object creation).
	void PlayerController::Initialize()
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
	void PlayerController::Update(float dt)
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
		PlayerController* player = static_cast<PlayerController*>(object.GetComponent("PlayerController"));

		if (collision.bottom)
		{
			player->onGround = true;
		}

		if (collision.top)
		{
			player->jumping = false;
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
	void PlayerController::MoveHorizontal()
	{
		Vector2D move = physics->GetVelocity();
		move.x = 0.0f;

		if (Input::GetInstance().CheckHeld(VK_RIGHT))
		{
			move.x += PlayerWalkSpeed;

			if (onGround)
			{
				if (playerState == PlayerState::idleRt)
				{
					playerState = PlayerState::runRt;

				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::runRtUp;

				}
			}
		}
		else if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			move.x -= PlayerWalkSpeed;

			if (onGround)
			{
				if (playerState == PlayerState::idleRt)
				{
					playerState = PlayerState::runLt;

				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::runLtUp;

				}
			}
		}
		else
		{
			if (onGround)
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
	void PlayerController::MoveVertical()
	{
		Vector2D move = physics->GetVelocity();

		//jump movement
		if (jumping == true)
		{
			if (Input::GetInstance().CheckHeld(VK_SPACE))
			{
				if (transform->GetTranslation().y > jumpStartY + maxJumpHeight)
				{
					jumping = false;
				}

				move.y = PlayerJumpSpeed;	
			}
			else if (transform->GetTranslation().y > jumpStartY + maxJumpHeight / 2)
			{
				jumping = false;
			}
		}
		//gravity
		else
		{
			move.y = gravity.y;
		}	

		physics->SetVelocity(move);

		//jump start logic
		if (Input::GetInstance().CheckTriggered(VK_SPACE))
		{
			if (onGround == true)
			{
				onGround = false;
				jumping = true;
				jumpStartY = transform->GetTranslation().y;

				if (playerState == PlayerState::idleLt)
				{
					playerState = PlayerState::jumpLt;

				}

				if (playerState == PlayerState::idleLtUp)
				{
					playerState = PlayerState::jumpLtUp;

				}

				if (playerState == PlayerState::runLt || playerState == PlayerState::runLtUp)
				{
					playerState = PlayerState::jumpLtRoll;

				}

				if (playerState == PlayerState::idleRt)
				{
					playerState = PlayerState::idleRt;

				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::jumpRtUp;

				}

				if (playerState == PlayerState::runRt || playerState == PlayerState::runRtUp)
				{
					playerState = PlayerState::jumpRtRoll;

				}
			}
		}
	}

	// Shoot projectiles when enter is pressed
	void PlayerController::Shoot()
	{
		if (Input::GetInstance().CheckTriggered(VK_RETURN))
		{
			//initiliaze working data
			GameObject* bullet = new GameObject(*bulletArchetype);
			Transform* bulletTransform = static_cast<Transform*>(bullet->GetComponent("Transform"));
			Physics* bulletPhysics = static_cast<Physics*>(bullet->GetComponent("Physics"));
			Vector2D shootDirection;

			if (playerState == PlayerState::jumpLtRoll)
			{
				playerState = PlayerState::jumpLt;
			}

			if (playerState == PlayerState::jumpRtRoll)
			{
				playerState = PlayerState::jumpRt;
			}

			if (playerState == PlayerState::idleLt || playerState == PlayerState::runLt || playerState == PlayerState::jumpLt)
			{
				shootDirection = Vector2D(-1, 0);
			}

			if (playerState == PlayerState::idleRt || playerState == PlayerState::runRt || playerState == PlayerState::jumpRt)
			{
				shootDirection = Vector2D(1, 0);
			}

			if (playerState == PlayerState::idleLtUp || playerState == PlayerState::runLtUp || playerState == PlayerState::jumpLtUp ||
				playerState == PlayerState::idleRtUp || playerState == PlayerState::runRtUp || playerState == PlayerState::jumpRtUp)
			{
				shootDirection = Vector2D(0, 1);
			}
			

			//adjust bullet data
			bulletTransform->SetTranslation(transform->GetTranslation() + shootDirection.Normalized() * transform->GetScale().x / 2);
			bulletTransform->SetRotation(transform->GetRotation());
			bulletPhysics->SetOldTranslation(bulletTransform->GetTranslation());
			bulletPhysics->SetVelocity(shootDirection * bulletSpeed);
			//projectile->SetSpawner(this);

			GetOwner()->GetSpace()->GetObjectManager().AddObject(*bullet);
		}
	}
}