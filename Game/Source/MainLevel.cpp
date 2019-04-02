//==================================================================-
/*
/file   MainLevel.cpp
/author Nicole Smith
/date   3/27/19
*/
//==================================================================-

#include "stdafx.h"
#include "Level2.h"
#include "MainLevel.h"
#include "GameObjectFactory.h"
#include "Level1.h"
#include "Level3.h"
#include "Omega.h"

#include "Archetypes.h"
#include "Space.h"
#include "Tilemap.h"
#include "Physics.h"
#include "Mesh.h"
#include "Animation.h"
#include "Sprite.h"
#include "Transform.h"
#include "Input.h"
#include "SpriteSource.h"
#include "MeshHelper.h"
#include "Texture.h"
#include <Engine.h>
#include <Graphics.h>
#include "CameraFollow.h"
#include <Parser.h>

namespace Levels
{
	//==================================================================-
	// Public Methods:
	//==================================================================-

	// Creates an instance of Level 2.
	MainLevel::MainLevel()
		: Level("MainLevel"), columnsMap(3), rowsMap(4)
	{
	}

	// Load the resources associated with Level 2.
	void MainLevel::Load()
	{
		std::cout << "MainLevel::Load" << std::endl;

		//textureMonkey = Texture::CreateTextureFromFile("Monkey.png");
		//spriteSourceMonkey = new SpriteSource(textureMonkey, "Monkey", columnsMonkey, rowsMonkey);

		//graphics
		samusIdleMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5));
		samusRunMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		samusJumpRollMesh = CreateQuadMesh(Vector2D(0.33f, 0.5f), Vector2D(0.5, 0.5));
		crawlerMesh = CreateQuadMesh(Vector2D(1.0f, 0.5f), Vector2D(0.5, 0.5));

		ResourceManager& resourceManager = GetSpace()->GetResourceManager();
		samusStanding = resourceManager.GetSpriteSource("SamusStanding");
		resourceManager.GetSpriteSource("SamusIdle");
		resourceManager.GetSpriteSource("SamusIdleUp");
		resourceManager.GetSpriteSource("SamusRun");
		resourceManager.GetSpriteSource("SamusRunUp");
		resourceManager.GetSpriteSource("SamusRunShoot");
		resourceManager.GetSpriteSource("SamusJump");
		resourceManager.GetSpriteSource("SamusJumpRoll");
		resourceManager.GetSpriteSource("SamusRoll");

		samusBullet = resourceManager.GetSpriteSource("SamusBullet");
		crawlerSpriteSource = resourceManager.GetSpriteSource("Crawler");

		resourceManager.AddMesh("SamusStanding", samusIdleMesh);
		resourceManager.AddMesh("SamusIdle", samusIdleMesh);
		resourceManager.AddMesh("SamusIdleUp", samusIdleMesh);
		resourceManager.AddMesh("SamusRun", samusRunMesh);
		resourceManager.AddMesh("SamusRunUp", samusRunMesh);
		resourceManager.AddMesh("SamusRunShoot", samusRunMesh);
		resourceManager.AddMesh("SamusJump", samusRunMesh);
		resourceManager.AddMesh("SamusJumpRoll", samusJumpRollMesh);
		resourceManager.AddMesh("SamusRoll", samusJumpRollMesh);

		resourceManager.AddMesh("SamusBullet", samusIdleMesh);
		resourceManager.AddMesh("crawlerMesh", crawlerMesh);

		//bullet archetype
		GetSpace()->GetObjectManager().AddArchetype( *Archetypes::CreateSamusBulletArchetype(samusIdleMesh, samusBullet) );

		//tilemap
		Tilemap* tilemapBuffer = new Tilemap();
		Parser parser("Assets/Levels/MainLevel.txt", std::fstream::in);
		tilemapBuffer->Deserialize(parser);

		if (tilemapBuffer != nullptr)
		{
			dataMap = tilemapBuffer;
		}
		else
		{
			std::cout << "Error loading map!" << std::endl;
		}

		textureMap = Texture::CreateTextureFromFile("TilemapMetroid.png");
		spriteSourceMap = new SpriteSource(textureMap, "Map", columnsMap, rowsMap);

		meshMap = CreateQuadMesh(Vector2D(1.0f / columnsMap, 1.0f / rowsMap), Vector2D(0.5, 0.5));
	
		//load sounds
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		//soundManager->SetEffectsVolume(0.01f);

		//soundManager->AddMusic("Asteroid_Field.mp3");
		soundManager->AddEffect("EnemyDeathFX.wav");
		soundManager->AddEffect("EnemyHitFX.wav");
		soundManager->AddEffect("EnergyPickUpFX.wav");
		soundManager->AddEffect("LowHealthFX.wav");
		soundManager->AddEffect("PauseFX2.wav");
		soundManager->AddEffect("PlayerDeathFX1.wav");
		soundManager->AddEffect("PlayerFire.wav");
		soundManager->AddEffect("PlayerHitFX.wav");
		soundManager->AddEffect("PlayerJump.wav");
		soundManager->AddEffect("PlayerRun2FX.wav");

		//misc
		Graphics::GetInstance().SetDepthEnabled(true);
		Graphics::GetInstance().GetCurrentCamera().SetFOV(75.0f);
	}

	// Initialize the memory associated with Level 2.
	void MainLevel::Initialize()
	{
		std::cout << "Level2::Initialize" << std::endl;

		GetSpace()->GetObjectManager().AddObject(*Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap));

		GameObject* samus = Archetypes::CreateSamus(samusIdleMesh, samusStanding);
		GetSpace()->GetObjectManager().AddObject(*samus);

		GameObject* crawler = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource);
		GetSpace()->GetObjectManager().AddObject( *crawler );

		//GameObjectFactory::GetInstance().CreateObject("Monkey", meshMonkey, spriteSourceMonkey);
		samus->GetComponent<Behaviors::CameraFollow>()->SetTileMap(dataMap);

		//play music
		//musicChannel = soundManager->PlaySound("");
	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		
		//press enter/start
		if (Input::GetInstance().CheckTriggered(VK_RETURN))
		{
			//pause
			soundManager->PlaySound("PauseFX2.wav")->setVolume(0.2f);
		}
		else if (Input::GetInstance().CheckTriggered('1'))
		{
			GetSpace()->RestartLevel();
		}

	}

	// Unload the resources associated with Level 2.
	void MainLevel::Unload()
	{
		std::cout << "MainLevel::Unload" << std::endl;

		//unload sounds
		soundManager->Shutdown();

		//delete meshMonkey;
		//delete textureMonkey;
		//delete spriteSourceMonkey;

		delete dataMap;
		delete meshMap;
		delete textureMap;
		delete spriteSourceMap;
	}
}