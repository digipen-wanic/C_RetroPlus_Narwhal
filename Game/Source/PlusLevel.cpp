//==================================================================-
/*
/file   PlusLevel.cpp
/author Jakob McFarland
/date   3/27/19
*/
//==================================================================-

#include "stdafx.h"
#include "Level2.h"
#include "PlusLevel.h"
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
	PlusLevel::PlusLevel()
		: Level("PlusLevel"), columnsMap(3), rowsMap(4), isDone(false), isPlaying(false)
	{
	}

	// Load the resources associated with Level 2.
	void PlusLevel::Load()
	{
		std::cout << "PlusLevel::Load" << std::endl;

		//graphics
		samusIdleMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5));

		//samusRunMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		//samusJumpRollMesh = CreateQuadMesh(Vector2D(0.33f, 0.5f), Vector2D(0.5, 0.5));
		crawlerMesh = CreateQuadMesh(Vector2D(1.0f, 0.5f), Vector2D(0.5, 0.5));
		batMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		doorMesh = CreateQuadMesh(Vector2D(1.0f, 0.5f), Vector2D(0.5, 0.5));
		Mesh* healthMesh = CreateQuadMesh(Vector2D(1.0f, 0.5f), Vector2D(0.5, 0.5));

		//samusRunMesh = CreateQuadMesh(Vector2D(0.5f, 0.5f), Vector2D(0.5, 0.5));
		//samusJumpRollMesh = CreateQuadMesh(Vector2D(0.33f, 0.5f), Vector2D(0.5, 0.5));
		samusStandingMesh = CreateQuadMesh(Vector2D(1.0f / 12.0f, 1.0f / 7.0f), Vector2D(0.5, 0.5));

		ResourceManager& resourceManager = GetSpace()->GetResourceManager();
		samusStanding = resourceManager.GetSpriteSource("SamusStandingPlus");
		resourceManager.GetSpriteSource("SamusIdlePlus");
		resourceManager.GetSpriteSource("SamusIdleUpPlus");
		resourceManager.GetSpriteSource("SamusRunPlus");
		resourceManager.GetSpriteSource("SamusRunUpPlus");
		resourceManager.GetSpriteSource("SamusRunShootPlus");
		resourceManager.GetSpriteSource("SamusJumpPlus");
		resourceManager.GetSpriteSource("SamusJumpRollPlus");
		resourceManager.GetSpriteSource("SamusRollPlus");

		samusBullet = resourceManager.GetSpriteSource("SamusBulletPlus");
		crawlerSpriteSource = resourceManager.GetSpriteSource("Crawler");
		batSpriteSource = resourceManager.GetSpriteSource("Scree");
		SpriteSource* healthSpriteSource = resourceManager.GetSpriteSource("Health");

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
		resourceManager.AddMesh("healthMesh", healthMesh);

		resourceManager.AddMesh("SamusBullet", CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5, 0.5)));

		//bullet archetype
		GetSpace()->GetObjectManager().AddArchetype( *Archetypes::CreateSamusBulletArchetype(samusIdleMesh, samusBullet) );

		GetSpace()->GetObjectManager().AddArchetype( *Archetypes::CreateHealthPickup(healthMesh, healthSpriteSource));

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

		textureMap = Texture::CreateTextureFromFile("TilemapPlus.png");
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
		soundManager->AddEffect("PlayerFirePlusUp.wav");
		soundManager->AddEffect("PlayerHitFX.wav");
		soundManager->AddEffect("PlayerJump.wav");
		soundManager->AddEffect("PlayerRunPlusUp.wav");

		soundManager->SetEffectsVolume(0.3f);

		//misc
		Graphics::GetInstance().SetDepthEnabled(true);
		Graphics::GetInstance().GetCurrentCamera().SetFOV(76.0f);
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(Vector2D(25.0f * 100.0f, 7.0f * -100.0f));

	}

	// Initialize the memory associated with Level 2.
	void PlusLevel::Initialize()
	{
		std::cout << "Level2::Initialize" << std::endl;

		GameObjectManager& gameObjectManager = GetSpace()->GetObjectManager();

		GameObject* tileMapObject = Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap);
		gameObjectManager.AddObject( *tileMapObject );

		GameObject* samus = Archetypes::CreateSamus(samusStandingMesh, samusStanding);
		gameObjectManager.AddObject(*samus);

		GameObject* healthDisplay = Archetypes::CreateHealthDisplay();
		healthDisplay->SetParent( samus );
		gameObjectManager.AddObject(*healthDisplay);

		GameObject* crawler = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject,0);
		GetSpace()->GetObjectManager().AddObject( *crawler );
		crawler->GetComponent<Transform>()->SetTranslation(Vector2D(2850,75));

		GameObject* crawler2 = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject,0);
		GetSpace()->GetObjectManager().AddObject(*crawler2);
		crawler2->GetComponent<Transform>()->SetTranslation(Vector2D(2250, 75));

		GameObject* crawler3 = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject,1);
		GetSpace()->GetObjectManager().AddObject(*crawler3);
		crawler3->GetComponent<Transform>()->SetTranslation(Vector2D(6300, 50));

		GameObject* crawler4 = Archetypes::CreateCrawler(crawlerMesh, crawlerSpriteSource, tileMapObject, 1);
		GetSpace()->GetObjectManager().AddObject(*crawler4);
		crawler4->GetComponent<Transform>()->SetTranslation(Vector2D(6250, -550));

		GameObject* door = Archetypes::CreateDoorObject(doorMesh, crawlerSpriteSource);
		GetSpace()->GetObjectManager().AddObject(*door);
		door->GetComponent<Transform>()->SetTranslation(Vector2D(6400, -300));

		GameObject* bat1 = Archetypes::CreateBat(batMesh, batSpriteSource, samus);
		gameObjectManager.AddObject(*bat1);
		bat1->GetComponent<Transform>()->SetTranslation(Vector2D(4350, 75));

		GameObject* bat2 = Archetypes::CreateBat(batMesh, batSpriteSource, samus);
		gameObjectManager.AddObject(*bat2);
		bat2->GetComponent<Transform>()->SetTranslation(Vector2D(5350, 75));

		GameObject* bat3 = Archetypes::CreateBat(batMesh, batSpriteSource, samus);
		gameObjectManager.AddObject(*bat3);
		bat3->GetComponent<Transform>()->SetTranslation(Vector2D(5550, 75));

		//GameObjectFactory::GetInstance().CreateObject("Monkey", meshMonkey, spriteSourceMonkey);
		samus->GetComponent<Behaviors::CameraFollow>()->SetTileMap(dataMap);

		//play music
		//musicChannel = soundManager->PlaySound("");
		effectChannel = soundManager->PlaySound("StartMusic4.wav");
		effectChannel->setVolume(0.3f);
		

	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void PlusLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (!isPlaying)
		{
			effectChannel->isPlaying(&isDone);
			if (isDone != true)
			{
				musicChannel = soundManager->PlaySound("LevelMusicPlusUpMP3");
				musicChannel->setVolume(0.8f);
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

	void PlusLevel::Shutdown()
	{
		musicChannel->stop();
		isPlaying = false;
	}

	// Unload the resources associated with Level 2.
	void PlusLevel::Unload()
	{
		std::cout << "PlusLevel::Unload" << std::endl;

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