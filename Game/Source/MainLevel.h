//------------------------------------------------------------------------------
//
// File Name:	MainLevel.h
// Author(s):	Nicole Smith
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
#include <SoundManager.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class SpriteSource;
class Mesh;
class GameObject;
class Tilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class MainLevel : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 2.
		MainLevel();

		// Load the resources associated with Level 2.
		void Load() override;

		// Initialize the memory associated with Level 2.
		void Initialize() override;

		// Update Level 2.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Level 2.
		void Unload() override;

	private:

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Monkey
		//Mesh* meshMonkey;
		//Texture* textureMonkey;
		//SpriteSource* spriteSourceMonkey;
		//unsigned columnsMonkey;
		//unsigned rowsMonkey;

		//Samus
		Mesh* samusIdleMesh;
		SpriteSource* samusStanding;
		SpriteSource* samusBullet;
		SpriteSource* crawlerSpriteSource;
		SpriteSource* batSpriteSource;
		Mesh* samusStandingMesh;
		Mesh* crawlerMesh;
		Mesh* batMesh;

		// Tilemap
		Tilemap* dataMap;
		Texture* textureMap;
		SpriteSource* spriteSourceMap;
		Mesh* meshMap;
		unsigned columnsMap;
		unsigned rowsMap;

		//Sound manager
		SoundManager* soundManager;
		FMOD::Channel* effectChannel;
		FMOD::Channel* musicChannel;
		bool isDone;
		bool isPlaying;
	};
}

//----------------------------------------------------------------------------
