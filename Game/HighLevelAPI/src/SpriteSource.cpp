//==================================================================-
/*
/file   SpriteSource.cpp
/author Jakob McFarland
/date   10/23/2018
/brief

This is the implementation file for all member functions
of the class SpriteSource, as specified in specification
file SpriteSource.h.

*/
//==================================================================-

#include "stdafx.h"
#include "SpriteSource.h"

#include "Vector2D.h"

//==================================================================-
// Public Functions:
//==================================================================-

// Allocate a new sprite source object.
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 texture = A pointer to a texture that has been loaded by the Alpha Engine.
SpriteSource::SpriteSource(int numCols, int numRows, Texture* texture)
	: numRows(numRows), numCols(numCols), texture(texture)
{
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Returns:
//	 A pointer to the sprite source texture.
Texture* SpriteSource::GetTexture() const
{
	return texture;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Returns:
//	 The calculated frame count.
unsigned SpriteSource::GetFrameCount() const
{
	return numCols * numRows;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 frameIndex = A frame index within a sprite sheet.
//   textureCoords = Reference to a Vector2D containing the UV/texture coordinates.
void SpriteSource::GetUV(unsigned int frameIndex, Vector2D& textureCoords) const
{
	//get uv scalers
	float uSize = 1.0f / static_cast<float>(numCols);
	float vSize = 1.0f / static_cast<float>(numRows);

	//get offset
	int row = frameIndex / numCols;
	int col = frameIndex % numCols;

	//scale and set
	textureCoords.x = static_cast<float>(col) * uSize;
	textureCoords.y = static_cast<float>(row) * vSize;
}
