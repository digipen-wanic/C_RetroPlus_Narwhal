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
#include <Animation.h>
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

		bgMesh = CreateQuadMesh(Vector2D(1.0f / 10.0f, 1.0f / 12.0f), Vector2D(0.5, 0.5));

		ResourceManager& resourceManager = GetSpace()->GetResourceManager();
		bgSpriteSource = resourceManager.GetSpriteSource("TitleScreen");

	}

	// Initialize the memory associated with Level 2.
	void StartScreen::Initialize()
	{
		Transform* transform = new Transform(Vector2D(0.0f, 0.0f), Vector2D(1050.0f, 1000.0f));
		Sprite* sprite = new Sprite();
		sprite->SetMesh(bgMesh);
		sprite->SetSpriteSource(bgSpriteSource);
		Animation* animation = new Animation();

		GameObject* bg = new GameObject("Bg");

		bg->AddComponent(sprite);
		bg->AddComponent(transform);
		bg->AddComponent(animation);

		GetSpace()->GetObjectManager().AddObject(*bg);

		animation->Play(0.1f, true);
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
	}
}

//----------------------------------------------------------------------------
