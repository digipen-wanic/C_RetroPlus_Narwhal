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

#include "Sprite.h"
#include "Mesh.h"
#include "SpriteSource.h"

#include <SoundManager.h>
#include <Engine.h>

namespace Behaviors
{
	//==================================================================-
	// Public methods:
	//==================================================================-

	void PlayerCollisionHandler(GameObject& object, GameObject& other);

	void PlayerMapCollisionHandler(GameObject& object,
		const MapCollision& collision);

	// Constructor
	PlayerController::PlayerController(Mesh* idleMesh, Mesh* runMesh, SpriteSource* standing, SpriteSource* idle,
		SpriteSource* run, SpriteSource* runShoot, SpriteSource* jump,
		SpriteSource* jumpRoll, SpriteSource* roll)
	: Component("PlayerController"), PlayerWalkSpeed(2.5f * tileUnit),
		PlayerJumpSpeed(5.0f * tileUnit), gravity(0.0f, -10.0f * tileUnit),
		maxJumpHeight( 4 * tileUnit ), firingSpeed(0.25), firingTimer(0), 
		bulletSpeed(tileUnit * 4), onGround(false), jumping(false), maxGravity(-4.0),
		idleMesh(idleMesh), runMesh(runMesh), standing(standing), idle(idle), run(run), runShoot(runShoot), 
		jump(jump), jumpRoll(jumpRoll), roll(roll)
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
	
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
	
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
				soundManager->PlaySound("PlayerRun2FX.wav");

				if (playerState == PlayerState::idleRt)
				{
					playerState = PlayerState::runRt;
					sprite->SetSpriteSource(run);
				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::runRtUp;
					sprite->SetSpriteSource(run);
				}

				//improve!
				Vector2D scale = transform->GetScale();
				scale.x = std::abs(scale.x);
				transform->SetScale(scale);

				sprite->SetMesh(runMesh);
			}
		}
		else if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			move.x -= PlayerWalkSpeed;

			if (onGround)
			{
				if (playerState == PlayerState::idleLt)
				{
					playerState = PlayerState::runLt;
					sprite->SetSpriteSource(run);
				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::runRtUp;
					sprite->SetSpriteSource(run);
				}

				//Improve!
				Vector2D scale = transform->GetScale();
				scale.x = - std::abs( scale.x);
				transform->SetScale( scale );

				//sprite->SetMesh(runMesh);
			}
		}
		else
		{
			if (onGround)
			{
				if (playerState == PlayerState::runLt)
				{
					playerState = PlayerState::idleLt;
					sprite->SetSpriteSource(idle);
				}

				if (playerState == PlayerState::runRt)
				{
					playerState = PlayerState::idleRt;
					sprite->SetSpriteSource(idle);
				}

				//sprite->SetMesh(idleMesh);
			}
		}

		if (Input::GetInstance().CheckTriggered (VK_UP))
		{
			if (playerState == PlayerState::idleLt)
			{
				playerState = PlayerState::idleLtUp;
				//sprite->SetSpriteSource(run);
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
				sprite->SetSpriteSource(idle);
			}

			if (playerState == PlayerState::runLtUp)
			{
				playerState = PlayerState::runLt;
				sprite->SetSpriteSource(run);
			}

			if (playerState == PlayerState::idleRtUp)
			{
				playerState = PlayerState::idleRt;
				sprite->SetSpriteSource(idle);
			}

			if (playerState == PlayerState::runRtUp)
			{
				playerState = PlayerState::runRt;
				sprite->SetSpriteSource(run);
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
			if (Input::GetInstance().CheckHeld('X'))
			{
				if (transform->GetTranslation().y > jumpStartY + maxJumpHeight)
				{
					jumping = false;
				}

				move.y = PlayerJumpSpeed;	
				physics->SetVelocity(move);
			}
			else if (transform->GetTranslation().y > jumpStartY + maxJumpHeight / 2)
			{
				jumping = false;
			}
		}
		//gravity
		else
		{
			//move.y = gravity.y;
			physics->AddForce(gravity);

			if (move.y >= maxGravity)
			{
				move.y = maxGravity;
			}
		}	

		physics->SetVelocity(move);

		//jump start logic
		if (Input::GetInstance().CheckTriggered('X'))
		{
			if (onGround == true)
			{
				onGround = false;
				jumping = true;
				jumpStartY = transform->GetTranslation().y;

				if (playerState == PlayerState::idleLt)
				{
					playerState = PlayerState::jumpLt;
					sprite->SetSpriteSource(jump);
				}

				if (playerState == PlayerState::idleLtUp)
				{
					playerState = PlayerState::jumpLtUp;
					sprite->SetSpriteSource(jump);
				}

				if (playerState == PlayerState::runLt || playerState == PlayerState::runLtUp)
				{
					playerState = PlayerState::jumpLtRoll;
					sprite->SetSpriteSource(jumpRoll);
				}

				if (playerState == PlayerState::idleRt)
				{
					playerState = PlayerState::idleRt;
					sprite->SetSpriteSource(jump);
				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::jumpRtUp;
					sprite->SetSpriteSource(jump);
				}

				if (playerState == PlayerState::runRt || playerState == PlayerState::runRtUp)
				{
					playerState = PlayerState::jumpRtRoll;
					sprite->SetSpriteSource(jumpRoll);
				}
			}
		}
	}

	// Shoot projectiles when enter is pressed
	void PlayerController::Shoot()
	{
		if (Input::GetInstance().CheckTriggered('Z'))
		{
			//initiliaze working data

			soundManager->PlaySound("PlayerFire.wav");

			GameObject* bullet = new GameObject(*bulletArchetype);
			Transform* bulletTransform = static_cast<Transform*>(bullet->GetComponent("Transform"));
			Physics* bulletPhysics = static_cast<Physics*>(bullet->GetComponent("Physics"));
			Vector2D shootDirection;

			if (playerState == PlayerState::jumpLtRoll)
			{
				playerState = PlayerState::jumpLt;
				sprite->SetSpriteSource(jump);
			}

			if (playerState == PlayerState::jumpRtRoll)
			{
				playerState = PlayerState::jumpRt;
				sprite->SetSpriteSource(jump);
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