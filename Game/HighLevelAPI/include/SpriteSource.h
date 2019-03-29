//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Serializable.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Texture;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class SpriteSource : public Serializable
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for SpriteSource
	// Params:
	//   texture = The texture that contains the frames that this sprite source will use.
	//   name = The name to use for this sprite source.
	//	 numCols = The number of total columns in the sprite sheet texture.
	//	 numRows = The number of total rows in the sprite sheet texture.
	//   frameCount = The number of frames in the sprite source (for animation).
	//   frameStart = The starting frame for the sprite source (for animation).
	SpriteSource(const Texture* texture = nullptr, std::string name = "", unsigned numCols = 1, unsigned numRows = 1, 
		unsigned frameCount = 1, unsigned frameStart = 0);

	// Returns a pointer to the texture, for the purposes of rendering a sprite.
	const Texture* GetTexture() const;

	// Sets the texture used by the sprite source.
	void SetTexture(const Texture* texture);

	// Returns the maximum number of possible frames in the sprite source's texture (rows * cols).
	unsigned GetFrameCountTexture() const;

	// Returns the number of frames used by this sprite source for animation.
	unsigned GetFrameCount() const;

	// Returns the first frame used when animating.
	unsigned GetFrameStart() const;

	// Returns the UV coordinates of the specified frame in a sprite sheet.
	// Params:
	//	 frameIndex = A frame index within a sprite sheet.
	// Returns:
	//   A vector containing the UV/texture coordinates.
	const Vector2D GetUV(unsigned int frameIndex) const;

	// Gets the name of the sprite source.
	const std::string& GetName() const;

	// Gets the name of the texture (for serialization)
	const std::string& GetTextureName() const;

	// Gets the dimensions of the texture (number of columns, rows)
	const Vector2D GetTextureDimensions() const;

	// Save object data to file.
	// Params:
	//   parser = The parser object used to save the object's data.
	void Serialize(Parser& parser) const;

	// Load object data from file
	// Params:
	//   parser = The parser object used to load the object's data.
	void Deserialize(Parser& parser);

	// Relative paths
	static const std::string spriteSourcePath;

private:
	// The dimensions of the sprite sheet texture.
	unsigned numRows;
	unsigned numCols;

	// Animation attributes
	unsigned frameCount;
	unsigned frameStart;

	// Pointer to a texture created using the Low Level API.
	const Texture* texture;

	// Name of sprite source
	std::string name;

	// Name of texture (for serialization)
	std::string textureName;
};

//----------------------------------------------------------------------------
