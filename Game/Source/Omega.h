//------------------------------------------------------------------------------
//
// File Name:	Omega.h
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

#include <Level.h>

#include "PlayerShip.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class SpriteSource;
class Texture;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class Omega : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create an instance of the Omega game state.
		Omega();

		// Load archetypes, meshes
		void Load();

		// Initialize the memory associated with the Omega game state.
		void Initialize();

		// Update the Omega game state.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

		// Unload textures, sprite sources, meshes
		void Unload();

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------
	
		// Create a single asteroid object from the archetype
		void SpawnAsteroid(void);

		// Create a group of asteroids
		void SpawnAsteroidWave(void);

		// Update the score string
		void UpdateScore();

		// Switch levels when keys are pressed
		void SwitchLevels();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Meshes
		Mesh* meshShip;
		Mesh* meshBullet;
		Mesh* meshAsteroid;

		Texture* textureAsteroid;
		SpriteSource* spriteSourceAsteroid;

		// Spawning
		const unsigned asteroidSpawnInitial;
		const unsigned asteroidSpawnMaximum;
		unsigned asteroidSpawnCount;
		unsigned asteroidWaveCount;

		// Player
		Behaviors::PlayerShip* playerShip;
		unsigned asteroidHighScore;
		
		// Window title stuff
		static const unsigned titleStringLength = 128;
		char windowTitle[titleStringLength];
	};
}

//----------------------------------------------------------------------------
