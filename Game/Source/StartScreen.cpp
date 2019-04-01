//------------------------------------------------------------------------------
//
// File Name:	StartScreen.cpp
// Author(s):	Nicole Smith
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//-----------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Input.h>
#include "StartScreen.h"
#include <Level.h>
#include <SoundManager.h>
#include "MeshHelper.h"
#include <GameObjectManager.h>
#include <SpriteSource.h>
#include <Texture.h>
#include <SoundManager.h>
#include <Engine.h>
#include "MainLevel.h"
#include <Space.h>
#include "Archetypes.h"
#include "Sprite.h"
#include "Mesh.h"
#include <Transform.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{

	// Creates an instance of Level 2.
	StartScreen::StartScreen()
		:  Level("StartScreen"), bgMesh(nullptr), bgSpriteSource(nullptr)
	{
	}

	// Load the resources associated with Level 2.
	void StartScreen::Load()
	{
		//load sounds
		soundManager = Engine::GetInstance().GetModule<SoundManager>();

		bgMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5));

		bgTexture = Texture::CreateTextureFromFile("StartBG_Text.png");
		bgSpriteSource = new SpriteSource(bgTexture, "BG", 1, 1);
	}

	// Initialize the memory associated with Level 2.
	void StartScreen::Initialize()
	{
		Transform* transform = new Transform(Vector2D(0.0f, 0.0f), Vector2D(1000.0f, 1000.0f));
		Sprite* sprite = new Sprite();
		sprite->SetMesh(bgMesh);
		sprite->SetSpriteSource(bgSpriteSource);

		GameObject* bg = new GameObject("Bg");

		bg->AddComponent(sprite);
		bg->AddComponent(transform);


		GetSpace()->GetObjectManager().AddObject(*bg);
	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void StartScreen::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		//press enter/start
		if (Input::GetInstance().CheckTriggered(VK_RETURN))
		{
			GetSpace()->SetLevel(new MainLevel);
		}
	}

	// Unload the resources associated with Level 2.
	void StartScreen::Unload()
	{
		//unload sounds
		soundManager->Shutdown();

		delete bgMesh;
		delete bgSpriteSource;
		delete bgTexture;
	}
}

//----------------------------------------------------------------------------
