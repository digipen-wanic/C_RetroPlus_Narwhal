//==================================================================-
/*
/file   Asteroid.cpp
/author Jakob McFarland
/date   10/18/2018
/brief

This is the implementation file for all member functions
of the class Asteroid, as specified in specification
file Asteroid.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Asteroid.h"
//#include "PlayerProjectile.h"

#include <Component.h>
#include <Random.h>
#include <Space.h>
#include <Engine.h>
//#include <EventManager.h>
#include <Graphics.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderCircle.h>

namespace Behaviors
{
	//==================================================================-
	// Static Data:
	//==================================================================-

	//unsigned Asteroid::instanceCount = 0;

	//==================================================================-
	// Public Functions:
	//==================================================================-

	void CollisionHandlerAsteroid(GameObject& asteroid, GameObject& otherObject);

	// Asteroid constructor
	// Params:
	//   speedMin = Minimum of range for asteroid movement speed.
	//   speedMax = Maximum of range for asteroid movement speed.
	//   basePointsValue = Point value for largest asteroid.
	//   sizePointsModifier = Multiplicative scale applied to points for smaller asteroids.
	//   spawnScaleModifier = Percentage of original asteroid's scale to use for new asteroids.
	Asteroid::Asteroid(float speedMin, float speedMax, unsigned basePointsValue,
		float sizePointsModifier, float spawnScaleModifier)
		: Component("Asteroid"), speedMin(speedMin), speedMax(speedMax), basePointsValue(basePointsValue),
		sizePointsModifier(sizePointsModifier), spawnScaleModifier(spawnScaleModifier), size(SizeLarge),
		location(LocationTopLeft)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Asteroid::Clone() const
	{
		//++instanceCount;
		return new Asteroid(*this);
	}

	// Initialize this component (happens at object creation).
	void Asteroid::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

		//register our collision event
		ColliderCircle* collider = GetOwner()->GetComponent<ColliderCircle>();
		if (collider != nullptr) collider->SetCollisionHandler(CollisionHandlerAsteroid);

		SetPosition();
		SetVelocity();
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Asteroid::Serialize(Parser& parser) const
	{
		//Stub
		UNREFERENCED_PARAMETER(parser);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Asteroid::Deserialize(Parser& parser)
	{
		//Stub
		UNREFERENCED_PARAMETER(parser);
	}

	// Get the value of this object for increasing the player's score.
	unsigned Asteroid::GetPointValue() const
	{
  		switch (size)
		{
			case SizeLarge:
				return basePointsValue;
			case SizeMedium:
				return basePointsValue + static_cast<unsigned>(sizePointsModifier);
			case SizeSmall:
				return basePointsValue + static_cast<unsigned>(sizePointsModifier * sizePointsModifier);
		}
		return 0;
	}

	// Retrieve number of objects with this component
	//unsigned Asteroid::GetInstanceCount()
	//{
		//return instanceCount;
	//}

	//==================================================================-
	// Private Functions:
	//==================================================================-

	// The collision handling function for Asteroids.
	// Params:
	//	 asteroid = The asteroid.
	//   otherObject = The other object.
	void CollisionHandlerAsteroid(GameObject& asteroid, GameObject& otherObject)
	{
		if (otherObject.GetName() == "Bullet" ||otherObject.GetName() == "PlayerShip"
			|| otherObject.GetName() == "Missile")
		{
			asteroid.GetComponent<Asteroid>()->SpawnNewAsteroids();
			asteroid.Destroy();
		}
	}

	// Set position of asteroid based on origin
	void Asteroid::SetPosition()
	{
		if (size == SizeLarge)
		{
			BoundingRectangle screenBox = Graphics::GetInstance().GetScreenWorldDimensions();

			float offset = 15.0f;

			int rCorner = RandomRange(0, 3);

			switch (rCorner)
			{
			case 0:
				transform->SetTranslation(Vector2D(screenBox.left + offset, screenBox.top - offset));
				break;
			case 1:
				transform->SetTranslation(Vector2D(screenBox.right - offset, screenBox.top - offset));
				break;
			case 2:
				transform->SetTranslation(Vector2D(screenBox.left + offset, screenBox.bottom + offset));
				break;
			case 3:
				transform->SetTranslation(Vector2D(screenBox.right - offset, screenBox.bottom + offset));
				break;
			}
		}

		GetOwner()->GetComponent<Physics>()->SetOldTranslation( GetOwner()->GetComponent<Transform>()->GetTranslation() );
	}

	// Set velocity based on size
	void Asteroid::SetVelocity()
	{
		float rTheta = RandomRange(0.0f, static_cast<float>(M_PI) * 2.0f);
		float rSpeed = RandomRange(speedMin, speedMax);

		physics->SetVelocity(Vector2D::FromAngleRadians(rTheta) * rSpeed);
	}

	// Generate new asteroids based off this asteroid
	void Asteroid::SpawnNewAsteroids()
	{
		switch (size)
		{
			int rSpawns;
			GameObject* asteroid;

		case SizeLarge:

			rSpawns = RandomRange(2, 3);

			for (int i = 0; i < rSpawns; ++i)
			{
				asteroid = new GameObject(*GetOwner());

				asteroid->GetComponent<Asteroid>()->size = SizeMedium;
				asteroid->GetComponent<Transform>()->SetScale(asteroid->GetComponent<Transform>()->GetScale() * spawnScaleModifier);
				asteroid->GetComponent<ColliderCircle>()->SetRadius(asteroid->GetComponent<Transform>()->GetScale().x / 2.0f);

				asteroid->GetComponent<Asteroid>()->SetVelocity();
				GetOwner()->GetSpace()->GetObjectManager().AddObject(*asteroid);
			}

			break;

		case SizeMedium:

			rSpawns = RandomRange(1, 2);

			for (int i = 0; i < rSpawns; ++i)
			{
				asteroid = new GameObject(*GetOwner());

				asteroid->GetComponent<Asteroid>()->size = SizeSmall;
				asteroid->GetComponent<Transform>()->SetScale(asteroid->GetComponent<Transform>()->GetScale() * spawnScaleModifier);
				asteroid->GetComponent<ColliderCircle>()->SetRadius(asteroid->GetComponent<Transform>()->GetScale().x / 2.0f);

				asteroid->GetComponent<Asteroid>()->SetVelocity();
				GetOwner()->GetSpace()->GetObjectManager().AddObject(*asteroid);
			}

			break;
		}
	}
}