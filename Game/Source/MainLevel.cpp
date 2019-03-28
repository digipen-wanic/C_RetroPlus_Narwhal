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

namespace Levels
{
	//==================================================================-
	// Public Methods:
	//==================================================================-

	// Creates an instance of Level 2.
	MainLevel::MainLevel()
		: Level("MainLevel"), columnsMonkey(3), rowsMonkey(5), columnsMap(4), rowsMap(3)
	{
	}

	// Load the resources associated with Level 2.
	void MainLevel::Load()
	{
		std::cout << "MainLevel::Load" << std::endl;

		textureMonkey = Texture::CreateTextureFromFile("Monkey.png");
		spriteSourceMonkey = new SpriteSource(columnsMonkey, rowsMonkey, textureMonkey);

		meshMonkey = CreateQuadMesh(Vector2D(0.33333f, 0.2f), Vector2D(0.5, 0.5));

		Tilemap* tilemapBuffer = Tilemap::CreateTilemapFromFile("Assets/Levels/Platformer.txt");

		if (tilemapBuffer != nullptr)
		{
			dataMap = tilemapBuffer;
		}
		else
		{
			std::cout << "Error loading map!" << std::endl;
		}

		textureMap = Texture::CreateTextureFromFile("Tilemap.png");
		spriteSourceMap = new SpriteSource(columnsMap, rowsMap, textureMap);

		meshMap = CreateQuadMesh(Vector2D(0.25f, 0.33333f), Vector2D(0.5, 0.5));
	}

	// Initialize the memory associated with Level 2.
	void MainLevel::Initialize()
	{
		std::cout << "Level2::Initialize" << std::endl;

		GetSpace()->GetObjectManager().AddObject(*Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap));
		GameObject* monkey = GameObjectFactory::GetInstance().CreateObject("Monkey", meshMonkey, spriteSourceMonkey);
		monkey->GetComponent<Behaviors::CameraFollow>()->SetTileMap(dataMap);
		GetSpace()->GetObjectManager().AddObject(*monkey);
	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (Input::GetInstance().CheckTriggered('1'))
		{
			GetSpace()->SetLevel(new Level1());
		}
		else if (Input::GetInstance().CheckTriggered('2'))
		{
			GetSpace()->RestartLevel();
		}
		else if (Input::GetInstance().CheckTriggered('3'))
		{
			GetSpace()->SetLevel(new Level3());
		}
		else if (Input::GetInstance().CheckTriggered('O'))
		{
			GetSpace()->SetLevel(new Omega());
		}
	}

	// Unload the resources associated with Level 2.
	void MainLevel::Unload()
	{
		std::cout << "Level2::Unload" << std::endl;

		delete meshMonkey;
		delete textureMonkey;
		delete spriteSourceMonkey;

		delete dataMap;
		delete meshMap;
		delete textureMap;
		delete spriteSourceMap;
	}
}