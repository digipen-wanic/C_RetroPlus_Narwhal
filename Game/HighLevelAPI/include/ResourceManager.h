//------------------------------------------------------------------------------
//
// File Name:	ResourceManager.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class SpriteSource;
class Texture;
class Tilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ResourceManager
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	ResourceManager();

	// Destructor
	~ResourceManager();

	// Manually create an association between a mesh and a particular type of object
	// Params:
	//   name = The name to use for the mesh.
	//   mesh = The mesh to add to the manager.
	void AddMesh(const std::string& name, Mesh* mesh);

	// Retrieve an existing mesh required by a game object.
	// Params:
	//   name = The name of the mesh.
	//   createIfNotFound = Whether to create a default quad mesh if no mesh is found.
	Mesh* GetMesh(const std::string& name, bool createIfNotFound = true);

	// Manually add a sprite source to the manager.
	// Params:
	//   spriteSource = The sprite source to add to the manager.
	void AddSpriteSource(SpriteSource* spriteSource);

	// Retrieve a sprite source with the given name, create it if not found.
	// Params:
	//	 name = Name of the sprite source.
	//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
	SpriteSource* GetSpriteSource(const std::string& name, bool createIfNotFound = true);

	// Saves a sprite source to a file so it can be loaded later.
	// Params:
	//   spriteSource = The sprite source being saved to a file. Filename is based on sprite source name.
	void SaveSpriteSourceToFile(const SpriteSource* spriteSource);

	// Manually create an association between a map and a particular type of object
	// Params:
	//   name = The name to use for the map.
	//   map = The map to add to the manager.
	void AddTilemap(const std::string& name, Tilemap* map);

	// Retrieves a texture with the given filename, loads it if not found.
	// Params:
	//   name = Filename of the texture with extension.
	//   createIfNotFound = Whether to load the texture if no texture is found.
	const Texture* GetTexture(const std::string& filename, bool createIfNotFound = true);

	// Retrieves a tilemap with the given filename, loads it if not found.
	// Params:
	//   name = Filename of the tilemap.
	//   createIfNotFound = Whether to load the tilemap if it is not found.
	Tilemap* GetTilemap(const std::string& filename, bool createIfNotFound = true);

	// Unloads all resources used by the resource manager.
	void Shutdown();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Free all meshes associated with any created game objects.
	void FreeMeshes(void);

	// Free all sprite source objects.
	void FreeSpriteSources(void);

	// Free all texture objects
	void FreeTextures(void);

	// Free all tilemap objects
	void FreeTilemaps(void);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Array containing all meshes used by objects
	std::map<const std::string, Mesh*> meshes;

	// Array containing sprite sources used by objects
	std::map<const std::string, SpriteSource*> spriteSources;

	// Array containing textures used by objects
	std::map<const std::string, Texture*> textures;

	// Array containing tilemaps
	std::map<std::string, Tilemap*> maps;
};

//------------------------------------------------------------------------------
