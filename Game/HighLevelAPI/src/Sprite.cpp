//==================================================================-
/*
/file   Sprite.cpp
/author Jakob McFarland
/date   10/28/2018
/brief

This is the implementation file for all member functions
of the class Sprite, as specified in specification
file Sprite.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Sprite.h"
#include <Parser.h>

#include "GameObject.h"
#include "Color.h"
#include "Mesh.h"
#include "Graphics.h"
#include "SpriteSource.h"
#include "Texture.h"
#include "Transform.h"

//==================================================================-
// Public Functions:
//==================================================================-

// Create a new sprite object.
Sprite::Sprite()
	: Component("Sprite"), frameIndex(0), spriteSource(nullptr),
	mesh(nullptr)
{
}

// Clone the sprite, returning a dynamically allocated copy.
Component* Sprite::Clone() const
{
	return new Sprite(*this);
}

// Saves object data to a file.
void Sprite::Serialize(Parser& parser) const
{
	parser.WriteVariable("frameIndex", frameIndex);
	parser.WriteVariable("color", color);
}

// Loads object data from a file.
void Sprite::Deserialize(Parser& parser)
{
	parser.ReadVariable("frameIndex", frameIndex);
	parser.ReadVariable("color", color);
}

// Initialize components.
void Sprite::Initialize()
{
	transform = static_cast<Transform*>( GetOwner()->GetComponent("Transform") );
}

// Draw a sprite (Sprite can be textured or untextured).
void Sprite::Draw()
{
	Draw(Vector2D(0.0f, 0.0f));
}

// Draw a sprite at an offset from the object's translation.
// Params:
//   offset = The offset that will be added to the translation when drawing.
void Sprite::Draw(const Vector2D& offset)
{
	if (mesh == nullptr)
	{
		return;
	}

	if (transform == nullptr)
	{
		return;
	}

	Graphics& graphics = Graphics::GetInstance();

	//do we need to set up a uv?
	//yes
	if (spriteSource != nullptr)
	{
		Vector2D uv;
		spriteSource->GetUV(frameIndex, uv);

		graphics.SetTexture(spriteSource->GetTexture(), uv);
	}
	//no
	else
	{
		graphics.SetTexture(nullptr);
	}

	graphics.SetSpriteBlendColor(color);

	CS230::Matrix2D transformMatrix = CS230::Matrix2D::TranslationMatrix(offset.x, offset.y) * transform->GetMatrix();
	graphics.SetTransform( reinterpret_cast<const Matrix2D&>( transformMatrix ) );

	mesh->Draw();
}


// Set a sprite's transparency (between 0.0f and 1.0f).
// Params:
//   alpha = New value for the sprite's 'alpha' value.
void Sprite::SetAlpha(float alpha) 
{
	color.a = alpha;
}

// Get the current value for a sprite's transparency.
float Sprite::GetAlpha() const
{
	return color.a;
}

// Set the sprite's current frame.
// (NOTE: You must validate the frame index against the sprite source's frame count.)
// Params:
//   frameIndex = New frame index for the sprite (0 .. frame count).
void Sprite::SetFrame(unsigned int frameIndex_)
{
	frameIndex = frameIndex_;
	//std::cout << "Sprite::SetFrame(" << frameIndex << ")" << std::endl;
}

// Returns the index of the current frame being displayed.
unsigned Sprite::GetFrame() const
{
	return frameIndex;
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//   mesh = Pointer to a mesh created using the Alpha Engine.
void Sprite::SetMesh(Mesh* mesh_)
{
	mesh = mesh_;
}

// Set a new SpriteSource for the specified sprite.
// Params:
//	 spriteSource = A new sprite source for the sprite.
void Sprite::SetSpriteSource(SpriteSource* spriteSource_)
{
	spriteSource = spriteSource_;
}

// Set the blend color for the specified sprite.
// Params:
//	 color = A new color for the sprite.
void Sprite::SetColor(Color color_)
{
	color = color_;
}

// Retrieves the blend color for the sprite.
const Color& Sprite::GetColor() const
{
	return color;
}