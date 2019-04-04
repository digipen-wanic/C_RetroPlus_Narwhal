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
#include "PlayerController.h"

namespace Levels
{
	//==================================================================-
	// Public Methods:
	//==================================================================-

	// Creates an instance of Level 2.
	MainLevel::MainLevel()
		: Level("MainLevel"), columnsMap(3), rowsMap(4), isDone(false), isPlaying(false)
	{
	}

	// Load the resources associated with Level 2.
	void MainLevel::Load()
	{
		std::cout << "MainLevel::Load" << std::endl;

		//graphics
		samusIdleMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5));

		//samusRunMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		//samusJumpRollMesh = CreateQuadMesh(Vector2D(0.33f, 0.5f), Vector2D(0.5, 0.5));
		crawlerMesh = CreateQuadMesh(Vector2D(1.0f, 0.5f), Vector2D(0.5, 0.5));
		batMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		doorMesh = CreateQuadMesh(Vector2D(1.0f, 0.5f), Vector2D(0.5, 0.5));

		//samusRunMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		//samusJumpRollMesh = CreateQuadMesh(Vector2D(0.33f, 0.5f), Vector2D(0.5, 0.5));
		samusStandingMesh = CreateQuadMesh(Vector2D(1.0f / 12.0f, 1.0f / 7.0f), Vector2D(0.5, 0.5));

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
		batSpriteSource = resourceManager.GetSpriteSource("Scree");

		resourceManager.AddMesh("SamusStanding", samusStandingMesh);
		resourceManager.AddMesh("SamusIdle", samusIdleMesh);
		resourceManager.AddMesh("SamusIdleUp", CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5)));
		resourceManager.AddMesh("SamusRun", CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5)));
		resourceManager.AddMesh("SamusRunUp", CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5)));
		resourceManager.AddMesh("SamusRunShoot", CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5)));
		resourceManager.AddMesh("SamusJump", CreateQuadMesh(Vector2D( 1.0f , 1.0f), Vector2D(0.5, 0.5)));
		resourceManager.AddMesh("SamusJumpRoll", CreateQuadMesh(Vector2D(0.33f, 0.5f), Vector2D(0.5, 0.5)));
		resourceManager.AddMesh("SamusRoll", CreateQuadMesh(Vector2D(1.0f / 2.0f, 1.0f / 2.0f), Vector2D(0.5, 0.5)));

		resourceManager.AddMesh("crawlerMesh", crawlerMesh);
		resourceManager.AddMesh("batMesh", batMesh);

		resourceManager.AddMesh("SamusBullet", CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5)));

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

		soundManager->AddMusic("LevelMusicMP3.mp3");
		soundManager->AddEffect("StartMusic4.wav");
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

		soundManager->SetEffectsVolume(0.2f);

		//misc
		Graphics::GetInstance().SetDepthEnabled(true);
		Graphics::GetInstance().GetCurrentCamera().SetFOV(76.0f);
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(Vector2D(25.0f * 100.0f, 7.0f * -100.0f));

	}

	// Initialize the memory associated with Level 2.
	void MainLevel::Initialize()
	{
		std::cout << "Level2::Initialize" << std::endl;

		GameObjectManager& gameObjectManager = GetSpace()->GetObjectManager();

		GameObject* tileMapObject = Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap);

		gameObjectManager.AddObject( *tileMapObject );

		GameObject* samus = Archetypes::CreateSamus(samusStandingMesh, samusStanding);

		gameObjectManager.AddObject(*samus);

		//GameObject* crawler = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject,0);
		//GetSpace()->GetObjectManager().AddObject( *crawler );
		//crawler->GetComponent<Transform>()->SetTranslation(Vector2D(2850,75));

		//GameObject* crawler2 = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject,0);
		//GetSpace()->GetObjectManager().AddObject(*crawler2);
		//crawler2->GetComponent<Transform>()->SetTranslation(Vector2D(2250, 75));

		//GameObject* crawler3 = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject,1);
		//GetSpace()->GetObjectManager().AddObject(*crawler3);
		//crawler3->GetComponent<Transform>()->SetTranslation(Vector2D(6300, 50));

		GameObject* crawler4 = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject, 2);
		GetSpace()->GetObjectManager().AddObject(*crawler4);
		crawler4->GetComponent<Transform>()->SetTranslation(Vector2D(6250, -550));

		GameObject* door = Archetypes::CreateDoorObject(doorMesh, crawlerSpriteSource);
		GetSpace()->GetObjectManager().AddObject(*door);
		door->GetComponent<Transform>()->SetTranslation(Vector2D(6400, -300));

		GameObject* bat1 = Archetypes::CreateBat(batMesh, batSpriteSource, samus);
		gameObjectManager.AddObject(*bat1);
		bat1->GetComponent<Transform>()->SetTranslation(Vector2D(4350, 75));

		//GameObjectFactory::GetInstance().CreateObject("Monkey", meshMonkey, spriteSourceMonkey);
		samus->GetComponent<Behaviors::CameraFollow>()->SetTileMap(dataMap);

		//play music
		//musicChannel = soundManager->PlaySound("");
		effectChannel = soundManager->PlaySound("StartMusic4.wav");
		effectChannel->setVolume(0.2f);
		

	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (!isPlaying)
		{
			effectChannel->isPlaying(&isDone);
			if (isDone != true)
			{
				musicChannel = soundManager->PlaySound("LevelMusicMP3");
				musicChannel->setVolume(0.5f);
				isPlaying = true;
				std::cout << "play music" << std::endl;
			}
		}

		//press enter/start
		if (Input::GetInstance().CheckTriggered(VK_RETURN))
		{
			//pause
			soundManager->PlaySound("PauseFX2.wav")->setVolume(0.2f);

			if (!GetSpace()->IsPaused())
			{
				GetSpace()->SetPaused(true);
				musicChannel->setPaused(true);
			}
			else {
				GetSpace()->SetPaused(false);
				musicChannel->setPaused(false);
			}
		}
		else if (Input::GetInstance().CheckTriggered('1'))
		{
			
			GetSpace()->RestartLevel();
		}

	}

	void MainLevel::Shutdown()
	{
		musicChannel->stop();
		isPlaying = false;
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
		delete doorMesh;
	}
}