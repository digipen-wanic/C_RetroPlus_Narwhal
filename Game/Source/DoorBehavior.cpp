//------------------------------------------------------------------------------
//
// File Name:	DoorBehavior.cpp
// Author(s):	Nicole Smith
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Component.h" // base class
#include "DoorBehavior.h"
#include <Transform.h>
#include <Sprite.h>
#include <Animation.h>
#include <GameObject.h>
#include <Collider.h>
#include <Animation.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	void DoorCollisionHandler(GameObject& object, GameObject& other);
	// Constructor
	// Params:
	//   normalColor = The color that the object reverts to when not colliding.
	//   collidedColor = The color that the object changes to when colliding.
	//   collidedColorTime = The amount of time the object will retain the collided color.
	DoorBehavior::DoorBehavior(float doorOpenTime, float doorCloseTime)
		:	Component("DoorBehavior"), sprite(sprite), doorOpenTime(doorOpenTime), doorCloseTime(doorCloseTime), timer(0.0f),open(false)
			
	{
	}

	// Return a new copy of the component.
	Component* DoorBehavior::Clone() const
	{
		return new Behaviors::DoorBehavior(*this);
	}

	// Initialize data for this object.
	void DoorBehavior::Initialize()
	{
		sprite = GetOwner()->GetComponent<Sprite>();

		static_cast<Collider*>(GetOwner()->GetComponent("Collider"))->SetCollisionHandler(DoorCollisionHandler);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void DoorBehavior::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}
	void DoorBehavior::TransportToLevel()
	{
		std::cout << "ITS GO TIME!!!" << std::endl;
	}
	// Collision handler for ColorChange objects.
	// Params:
	//   object = The first object.
	//   other  = The other object the first object is colliding with.
	void DoorCollisionHandler(GameObject& object, GameObject& other)
	{
		//if hit by a bullet
		if (other.GetName() == "samusBullet")
		{
			if (!object.GetComponent<DoorBehavior>()->open)
			{
				object.GetComponent<DoorBehavior>()->open = true;
			}
		}
		if (other.GetName() == "Samus")
		{
			if (object.GetComponent<DoorBehavior>()->open == true)
			{
				object.GetComponent<DoorBehavior>()->TransportToLevel();
			}
		}
		if(other.GetName() == "Samus")
			other.GetComponent<Transform>()->SetTranslation(Vector2D(object.GetComponent<Transform>()->GetTranslation().x - .5f * (object.GetComponent<Transform>()->GetScale().x + .5f * (other.GetComponent<Transform>()->GetScale().x)), other.GetComponent<Transform>()->GetTranslation().y));
	}

}
