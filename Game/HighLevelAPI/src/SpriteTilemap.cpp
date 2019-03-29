//------------------------------------------------------------------------------
//
// File Name:	SpriteTilemap.cpp
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
#include "SpriteTilemap.h"

// Components
#include "Transform.h"	// GetMatrix
#include "GameObject.h"	// GetComponent

// Systems
#include <Parser.h> // Read/Write Variable
#include <Space.h>	// GetResourceManager

// Resources
#include "Tilemap.h"	// GetWidth, GetHeight, GetCellValue

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite object.
SpriteTilemap::SpriteTilemap()
	: map(nullptr)
{
}

// Clone the sprite, returning a dynamically allocated copy.
Component* SpriteTilemap::Clone() const
{
	return new SpriteTilemap(*this);
}

// Draw a sprite (Sprite can be textured or untextured).
void SpriteTilemap::Draw()
{
	// Set up variables
	int minX = map->GetMinIndexX();
	int maxX = map->GetMaxIndexX();
	int minY = map->GetMinIndexY();
	int maxY = map->GetMaxIndexY();
	Vector2D offset;
	int cellValue = 0;
	const Vector2D& tileSize = transform->GetScale();

	// Loop through cells in map
	for(int r = minY; r < maxY; ++r)
	{
		for (int c = minX; c < maxX; ++c)
		{
			// Set frame index based on cell data
			cellValue = map->GetCellValue(c, r);

			// Skip empty cells
			if (cellValue < 1)
				continue;

			// Set frame index to cell value
			SetFrame(cellValue - 1);

			// Set offset based on cell location and scale
			offset.x = c * tileSize.x;
			offset.y = r * -tileSize.y;

			// Draw the sprite
			Sprite::Draw(offset);
		}
	}
}

// Sets the tilemap data that will be used by the sprite.
// Params:
//   map = A pointer to the tilemap resource.
void SpriteTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file we want to read from.
void SpriteTilemap::Deserialize(Parser & parser)
{
	Sprite::Deserialize(parser);
	std::string tilemapName;
	parser.ReadVariable("tilemap", tilemapName);
	SetTilemap(GetOwner()->GetSpace()->GetResourceManager().GetTilemap(tilemapName));
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file we want to write to.
void SpriteTilemap::Serialize(Parser & parser) const
{
	Sprite::Serialize(parser);
	parser.WriteVariable("tilemap", map->GetName());
}
