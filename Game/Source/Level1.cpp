//==================================================================-
/*
/file   Level1.cpp
/author Jakob McFarland
/date   12/7/2018
/brief

This is the implementation file for all member functions
of the class Level1, as specified in specification
file Level1.h.

*/
//==================================================================-

#include "stdafx.h"
#include "level1.h"

#include "level2.h"
#include "level3.h"
#include "Omega.h"
#include "Matrix2DStudent.h"
#include <Parser.h>
#include <GameObjectFactory.h>

#include "Archetypes.h"
#include "Physics.h"
#include "MeshHelper.h"
#include "Transform.h"
#include "Color.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Space.h"
#include <Engine.h>
#include <Input.h>
#include <System.h>
#include "SoundManager.h"

namespace Levels
{
	//==================================================================-
	// Public Methods:
	//==================================================================-

		// Creates an instance of Level 1.
	Level1::Level1()
		: Level("Level1")
	{
	}

	// Load the resources associated with Level 1.
	void Level1::Load()
	{
		std::cout << "Level1::Load" << std::endl;

		meshShip = CreateTriangleMesh(Colors::Green, Colors::Green, Colors::Green);
		meshBullet = CreateTriangleMesh(Colors::Red, Colors::Red, Colors::Red);

		GetSpace()->GetObjectManager().AddArchetype(*Archetypes::CreateBulletArchetype(meshBullet));
		//GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Bullet", meshBullet));

		/*soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->AddMusic("Asteroid_Field.mp3");
		soundManager->AddEffect("teleport.wav");

		soundManager->AddBank("Master Bank.strings.bank");
		soundManager->AddBank("Master Bank.bank");*/
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		std::cout << "Level1::Initialize" << std::endl;

		GetSpace()->GetObjectManager().AddObject(*GameObjectFactory::GetInstance().CreateObject("PlayerShip", meshShip));
		//GetSpace()->GetObjectManager().AddArchetype(*Archetypes::CreateShip(meshShip));

		//musicChannel = soundManager->PlaySound("Asteroid Field");
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		//std::cout << "Level1::Update" << std::endl;

		sprintf_s(windowTitle, titleStringLength, "Bullets: %d", GetSpace()->GetObjectManager().GetObjectCount("Bullet"));
		System::GetInstance().SetWindowTitle(windowTitle);

		if (Input::GetInstance().CheckTriggered('1') )
		{
			GetSpace()->RestartLevel();
		}
		else if (Input::GetInstance().CheckTriggered('2'))
		{
			GetSpace()->SetLevel(new Level2());
		}
		else if (Input::GetInstance().CheckTriggered('3'))
		{
			GetSpace()->SetLevel(new Level3());
		}
		else if (Input::GetInstance().CheckTriggered('O'))
		{
			GetSpace()->SetLevel(new Omega());
		}

		if (Input::GetInstance().CheckTriggered('T'))
		{
			//soundManager->PlaySound("teleport.wav");
		}
	}

	// Destroy objects associated with level 1.
	/*void Level1::Shutdown()
	{
		musicChannel->stop();
	}*/

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
		std::cout << "Level1::Unload" << std::endl;

		delete meshShip;
		delete meshBullet;

		//soundManager->Shutdown();	
	}
}