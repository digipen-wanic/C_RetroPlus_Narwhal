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
#include "ColliderRectangle.h"
#include "Input.h"
#include "Transform.h"
#include "Physics.h"
#include "Space.h"
#include "Graphics.h"

#include "Sprite.h"
#include "Mesh.h"
#include "Animation.h"
#include "SpriteSource.h"
#include "StartScreen.h"

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
	PlayerController::PlayerController()
	: Component("PlayerController"), PlayerWalkSpeed(3.0f * tileUnit),
		PlayerJumpSpeed(5.0f * tileUnit), gravity(0.0f, -10.0f * tileUnit),
		maxJumpHeight( 5 * tileUnit ), firingSpeed(0.2f), firingTimer(0), 
		bulletSpeed(tileUnit * 6), onGround(false), jumping(false), maxGravity(-4.0f),
		playerState(PlayerState::idleRt), footstepInterval(0.15f), footstepTimer(0.0f)
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
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
		sprite = GetOwner()->GetComponent<Sprite>();
		animation = GetOwner()->GetComponent<Animation>();
		resourceManager = &GetOwner()->GetSpace()->GetResourceManager();

		Collider* collider = static_cast<Collider*>( GetOwner()->GetComponent("Collider") );
		colliderRect = GetOwner()->GetComponent<ColliderRectangle>();

		collider->SetMapCollisionHandler( PlayerMapCollisionHandler );
		collider->SetCollisionHandler( PlayerCollisionHandler );
	
		soundManager = Engine::GetInstance().GetModule<SoundManager>();

		bulletArchetype = GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName("samusBullet");
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerController::Update(float dt)
	{
		MoveHorizontal(dt);
		MoveVertical();	
		Shoot(dt);
	}

	// Map collision handler for Player objects.
	// Params:
	//   object = The Player object.
	//   collision = Which sides the Player collided on.
	void PlayerMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		PlayerController* player = object.GetComponent<PlayerController>();

		if (collision.bottom && player->jumping == false)
		{
			player->onGround = true;

			if (player->playerState == PlayerState::jumpLt || player->playerState == PlayerState::jumpLtUp || player->playerState == PlayerState::jumpLtRoll)
			{
				player->playerState = PlayerState::idleLt;
				player->sprite->SetSpriteSource(player->resourceManager->GetSpriteSource("SamusIdle"));
				player->sprite->SetMesh(player->resourceManager->GetMesh("SamusIdle"));

				std::cout << "ln 134" << std::endl;
			}

			if (player->playerState == PlayerState::jumpRt || player->playerState == PlayerState::jumpRtUp || player->playerState == PlayerState::jumpRtRoll)
			{
				player->playerState = PlayerState::idleRt;
				player->sprite->SetSpriteSource(player->resourceManager->GetSpriteSource("SamusIdle"));
				player->sprite->SetMesh(player->resourceManager->GetMesh("SamusIdle"));
			}
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

		if (other.GetName() == "Door")
		{
			object.GetSpace()->SetLevel(new Levels::StartScreen);
		}
	}

	//==================================================================-
	// Private Methods:
	//==================================================================-

	// Moves horizontally based on input
	void PlayerController::MoveHorizontal(float dt)
	{
		Vector2D move = physics->GetVelocity();
		move.x = 0.0f;

		//foot steps
		if (onGround && playerState != PlayerState::rollLt && playerState != PlayerState::rollRt &&
			(Input::GetInstance().CheckHeld(VK_RIGHT) || Input::GetInstance().CheckHeld(VK_LEFT)))
		{
			footstepTimer += dt;
			if (footstepTimer >= footstepInterval)
			{
				soundManager->PlaySound("PlayerRun2FX.wav")->setVolume(0.1f);
				footstepTimer = 0;
			}
		}

		if (Input::GetInstance().CheckHeld(VK_RIGHT))
		{
			move.x += PlayerWalkSpeed;

			if (playerState == PlayerState::idleRt || playerState == PlayerState::idleLt)
			{
				playerState = PlayerState::runRt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRun"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRun"));
				animation->Play(0.08f, true);
			}

			if (playerState == PlayerState::idleRtUp || playerState == PlayerState::idleLtUp)
			{
				playerState = PlayerState::runRtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRunUp"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRunUp"));
				animation->Play(0.08f, true);
			}

			//don't change sprite
			if (playerState == PlayerState::jumpRt || playerState == PlayerState::jumpLt)
			{
				playerState = PlayerState::jumpRt;
			}

			if (playerState == PlayerState::jumpRtUp || playerState == PlayerState::jumpLtUp)
			{
				playerState = PlayerState::jumpRtUp;
			}

			if (playerState == PlayerState::jumpRtRoll || playerState == PlayerState::jumpLtRoll)
			{
				playerState = PlayerState::jumpRtRoll;
			}	

			//improve!
			Vector2D scale = transform->GetScale();
			scale.x = std::abs(scale.x);
			transform->SetScale(scale);
		}
		else if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			//add delay
			//soundManager->PlaySound("PlayerRun2FX.wav");

			move.x -= PlayerWalkSpeed;

			if (playerState == PlayerState::idleRt || playerState == PlayerState::idleLt)
			{
				playerState = PlayerState::runLt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRun"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRun"));
				animation->Play(0.08f, true);
			}

			if (playerState == PlayerState::idleLtUp || playerState == PlayerState::idleLtUp)
			{
				playerState = PlayerState::runLtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRunUp"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRunUp"));
				animation->Play(0.08f, true);
			}

			if (playerState == PlayerState::jumpRt || playerState == PlayerState::jumpLt)
			{
				playerState = PlayerState::jumpLt;
			}

			if (playerState == PlayerState::jumpRtUp || playerState == PlayerState::jumpLtUp)
			{
				playerState = PlayerState::jumpLtUp;
			}

			if (playerState == PlayerState::jumpRtRoll || playerState == PlayerState::jumpLtRoll)
			{
				playerState = PlayerState::jumpLtRoll;
			}

			//Improve!
			Vector2D scale = transform->GetScale();
			scale.x = -std::abs(scale.x);
			transform->SetScale(scale);
		}
		else
		{
			if (playerState == PlayerState::runLt)
			{
				playerState = PlayerState::idleLt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));
			}

			if (playerState == PlayerState::runLtUp)
			{
				playerState = PlayerState::idleLtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));
			}

			if (playerState == PlayerState::runRt)
			{
				playerState = PlayerState::idleRt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));
			}

			if (playerState == PlayerState::runRtUp)
			{
				playerState = PlayerState::idleRtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));
			}		
		}

		if (Input::GetInstance().CheckTriggered (VK_UP))
		{
			if (playerState == PlayerState::idleLt)
			{
				playerState = PlayerState::idleLtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdleUp"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdleUp"));
			}

			if (playerState == PlayerState::runLt)
			{
				playerState = PlayerState::runLtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRunUp"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRunUp"));
				animation->Play(0.08f, true);
			}
	
			if (playerState == PlayerState::idleRt)
			{
				playerState = PlayerState::idleRtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdleUp"));

				sprite->SetMesh(resourceManager->GetMesh("SamusIdleUp"));
			}

			if (playerState == PlayerState::runRt)
			{
				playerState = PlayerState::runRtUp;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRunUp"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRunUp"));
				animation->Play(0.08f, true);
			}

			if (playerState == PlayerState::rollLt)
			{
				playerState = PlayerState::idleLt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));

				Vector2D scale = transform->GetScale();
				scale.y *= 2;
				transform->SetScale(scale);
				colliderRect->SetExtents(Vector2D(transform->GetScale().x * 0.5f, transform->GetScale().y * 0.5f));				
			}

			if (playerState == PlayerState::rollRt)
			{
				playerState = PlayerState::idleRt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));

				Vector2D scale = transform->GetScale();
				scale.y *= 2;
				transform->SetScale(scale);
				colliderRect->SetExtents(Vector2D(transform->GetScale().x * 0.5f, transform->GetScale().y * 0.5f));
			}
		}

		if (Input::GetInstance().CheckReleased(VK_UP))
		{
			if (playerState == PlayerState::idleLtUp)
			{
				playerState = PlayerState::idleLt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));
			}

			if (playerState == PlayerState::runLtUp)
			{
				playerState = PlayerState::runLt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRun"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRun"));
				animation->Play(0.08f, true);
			}

			if (playerState == PlayerState::idleRtUp)
			{
				playerState = PlayerState::idleRt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusIdle"));
				sprite->SetMesh(resourceManager->GetMesh("SamusIdle"));
			}

			if (playerState == PlayerState::runRtUp)
			{
				playerState = PlayerState::runRt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRun"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRun"));
				animation->Play(0.08f, true);
			}
		}

		if (Input::GetInstance().CheckTriggered(VK_DOWN))
		{
			if (playerState == PlayerState::idleLt )
			{
				playerState = PlayerState::rollLt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRoll"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRoll"));
				animation->Play(0.08f, true);

				Vector2D scale = transform->GetScale();
				scale.y /= 2;
				transform->SetScale(scale);
				colliderRect->SetExtents(Vector2D(transform->GetScale().x * 0.5f, transform->GetScale().y * 0.5f));
				
				//transform->SetTranslation( transform->GetTranslation() + Vector2D(0.0f, 0.5f) );
			}

			if (playerState == PlayerState::idleRt)
			{
				playerState = PlayerState::rollRt;
				sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusRoll"));
				sprite->SetMesh(resourceManager->GetMesh("SamusRoll"));
				animation->Play(0.08f, true);

				Vector2D scale = transform->GetScale();
				scale.y /= 2;
				transform->SetScale(scale);
				colliderRect->SetExtents(Vector2D(transform->GetScale().x * 0.5f, transform->GetScale().y * 0.5f));
				//transform->SetTranslation(transform->GetTranslation() + Vector2D(0.0f, 0.5f));
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
			else if (transform->GetTranslation().y > jumpStartY + maxJumpHeight * 2 / 5)
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
			if (onGround == true && jumping == false && 
				playerState != PlayerState::rollLt && playerState != PlayerState::rollRt)
			{
				//FMOD::Channel* jumpSound = 
				soundManager->PlaySound("PlayerJump.wav")->setVolume(0.2f);

				onGround = false;
				jumping = true;
				jumpStartY = transform->GetTranslation().y;

				if (playerState == PlayerState::idleLt)
				{
					playerState = PlayerState::jumpLt;
					sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJump"));
					sprite->SetMesh(resourceManager->GetMesh("SamusJump"));
					sprite->SetFrame(0);
				}

				if (playerState == PlayerState::idleLtUp)
				{
					playerState = PlayerState::jumpLtUp;
					sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJump"));
					sprite->SetMesh(resourceManager->GetMesh("SamusJump"));	
					sprite->SetFrame(0);
				}

				if (playerState == PlayerState::runLt || playerState == PlayerState::runLtUp)
				{
					playerState = PlayerState::jumpLtRoll;
					sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJumpRoll"));
					sprite->SetMesh(resourceManager->GetMesh("SamusJumpRoll"));
					animation->Play(0.08f, true);

					std::cout << "ln 424" << std::endl;
				}

				if (playerState == PlayerState::idleRt)
				{
					playerState = PlayerState::jumpRt;
					sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJump"));
					sprite->SetMesh(resourceManager->GetMesh("SamusJump"));
					sprite->SetFrame(0);
				}

				if (playerState == PlayerState::idleRtUp)
				{
					playerState = PlayerState::jumpRtUp;
					sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJump"));
					sprite->SetMesh(resourceManager->GetMesh("SamusJump"));
					sprite->SetFrame(0);
				}

				if (playerState == PlayerState::runRt || playerState == PlayerState::runRtUp)
				{
					playerState = PlayerState::jumpRtRoll;
					sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJumpRoll"));
					sprite->SetMesh(resourceManager->GetMesh("SamusJumpRoll"));
					animation->Play(0.08f, true);
				}
			}
		}
	}

	// Shoot projectiles when enter is pressed
	void PlayerController::Shoot(float dt)
	{
		//Shooting
		if (playerState != PlayerState::rollLt && playerState != PlayerState::rollRt)
		{
			if (Input::GetInstance().CheckTriggered('Z'))
			{
				Fire();
			}

			if (Input::GetInstance().CheckHeld('Z'))
			{
				firingTimer += dt;

				if (firingTimer >= firingSpeed)
				{
					firingTimer = 0;
					Fire();
				}
			}

			if (Input::GetInstance().CheckReleased('Z'))
			{
				firingTimer = -0.2f;
			}
		}
	}

	void PlayerController::Fire()
	{
		//initiliaze working data

		soundManager->PlaySound("PlayerFire.wav")->setVolume(0.2f);

		GameObject* bullet = new GameObject(*bulletArchetype);
		Transform* bulletTransform = bullet->GetComponent<Transform>();
		Physics* bulletPhysics = bullet->GetComponent<Physics>();
		Vector2D shootDirection;

		if (playerState == PlayerState::jumpLtRoll)
		{
			playerState = PlayerState::jumpLt;
			sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJump"));
			sprite->SetMesh(resourceManager->GetMesh("SamusJump"));
		}

		if (playerState == PlayerState::jumpRtRoll)
		{
			playerState = PlayerState::jumpRt;
			sprite->SetSpriteSource(resourceManager->GetSpriteSource("SamusJump"));
			sprite->SetMesh(resourceManager->GetMesh("SamusJump"));
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
		bulletTransform->SetTranslation(transform->GetTranslation() + Vector2D( 0, transform->GetScale().y / 4) + shootDirection.Normalized() * transform->GetScale().x / 2);
		bulletTransform->SetRotation(transform->GetRotation());
		bulletPhysics->SetOldTranslation(bulletTransform->GetTranslation());
		bulletPhysics->SetVelocity(shootDirection * bulletSpeed + Vector2D( physics->GetVelocity().x, 0.0f) );
		//projectile->SetSpawner(this);

		GetOwner()->GetSpace()->GetObjectManager().AddObject(*bullet);
	}
}