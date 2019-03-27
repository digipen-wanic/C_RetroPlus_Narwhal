//==================================================================-
/*
/file   SriteTilemap.cpp
/author Jakob McFarland
/date   12/4/2018
/brief

This is the implementation file for all member functions
of the class SriteTilemap as specified in specification
file SriteTilemap.h.

*/
//==================================================================-

#include "stdafx.h"
#include "SpriteTilemap.h"

#include "GameObject.h"
#include "Transform.h"
#include "Tilemap.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite for use with a tilemap.
SpriteTilemap::SpriteTilemap()
	: Sprite()
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
	unsigned rows = map->GetHeight();
	unsigned cols = map->GetWidth();

	Vector2D scale = transform->GetScale();

	int cellData;

	for (unsigned row = 0; row < rows; ++row)
	{
		for (unsigned col = 0; col < cols; ++col)
		{
			//foreach cell, get cell data
			cellData = map->GetCellValue(col, row);

			// skip it if empty
			if (cellData <= 0) continue;			

			Sprite::SetFrame(cellData - 1);

			//draw with offset from upper left corner
			Vector2D offset( col * scale.x, row * scale.y * -1 );
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