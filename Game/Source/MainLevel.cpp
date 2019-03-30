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
#include "CameraFollow.h"
#include <Parser.h>

namespace Levels
{
	//==================================================================-
	// Public Methods:
	//==================================================================-

	// Creates an instance of Level 2.
	MainLevel::MainLevel()
		: Level("MainLevel"), columnsMonkey(3), rowsMonkey(5), columnsMap(3), rowsMap(4)
	{
	}

	// Load the resources associated with Level 2.
	void MainLevel::Load()
	{
		std::cout << "MainLevel::Load" << std::endl;

		textureMonkey = Texture::CreateTextureFromFile("Monkey.png");
		spriteSourceMonkey = new SpriteSource(textureMonkey, "Monkey", columnsMonkey, rowsMonkey);

		samusIdleMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5));
		samusRunMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));

		samusStanding = GetSpace()->GetResourceManager().GetSpriteSource("SamusStanding");
		samusIdle = GetSpace()->GetResourceManager().GetSpriteSource("SamusIdle");
		samusRun = GetSpace()->GetResourceManager().GetSpriteSource("SamusRun");
		samusRunShoot = GetSpace()->GetResourceManager().GetSpriteSource("SamusRunShoot");
		samusJump = GetSpace()->GetResourceManager().GetSpriteSource("SamusJump");
		samusJumpRoll = GetSpace()->GetResourceManager().GetSpriteSource("SamusJumpRoll");
		samusRoll = GetSpace()->GetResourceManager().GetSpriteSource("SamusRoll");

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

	}

	// Initialize the memory associated with Level 2.
	void MainLevel::Initialize()
	{
		std::cout << "Level2::Initialize" << std::endl;

		GetSpace()->GetObjectManager().AddObject(*Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap));

		GameObject* samus = Archetypes::CreateSamus(samusIdleMesh, samusRunMesh, samusStanding, samusIdle,
			samusRun, samusRunShoot, samusJump, samusJumpRoll, samusRoll);
			//GameObjectFactory::GetInstance().CreateObject("Monkey", meshMonkey, spriteSourceMonkey);
		samus->GetComponent<Behaviors::CameraFollow>()->SetTileMap(dataMap);
		GetSpace()->GetObjectManager().AddObject(*samus);

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

		delete meshMonkey;
		delete textureMonkey;
		delete spriteSourceMonkey;

		delete dataMap;
		delete meshMap;
		delete textureMap;
		delete spriteSourceMap;
	}
}