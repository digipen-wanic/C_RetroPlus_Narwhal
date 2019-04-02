//------------------------------------------------------------------------------
//
// File Name:	Archetypes.h
// Author(s):	Jeremy Kings (j.kings)
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class GameObject;
class SpriteSource;
class Mesh;
class Tilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Archetypes
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
		
	// Create the player game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject * CreateSamus(Mesh * standingMesh, SpriteSource* standing);

	// Create the player bullet game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject * CreateSamusBulletArchetype(Mesh* mesh, SpriteSource* spriteSource);

	// Create the ship game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateShip(Mesh* mesh);

	// Create the archetype for the bullet object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateBulletArchetype(Mesh* mesh);

	GameObject* CreateCrawlerEnemy(Mesh* mesh, SpriteSource* spriteSource, GameObject* tm);

	// Create the monkey game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject * CreateMonkey(Mesh * mesh, SpriteSource * spriteSource);

	// Create the circle game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateCircle(Mesh* mesh, SpriteSource* spriteSource);

	// Create the point game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreatePoint(Mesh* mesh, SpriteSource* spriteSource);

	// Create the rectangle game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateRectangle(Mesh* mesh);
	
	// Create a tilemap object
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	//   map = The map to use for tilemap related components.
	// Returns:
	//   A pointer to the newly constructed game object.
	GameObject* CreateTilemapObject(Mesh* mesh, SpriteSource* spriteSource, Tilemap* map);

	// Create a game object archetype that uses the Asteroid texture.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateAsteroidArchetype(Mesh* mesh, SpriteSource * spriteSource);

	// Create a game object archetype that uses the Asteroid texture.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateWallsArchetype(Mesh* mesh);

	// Create a game object archetype that uses the Asteroid texture.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateArenaArchetype();

	// Create door object
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game object.
	GameObject* CreateDoorObject(Mesh * mesh, SpriteSource * spriteSource);
}

//----------------------------------------------------------------------------
