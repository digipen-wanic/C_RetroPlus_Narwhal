//------------------------------------------------------------------------------
//
// File Name:	ColorChange.h
// Author(s):	William Siauw
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Enemy.h" // base class
#include "Sprite.h"
#include <Random.h>
#include <Engine.h>
#include "GameObject.h"
#include "Space.h"
#include "Collider.h"
#include "ColliderTilemap.h"
#include <Tilemap.h>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Sprite;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	enum EnemyStates
	{
		StateIdle,
		StateWander,
		StateChase
	};

	// Constructor
	Enemy::Enemy()
		:Component("Enemy"), direction(1, 0), currentState(StateIdle), nextState(StateIdle), innerState(InnerStateEnter), colliding(false)
	{

	}
	// Constructor
	Enemy::Enemy(GameObject* t)
		: Component("Enemy"), direction(1,0), currentState(StateIdle), nextState(StateIdle), innerState(InnerStateEnter), tilemap(t)
	{

	}

	// Return a new copy of the component.
	Component* Enemy::Clone() const
	{
		Enemy* e = new Enemy(*this);
		return e;
	}

	// Collision handler for enemy.
	// Params:
	//   object = The enemy.
	//   other  = The object the monkey is colliding with.
	void EnemyCollisionHandler(GameObject& object, GameObject& other)
	{
		if (other.GetName() == "Monkey")
		{
			object.GetSpace()->RestartLevel();
		}
	}

	// Map collision handler for Monkey objects.
	// Params:
	//   object = The monkey object.
	//   collision = Which sides the monkey collided on.
	void EnemyMapCollisionHandler(GameObject& object,
		const MapCollision& collision)
	{
		UNREFERENCED_PARAMETER(object);
		if (collision.left)
		{
		}	//static_cast<Enemy*>(object.GetComponent("Enemy"))->SetDirection(1);
		else if (collision.right)
		{
		}	//static_cast<Enemy*>(object.GetComponent("Enemy"))->SetDirection(-1);
	}

	// Initialize data for this object.
	void Enemy::Initialize()
	{
		transform = static_cast<Transform*>(GetOwner()->GetComponent("Transform"));
		playerTransform = static_cast<Transform*>(GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("Monkey")->GetComponent("Transform"));
		physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics"));

		// Set the collision handlers
		static_cast<Collider*>(GetOwner()->GetComponent("Collider"))->SetMapCollisionHandler(EnemyMapCollisionHandler);
		static_cast<Collider*>(GetOwner()->GetComponent("Collider"))->SetCollisionHandler(EnemyCollisionHandler);
	}

	// Sets the direction of the enemy.
	// Params:
	//   dir = The direction the enemy should travel in.
	void Enemy::SetDirection(Vector2D dir)
	{
		direction = dir;	
	}
	Vector2D Enemy::GetTileIndicesOfFront(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		//NewIndices.y++;
		NewIndices.x ++;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfBelow(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		NewIndices.y++;
		//NewIndices.x += direction.x *0.5f;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfAbove(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		NewIndices.y--;
		//NewIndices.x += direction.x *0.5f;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfBack(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		//NewIndices.y++;
		NewIndices.x --;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfTopRightCorner(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		NewIndices.y--;
		NewIndices.x++;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfTopLeftCorner(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		NewIndices.y--;
		NewIndices.x--;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfBottomRightCorner(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		NewIndices.y++;
		NewIndices.x++;
		return NewIndices;
	}
	Vector2D Enemy::GetTileIndicesOfBottomLeftCorner(Vector2D indices) const
	{
		Vector2D NewIndices = indices;
		NewIndices.y++;
		NewIndices.x--;
		return NewIndices;
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Enemy::Update(float dt)
	{
		Vector2D tilemapCoords;	//	Tilemap Coordinates of the tile 1 in front and 1 below the enemy
		Vector2D curCoords;	//	Tilemap Coordinates of the enemy
		int valueOfFront;	//	Cell value of tile in front
		int valueOfBack;	//	Cell value of tile behind
		int valueOfAbove;	//	Cell value of tile Above
		int valueOfBelow;	//	Cell value of tile below
		int valueOfTopRight;	//	Cell value of tile in front
		int valueOfTopLeft;	//	Cell value of tile behind
		int valueOfBottomRight;	//	Cell value of tile Above
		int valueOfBottomLeft;	//	Cell value of tile below
		int cX;
		int cY;
		Vector2D distFromWhole;
		switch (currentState)
		{
		case StateIdle:
		{
			switch (innerState)
			{
			case InnerStateEnter:
				innerState = InnerStateUpdate;
				break;
			case InnerStateUpdate:
				innerState = InnerStateExit;
				nextState = StateWander;
				break;
			case InnerStateExit:
				currentState = nextState;
				innerState = InnerStateEnter;
				break;
			}
			break;
		}

		case StateWander:
			switch (innerState)
			{
			case InnerStateEnter:
				// TO DO: Pick direction to wander

				innerState = InnerStateUpdate;
				break;
			case InnerStateUpdate:
				curCoords = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTileIndices(transform->GetTranslation()) - Vector2D(0.5f, 0.5f);
				
				//	Get how far the enemy is from being fully encompassed in a tile, how far the translation is from a whole number
				cX = static_cast<int>(curCoords.x);
				cY = static_cast<int>(curCoords.y);	
				distFromWhole = curCoords - Vector2D(static_cast<float>(cX), static_cast<float>(cY));
				
				tilemapCoords = GetTileIndicesOfFront(curCoords);
				valueOfFront = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfBack(curCoords);
				valueOfBack = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfAbove(curCoords);
				valueOfAbove = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfBelow(curCoords);
				valueOfBelow = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfBottomLeftCorner(curCoords);
				valueOfBottomLeft = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfTopLeftCorner(curCoords);
				valueOfTopLeft = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfTopRightCorner(curCoords);
				valueOfTopRight = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				tilemapCoords = GetTileIndicesOfBottomRightCorner(curCoords);
				valueOfBottomRight = static_cast<ColliderTilemap*>(tilemap->GetComponent("Collider"))->GetTilemap()->GetCellValue(static_cast<int>(tilemapCoords.x), static_cast<int>(tilemapCoords.y));
				if (distFromWhole.x <= 0.1f && distFromWhole.y <= 0.1f)
				{
					//	DOES NOT WORK, EITHER CORNER OR INVERTED CORNER DOES NOT WORK TOGETHER
					if (valueOfBelow == 0 && (/*valueOfFront != 0 || */valueOfBottomLeft != 0))
						direction =  Vector2D(0.0f, -1.0f);
					if (valueOfBack == 0 && (/*valueOfBelow != 0 || */valueOfTopLeft != 0))
						direction = Vector2D(-1.0f, 0.0f);
					if (valueOfAbove == 0 && (/*valueOfBack != 0 || */valueOfTopRight != 0))
						direction = Vector2D(0.0f, 1.0f);
					if (valueOfFront == 0 && (/*valueOfAbove != 0 || */valueOfBottomRight != 0))
						direction = Vector2D(1.0f, 0.0f);
				}
				physics->SetVelocity(Vector2D(direction.x * 100, direction.y * 100));
				break;
			case InnerStateExit:
				currentState = nextState;
				innerState = InnerStateEnter;
				break;
			}
			break;
		}

		timer += dt;
	}
	// Sets the next state of the enemy.
	// Params:
	//   nextState = The next state the enemy should be in.
	void Enemy::SetState(unsigned nextState_)
	{
		nextState = nextState_;
	}
}
