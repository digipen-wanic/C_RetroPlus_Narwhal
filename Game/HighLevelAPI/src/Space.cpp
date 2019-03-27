//==================================================================-
/*
/file   Space.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class Space, as specified in specification
file Space.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Space.h"

#include "level.h"

//==================================================================-
// Public Methods:
//==================================================================-

// Constructor(s)
Space::Space(const std::string& name)
	: BetaObject(name), paused(false), currentLevel(nullptr),
	nextLevel(nullptr), objectManager(this)
{
}

// Destructor
Space::~Space()
{
	Shutdown();
}

// Updates the state manager and object manager.
// Params:
//   dt = The change in time since the last call to this function.
void Space::Update(float dt)
{
	//check for level change
	if (nextLevel != nullptr)
	{
		ChangeLevel();
	}

	//update the current level
	if (currentLevel != nullptr)
	{
		currentLevel->Update(dt);
	}

	objectManager.Update(dt);
}

// Shuts down the object manager
void Space::Shutdown()
{
	if (currentLevel != nullptr)
	{
		currentLevel->Shutdown();
		currentLevel->Unload();

		delete currentLevel;
		currentLevel = nullptr;
	}

	objectManager.Shutdown();
	objectManager.Unload();
}

// Returns a boolean indicating whether objects in this space are paused.
// Returns:
//   True if the space is paused, false otherwise.
bool Space::IsPaused() const
{
	return paused;
}

// Returns the name of the level currently running in this space.
const std::string& Space::GetLevelName() const
{
	if (currentLevel != nullptr)
	{
		return currentLevel->GetName();
	}

	//TEMP: has to return something
	return currentLevel->GetName();
}

// Pauses the space, preventing objects from being updated, but objects are still drawn.
// Params:
//   value = A true/false value that indicates whether the space should be paused.
void Space::SetPaused(bool value) 
{
	paused = value;
}

// Sets the level that the space is using after unloading the current level.
// Params:
//   level = The next level that the space will be using.
void Space::SetLevel(Level* level)
{
	nextLevel = level;
	nextLevel->SetParent(this);
}

// Restarts the current level (next level = current)
void Space::RestartLevel()
{
	nextLevel = currentLevel;
}

// Returns the object manager, which you can use to retrieve and add objects.
GameObjectManager& Space::GetObjectManager()
{
	return objectManager;
}

//==================================================================-
// Private Methods:
//==================================================================-

// Updates current/next level pointers and calls shutdown/unload/load/init
void Space::ChangeLevel()
{
	//if there is no level
	if (currentLevel == nullptr)
	{
		//set up a new one
		currentLevel = nextLevel;
		currentLevel->Load();
	}
	//if not
	else
	{
		//shutdown the current level down
		currentLevel->Shutdown();
		objectManager.Shutdown();

		//if we are switching levels
		if (currentLevel != nextLevel)
		{
			//unload current level, changes current level, and load new current level
			currentLevel->Unload();
			objectManager.Unload();
			delete currentLevel;
			currentLevel = nextLevel;
			currentLevel->Load();
		}
	}

	//and reinitilize the current level
	currentLevel->Initialize();

	//set nextLevel back to null
	nextLevel = nullptr;
}