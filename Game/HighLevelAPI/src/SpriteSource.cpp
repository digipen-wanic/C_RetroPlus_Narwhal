//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"

// Resources
#include <Texture.h>

// Systems
#include <Parser.h>

// Math
#include <Vector2D.h> // SetX and SetY

//------------------------------------------------------------------------------

const std::string SpriteSource::spriteSourcePath = "SpriteSources/";

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
SpriteSource::SpriteSource(const Texture* texture, std::string _name,
	unsigned numCols, unsigned numRows, unsigned frameCount, unsigned frameStart)
	: numCols(numCols), numRows(numRows), frameCount(frameCount),
	frameStart(frameStart), texture(texture), name(_name)
{
	// If name is empty, use name of texture without file extension
	if (name == "" && texture != nullptr)
	{
		// Get texture name without path
		std::string filename = texture->GetName();
		size_t lastSlash = filename.find_last_of('/');
		if (lastSlash != std::string::npos)
			filename = filename.substr(lastSlash + 1);

		// Remove file extension (.png)
		name = filename.substr(0, filename.length() - 4);
	}
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return a pointer to its texture,
//		else return nullptr.
const Texture* SpriteSource::GetTexture() const
{
	return texture;
}

// Sets the texture used by the sprite source.
void SpriteSource::SetTexture(const Texture * _texture)
{
	texture = _texture;
}

// Returns the maximum number of possible frames in the sprite source's texture (rows * cols).
unsigned SpriteSource::GetFrameCountTexture() const
{
	return numCols * numRows;
}

// Returns the first frame used when animating.
unsigned SpriteSource::GetFrameCount() const
{
	return frameCount;
}

// Returns the first frame used when animating.
unsigned SpriteSource::GetFrameStart() const
{
	return frameStart;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// Params:
//	 frameIndex = A frame index within a sprite sheet.
// Returns:
//   A vector containing the UV/texture coordinates.
const Vector2D SpriteSource::GetUV(unsigned int frameIndex) const
{
	Vector2D textureCoords;

	float uSize = 1.0f / numCols;
	float vSize = 1.0f / numRows;

	textureCoords.x = uSize * (frameIndex % numCols);
	textureCoords.y = vSize * (frameIndex / numCols);

	return textureCoords;
}

// Gets the name of the sprite source.
const std::string & SpriteSource::GetName() const
{
	return name;
}

// Gets the name of the texture (for serialization)
const std::string & SpriteSource::GetTextureName() const
{
	if (texture != nullptr)
	{
		return texture->GetName();
	}
	else
	{
		return textureName;
	}
}

// Gets the dimensions of the texture in rows/cols
const Vector2D SpriteSource::GetTextureDimensions() const
{
	return Vector2D(static_cast<float>(numCols), static_cast<float>(numRows));
}

// Save object data to file.
// Params:
//   parser = The parser object used to save the object's data.
void SpriteSource::Serialize(Parser& parser) const
{
	parser.WriteValue(name);
	parser.BeginScope();

	parser.WriteVariable("numRows", numRows);
	parser.WriteVariable("numCols", numCols);

	parser.WriteVariable("frameCount", frameCount);
	parser.WriteVariable("frameStart", frameStart);

	// Extract the filename from the relative path
	std::string filename = texture->GetName();
	size_t lastSlash = filename.find_last_of('/');
	if(lastSlash != std::string::npos)
	filename = filename.substr(lastSlash + 1);

	// Write texture filename to file
	parser.WriteVariable("texture", filename);

	parser.EndScope();
}

// Load object data from file
// Params:
//   parser = The parser object used to load the object's data.
void SpriteSource::Deserialize(Parser& parser)
{
	parser.ReadValue(name);
	parser.ReadSkip('{');

	parser.ReadVariable("numRows", numRows);
	parser.ReadVariable("numCols", numCols);

	parser.ReadVariable("frameCount", frameCount);
	parser.ReadVariable("frameStart", frameStart);

	// Read the texture filename
	parser.ReadVariable("texture", textureName);

	parser.ReadSkip('}');
}
