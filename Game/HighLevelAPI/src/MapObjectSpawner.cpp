//------------------------------------------------------------------------------
//
// File Name:	MapObjectSpawner.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework Map Editor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MapObjectSpawner.h"

// Systems
#include <Graphics.h>	// ScreenToWorldPosition
#include <Parser.h>		// Read/Write variables
#include <GameObject.h>	// GetSpace
#include <Space.h>		// GetObjectManager

// Components
#include <Transform.h>	// GetTranslation

// Resources
#include <Tilemap.h>	// GetObjects

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

MapObjectSpawner::MapObjectSpawner()
	: Component("MapObjectSpawner"), map(nullptr)
{
}

Component * MapObjectSpawner::Clone() const
{
	return new MapObjectSpawner(*this);
}

void MapObjectSpawner::Initialize()
{
	// Get components
	transform = GetOwner()->GetComponent<Transform>();

	// Set new map
	//const MapLoad& loadEvent = static_cast<const MapLoad&>(event);
	//map = loadEvent.map;
	GameObjectManager& oManager = GetOwner()->GetSpace()->GetObjectManager();

	// Place new objects
	const std::vector<ObjectInMap>& newObjects = map->GetObjects();
	for (auto it = newObjects.begin(); it != newObjects.end(); ++it)
	{
		// Try to get existing archetype
		const std::string& archetypeName = (*it).name;
		GameObject* object = new GameObject(*oManager.GetArchetypeByName(archetypeName));
		(*it).object = object;
		PlaceObject(object, (*it).x, (*it).y);
	}
}

// Sets the tilemap data that will be used by this component.
// Params:
//   map = A pointer to the tilemap resource.
void MapObjectSpawner::SetTilemap(const Tilemap* _map)
{
	map = _map;
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file we want to read from.
void MapObjectSpawner::Deserialize(Parser & parser)
{
	std::string tilemapName;
	parser.ReadVariable("tilemap", tilemapName);
	SetTilemap(GetOwner()->GetSpace()->GetResourceManager().GetTilemap(tilemapName));
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file we want to write to.
void MapObjectSpawner::Serialize(Parser & parser) const
{
	parser.WriteVariable("tilemap", map->GetName());
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void MapObjectSpawner::PlaceObject(GameObject * object, int x, int y)
{
	Transform* objectTransform = object->GetComponent<Transform>();

	// Set translation to specific cell on map
	objectTransform->SetTranslation(transform->GetMatrix() *
		Vector2D(static_cast<float>(x), static_cast<float>(-y)));

	// Add to object manager
	GetOwner()->GetSpace()->GetObjectManager().AddObject(*object);
}


//------------------------------------------------------------------------------
