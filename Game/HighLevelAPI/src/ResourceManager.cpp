//------------------------------------------------------------------------------
//
// File Name:	ResourceManager.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ResourceManager.h"

// Math
#include "Vector2D.h" // constructors

// Resources
#include "Mesh.h" // destructor
#include "Vertex.h" // constructors
#include "SpriteSource.h" // GetTexture
#include "Texture.h" // GetName
#include "Tilemap.h"

// Systems
#include "MeshFactory.h" // MeshCreateQuad
#include <Parser.h> // SpriteSource serialization
#include "Engine.h" // GetModule
#include "SoundManager.h" // AddEffect, AddBGM

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
ResourceManager::ResourceManager()
{
}

// Destructor
ResourceManager::~ResourceManager()
{
	Shutdown();
}

// Create an association between a mesh and a particular type of object
void ResourceManager::AddMesh(const std::string & objectName, Mesh * mesh)
{
	// If mesh with same name exists, delete it
	auto foundMesh = meshes.find(objectName);
	if (foundMesh != meshes.end())
	{
		delete foundMesh->second;
		meshes.erase(objectName);
	}

	meshes.emplace(objectName, mesh);
}

// Retrieve an existing mesh required by a game object.
Mesh* ResourceManager::GetMesh(const std::string& objectName, bool createIfNotFound)
{
	Mesh* mesh = nullptr;
	auto foundMesh = meshes.find(objectName);
		
	if (foundMesh != meshes.end())
	{
		mesh = foundMesh->second;
	}

	// Couldn't find existing mesh
	if (mesh == nullptr && createIfNotFound)
	{
		mesh = MeshFactory::GetInstance().CreateQuadMesh();
		meshes.emplace(objectName, mesh);
	}

	return mesh;
}

// Manually add a sprite source to the manager.
// Params:
//   spriteSource = The sprite source to add to the manager.
void ResourceManager::AddSpriteSource(SpriteSource * spriteSource)
{
	// If sprite source with same name exists, delete it
	auto foundSource = spriteSources.find(spriteSource->GetName());
	if (foundSource != spriteSources.end())
		delete foundSource->second;

	spriteSources.emplace(spriteSource->GetName(), spriteSource);
}

// Retrieve a sprite source with the given name, create it if not found.
// Params:
//	 name = Name of the sprite source.
//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
SpriteSource* ResourceManager::GetSpriteSource(const std::string& name, bool createIfNotFound)
{
	SpriteSource* spriteSource = nullptr;

	auto foundSource = spriteSources.find(name);
	if (foundSource != spriteSources.end())
	{
		spriteSource = foundSource->second;
	}

	// Couldn't find existing sprite source
	if (spriteSource == nullptr && createIfNotFound)
	{
		spriteSource = new SpriteSource();
		const std::string& enginePath = Engine::GetInstance().GetFilePath();
		std::string filename = enginePath + SpriteSource::spriteSourcePath + name + ".txt";
		Parser parser(filename, std::fstream::in);
		try
		{
			// Attempt to load sprite source
			spriteSource->Deserialize(parser);

			// Set the sprite source's texture
			spriteSource->SetTexture(GetTexture(spriteSource->GetTextureName()));

			// Add to map
			AddSpriteSource(spriteSource);
		}
		catch (const ParseException& e)
		{
			std::cout << "ERROR in ResourceManager: " << e.what() << std::endl;
			delete spriteSource;
			spriteSource = nullptr;
		}
	}

	return spriteSource;
}

// Saves a sprite source to a file so it can be loaded later.
// Params:
//   spriteSource = The sprite source being saved to a file. Filename is based on sprite source name.
void ResourceManager::SaveSpriteSourceToFile(const SpriteSource * spriteSource)
{
	// Create filename
	const std::string& enginePath = Engine::GetInstance().GetFilePath();
	std::string filename = enginePath + SpriteSource::spriteSourcePath + spriteSource->GetName() + ".txt";
	Parser parser(filename, std::fstream::out);

	// Attempt to save object
	spriteSource->Serialize(parser);
	std::cout << "Wrote sprite source " << spriteSource->GetName() << " to file " << filename << std::endl;
}

// Manually create an association between a map and a particular type of object
// Params:
//   name = The name to use for the map.
//   map = The map to add to the manager.
void ResourceManager::AddTilemap(const std::string & name, Tilemap * map)
{
	maps.emplace(name, map);
}

// Retrieves a texture with the given filename, loads it if not found.
// Params:
//   name = Filename of the texture with extension.
//   createIfNotFound = Whether to load the texture if no texture is found.
const Texture* ResourceManager::GetTexture(const std::string& filename, bool createIfNotFound)
{
	Texture* texture = nullptr;
	auto foundTexture = textures.find(filename);

	if (foundTexture != textures.end())
	{
		texture = foundTexture->second;
	}

	// Couldn't find existing mesh
	if (texture == nullptr && createIfNotFound)
	{
		texture = Texture::CreateTextureFromFile(filename);
		textures.emplace(filename, texture);
	}

	return texture;
}

// Retrieves a tilemap with the given filename, loads it if not found.
// Params:
//   name = Filename of the tilemap.
//   createIfNotFound = Whether to load the tilemap if it is not found.
Tilemap * ResourceManager::GetTilemap(const std::string & filename, bool createIfNotFound)
{
	Tilemap* map = nullptr;

	auto foundMap = maps.find(filename);
	if (foundMap != maps.end())
	{
		map = foundMap->second;
	}

	// Couldn't find existing map
	if (map == nullptr && createIfNotFound)
	{
		map = new Tilemap();
		const std::string& enginePath = Engine::GetInstance().GetFilePath();
		std::string path = enginePath + Tilemap::mapPath + filename + ".txt";
		Parser parser(path, std::fstream::in);
		try
		{
			// Attempt to load sprite source
			map->Deserialize(parser);

			// Add to map
			AddTilemap(map->GetName(), map);
		}
		catch (const ParseException& e)
		{
			std::cout << "ERROR in ResourceManager: " << e.what() << std::endl;
			delete map;
			map = nullptr;
		}
	}

	return map;
}

// Unloads all resources used by the resource manager.
void ResourceManager::Shutdown()
{
	FreeMeshes();
	FreeSpriteSources();
	FreeTextures();
	FreeTilemaps();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Free all meshes associated with any created game objects.
void ResourceManager::FreeMeshes(void)
{
	for (auto it = meshes.begin(); it != meshes.end(); ++it)
	{
		delete it->second;
	}
	meshes.clear();
}

// Free all sprite source objects.
void ResourceManager::FreeSpriteSources(void)
{
	for (auto it = spriteSources.begin(); it != spriteSources.end(); ++it)
	{
		delete it->second;
	}
	spriteSources.clear();
}

// Free all texture objects
void ResourceManager::FreeTextures(void)
{
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		delete it->second;
	}
	textures.clear();
}

void ResourceManager::FreeTilemaps(void)
{
	for (auto it = maps.begin(); it != maps.end(); ++it)
	{
		delete it->second;
	}
	maps.clear();
}
