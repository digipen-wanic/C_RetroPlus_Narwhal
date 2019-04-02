//------------------------------------------------------------------------------
//
// File Name:	StartScreen.h
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

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class StartScreen : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 2.
		StartScreen();

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

		//Text image
		Mesh* bgMesh;
		SpriteSource* bgSpriteSource;
		Texture* bgTexture;


		//Sound manager
		SoundManager* soundManager;
		FMOD::Channel* musicChannel;
	};
}

//----------------------------------------------------------------------------
