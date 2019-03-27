//==================================================================-
/*
/file   PlayerShip.cpp
/author Jakob McFarland
/date   10/18/2018
/brief

This is the implementation file for all member functions
of the class PlayerShip, as specified in specification
file PlayerShip.h.

*/
//==================================================================-

#include "stdafx.h"
#include "PlayerShip.h"
#include <Parser.h>
#include <GameObjectFactory.h>

#include "Space.h"
#include <Engine.h>
#include "SoundManager.h"
#include <Random.h>
#include "GameObjectManager.h"
#include <Input.h>
#include <Parser.h>
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "PlayerProjectile.h"

namespace Behaviors
{
	//==================================================================-
	// Public Functions:
	//==================================================================-
	
	void CollisionHandlerShip(GameObject& ship, GameObject& otherObject);

	// Constructor
	// Params:
	//   forwardThrust = Amount of force added to the ship when thrusters are activated.
	//   maximumSpeed  = Maximum attainable speed of the ship.
	//   rotationSpeed = Speed at which the ship rotates.
	//   bulletSpeed   = Speed at which bullets move when fired by ship.
	//   deathDuration = Length of death "animation" in seconds.
	PlayerShip::PlayerShip(float forwardThrust, float maximumSpeed,
		float rotationSpeed, float bulletSpeed, float deathDuration)
		: Component("PlayerShip"), forwardThrust(forwardThrust), maximumSpeed(maximumSpeed),
		rotationSpeed(rotationSpeed), bulletSpeed(bulletSpeed), deathDuration(deathDuration),
		score(0)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerShip::Clone() const 
	{
		return new PlayerShip(*this);
	}

	// Saves object data to a file.
	void PlayerShip::Serialize(Parser& parser) const
	{
		parser.WriteVariable("forwardThrust", forwardThrust);
		parser.WriteVariable("maximumSpeed", maximumSpeed);
		parser.WriteVariable("rotationSpeed", rotationSpeed);
		parser.WriteVariable("bulletSpeed", bulletSpeed);
		parser.WriteVariable("deathDuration", deathDuration);
	}
	
	// Loads object data from a file.
	void PlayerShip::Deserialize(Parser& parser)
	{
		parser.ReadVariable("forwardThrust", forwardThrust);
		parser.ReadVariable("maximumSpeed", maximumSpeed);
		parser.ReadVariable("rotationSpeed", rotationSpeed);
		parser.ReadVariable("bulletSpeed", bulletSpeed);
		parser.ReadVariable("deathDuration", deathDuration);
	}

	// Initialize this component (happens at object creation).
	void PlayerShip::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

		ColliderCircle* collider = GetOwner()->GetComponent<ColliderCircle>();
		if (collider != nullptr) collider->SetCollisionHandler(CollisionHandlerShip);

		bulletArchetype = GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName("Bullet");

		/*SoundManager* soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundEvent = soundManager->PlayEvent("Test Tones");

		soundEvent->setPaused(true);
		soundEvent->setVolume(0.5f);
		soundEvent->setParameterValue("Wave Type", 0);
		soundEvent->setParameterValue("LowMidHigh", 0);*/
	}
	
	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerShip::Update(float dt) 
	{
		UNREFERENCED_PARAMETER(dt);

		Move();
		Rotate();
		if(bulletArchetype != nullptr) 
			Shoot();		
	}

	//==================================================================-
	// Private Functions:
	//==================================================================-
	
	// Move forward when up arrow is pressed
	void PlayerShip::Move() const
	{
		if (Input::GetInstance().CheckHeld(VK_UP))
		{
			physics->AddForce( Vector2D::FromAngleRadians(transform->GetRotation()) * forwardThrust );

			//clamp velocity
			if (physics->GetVelocity().Magnitude() > maximumSpeed)
			{
				physics->SetVelocity(Vector2D::FromAngleRadians(transform->GetRotation()) * maximumSpeed);
			}

			//soundEvent->setPaused(false);
		}
		else
		{
			//soundEvent->setPaused(true);
		}
	}
	
	// Rotate when left or right arrow key is pressed
	void PlayerShip::Rotate() const
	{
		float newAngularVelocity = 0;

		if (Input::GetInstance().CheckHeld(VK_RIGHT))
		{
			newAngularVelocity -= rotationSpeed;  
		}

		if (Input::GetInstance().CheckHeld(VK_LEFT))
		{
			newAngularVelocity += rotationSpeed;
		}

		physics->SetAngularVelocity(newAngularVelocity);
	}
	
	// Shoot projectiles when space is pressed
	void PlayerShip::Shoot()
	{
		if (Input::GetInstance().CheckTriggered(VK_SPACE))
		{
			//initiliaze working data
 			GameObject* bullet = new GameObject(*bulletArchetype);

			Vector2D shipDirection = Vector2D::FromAngleRadians(transform->GetRotation());
			Transform* bulletTransform = static_cast<Transform*>( bullet->GetComponent("Transform") );
			Physics* bulletPhysics = static_cast<Physics*>( bullet->GetComponent("Physics"));
			Behaviors::PlayerProjectile* projectile = bullet->GetComponent<Behaviors::PlayerProjectile>();

			//adjust bullet data
			bulletTransform->SetTranslation(transform->GetTranslation() + shipDirection.Normalized() * transform->GetScale().x / 2);
			bulletTransform->SetRotation(transform->GetRotation());
			bulletPhysics->SetOldTranslation( bulletTransform->GetTranslation() );	
			bulletPhysics->SetVelocity(shipDirection * bulletSpeed);
			projectile->SetSpawner(this);

			GetOwner()->GetSpace()->GetObjectManager().AddObject(*bullet);
		}
	}

	// Returns player's score
	unsigned PlayerShip::GetScore() const
	{
		return score;
	}

	// Increase player's score
	// Params:
	//   amount = This value is added to the player's current score.
	void PlayerShip::IncreaseScore(unsigned amount)
	{
		score += amount;
	}
	
	// Play death "animation"
	void PlayerShip::DeathSequence(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		GetOwner()->GetComponent<Sprite>()->SetColor(Colors::Red);
	}

	// Collision start event handler.
	// Params:
	//	 ship = The player ship.
	//   otherObject = The other object.
	void CollisionHandlerShip(GameObject& ship, GameObject& otherObject)
	{
		UNREFERENCED_PARAMETER(ship);
		UNREFERENCED_PARAMETER(otherObject);

		if (otherObject.GetName() == "Walls")
		{

		}

	}
}