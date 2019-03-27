//==================================================================-
/*
/file   Omega.cpp
/author Jakob McFarland
/date   3/4/2019
/brief

This is the implementation file for all member functions
of the level Omega, as specified in specification
file Omega.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Archetypes.h"
#include "Omega.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "Asteroid.h"
#include "PlayerShip.h"

#include <System.h>
#include <Space.h>
#include <Transform.h>
#include <Input.h>
#include <GameObjectManager.h>
#include <GameObjectFactory.h>
#include <SpriteSource.h>
#include <Physics.h>
#include <MeshHelper.h>
#include <Mesh.h>
#include <GameObject.h>
#include <Graphics.h>

namespace Levels
{
	//==================================================================-
	// Public Functions:
	//==================================================================-

	// Creates an instance of the Omega level.
	Omega::Omega()
		: Level("Omega"), asteroidSpawnInitial(8), asteroidSpawnMaximum(20), 
		asteroidSpawnCount(0), asteroidWaveCount(0), asteroidHighScore(0)
	{

	}

	// Load the resources associated with the Omega level.
	void Omega::Load()
	{
		Graphics::GetInstance().SetBackgroundColor(Colors::Black);

		meshShip = CreateTriangleMesh(Colors::Green, Colors::Green, Colors::Green);
		meshBullet = CreateTriangleMesh(Colors::Red, Colors::Red, Colors::Red);
		meshAsteroid = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		
		textureAsteroid = Texture::CreateTextureFromFile("Asteroid.png");
		spriteSourceAsteroid = new SpriteSource(1, 1, textureAsteroid);

		GetSpace()->GetObjectManager().AddArchetype( *GameObjectFactory::GetInstance().CreateObject("Bullet", meshBullet) );

		GameObject* asteroid = Archetypes::CreateAsteroidArchetype(meshAsteroid, spriteSourceAsteroid);	
		GetSpace()->GetObjectManager().AddArchetype(*asteroid);
	}

	// Initialize the memory associated with the Omega level.
	void Omega::Initialize()
	{
		asteroidWaveCount = 0;
		asteroidSpawnCount = 0;

		GameObject* player = Archetypes::CreateShip( meshShip );
		playerShip = player->GetComponent<Behaviors::PlayerShip>();
		player->GetComponent<Transform>()->SetTranslation( Vector2D(300.0f, 200.0f));
		player->GetComponent<Physics>()->SetOldTranslation(Vector2D(300.0f, 200.0f));
		GetSpace()->GetObjectManager().AddObject(*player);

		GetSpace()->GetObjectManager().AddObject(*Archetypes::CreateWallsArchetype(meshAsteroid));
		GetSpace()->GetObjectManager().AddObject(*Archetypes::CreateArenaArchetype());
	}

	// Update the Omega level.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Omega::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		UpdateScore();
		SwitchLevels();

		if ( GetSpace()->GetObjectManager().GetObjectCount("Asteroid") == 0)
		{
			SpawnAsteroidWave();
		}
	}

	void Omega::Unload()
	{
		//if(asteroidHighScore < playerShip->GetScore()) asteroidHighScore = playerShip->GetScore();
	
		delete meshShip;
		delete meshBullet;
		delete meshAsteroid;

		delete textureAsteroid;
		delete spriteSourceAsteroid;
	}

	//==================================================================-
	// Private Functions:
	//==================================================================-

	// Create a single asteroid object from the archetype
	void Omega::SpawnAsteroid(void) 
	{
		GameObject* asteroid = new GameObject(*GetSpace()->GetObjectManager().GetArchetypeByName("Asteroid"));
		GetSpace()->GetObjectManager().AddObject(*asteroid);
	}

	// Create a group of asteroids
	void Omega::SpawnAsteroidWave(void)
	{
		++asteroidWaveCount;

		for (unsigned i = 0; i < asteroidSpawnCount; ++i)
		{
			SpawnAsteroid();
		}

		if (asteroidSpawnCount < asteroidSpawnMaximum)
		{
			++asteroidSpawnCount;
		}
	}

	// Update the score string
	void Omega::UpdateScore()
	{
		sprintf_s(windowTitle, titleStringLength, "Omega: Wave = %d, Score = %d, High Score = %d", asteroidWaveCount, playerShip->GetScore(), asteroidHighScore);
		System::GetInstance().SetWindowTitle(windowTitle);
	}

	// Switch levels when keys are pressed
	void Omega::SwitchLevels()
	{
		Input& input = Input::GetInstance();

		//change level logic
		if (input.CheckTriggered('1'))
		{
			GetSpace()->SetLevel(new Level1);
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel(new Level2); 
		}
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->SetLevel(new Level3);
		}
		else if (Input::GetInstance().CheckTriggered('O'))
		{
			GetSpace()->RestartLevel();
		}
	}
}