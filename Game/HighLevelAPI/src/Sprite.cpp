//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
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
#include "Sprite.h"

#include "SpriteSource.h"
#include "Transform.h"
#include "Matrix2DStudent.h"
#include <Matrix2D.h>
#include "Camera.h" // GetViewMatrix
#include "GameObject.h" // GetComponent
#include "ResourceManager.h" // GetSpriteSource
#include "Mesh.h" // Draw
#include "Graphics.h" // SetTexture
#include "MeshFactory.h" // CreateQuadMesh

#include "Parser.h"
#include "Space.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite object.
Sprite::Sprite()
	: Component("Sprite"), transform(nullptr), frameIndex(0), spriteSource(nullptr), mesh(nullptr), zDepth(0.0f)
{
	camera = &Graphics::GetInstance().GetDefaultCamera();
}

// Returns a dynamically allocated copy of the component.
// Must be implemented so correct component is copied during copy.
Component* Sprite::Clone() const
{
	return new Sprite(*this);
}

void Sprite::Initialize()
{
	ResourceManager& resourceManager = GetOwner()->GetSpace()->GetResourceManager();
	// Is there a mesh?
	if (mesh == nullptr)
	{
		// If not, and we need a mesh for an animated sprite
		if (spriteSource != nullptr && spriteSource->GetFrameCountTexture() > 1)
		{
			// Does mesh already exist?
			mesh = resourceManager.GetMesh(GetOwner()->GetName(), false);

			// If not, create custom mesh based on sprite source columns/rows
			if (mesh == nullptr)
			{
				const Vector2D textureDimensions = spriteSource->GetTextureDimensions();
				mesh = MeshFactory::GetInstance().CreateQuadMesh(
					Vector2D(1.0f / textureDimensions.x, 1.0f / textureDimensions.y));
				resourceManager.AddMesh(GetOwner()->GetName(), mesh);
			}
		}
		// It's only a 1x1 sprite source or there is no sprite source,
		else
		{
			// So just find/create the mesh.
			mesh = resourceManager.GetMesh(GetOwner()->GetName());
		}
	}

	// Get the transform component
	transform = GetOwner()->GetComponent<Transform>();
}

// Draw a sprite (Sprite can be textured or untextured).
void Sprite::Draw()
{
	// Call other draw function
	Draw(Vector2D(0.0f, 0.0f));
}

void Sprite::Draw(const Vector2D& offset)
{
	// No mesh - get out!
	if (mesh == nullptr) return;

	if (spriteSource)
	{
		Graphics::GetInstance().SetTexture(spriteSource->GetTexture(), spriteSource->GetUV(frameIndex));
	}
	else
	{
		Graphics::GetInstance().SetTexture(nullptr);
	}

	Graphics::GetInstance().SetCurrentCamera(*camera);
	if (camera != &Graphics::GetInstance().GetDefaultCamera())
		std::cout << camera->GetTranslation() << std::endl;

	// Set blend color
	Graphics::GetInstance().SetSpriteBlendColor(color);

	// Set transform using given translation
	CS230::Matrix2D temp = CS230::Matrix2D::TranslationMatrix(offset.x, offset.y) * transform->GetMatrix();
	Graphics::GetInstance().SetTransform(reinterpret_cast<const Matrix2D&>(temp), zDepth);

	// Draw!
	mesh->Draw();
}

// Set a sprite's transparency (between 0.0f and 1.0f).
// Params:
//   alpha = New value for the sprite's 'alpha' value.
void Sprite::SetAlpha(float alpha)
{
	color.a = max(alpha, 0.0f);
	color.a = min(alpha, 1.0f);
}

// Get the current value for a sprite's transparency.
float Sprite::GetAlpha() const
{
	return color.a;
}

// Set the sprite's current frame.
// (NOTE: You must validate the frame index against the texture's frame count.)
// Params:
//   frameIndex = New frame index for the sprite (0 .. frame count).
void Sprite::SetFrame(unsigned int frameIndex_)
{
	if (frameIndex_ < spriteSource->GetFrameCountTexture())
	{
		frameIndex = frameIndex_;
	}
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//   mesh = Pointer to a mesh created using the Beta Framework.
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

// Returns the current sprite source being used by the sprite.
const SpriteSource * Sprite::GetSpriteSource()
{
	return spriteSource;
}

// Set the tint color for the specified sprite.
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

float Sprite::GetZDepth() const
{
	return zDepth;
}

void Sprite::SetZDepth(float depth_)
{
	zDepth = depth_;
}

// Save object data to file.
// Params:
//   parser = The parser object used to save the object's data.
void Sprite::Serialize(Parser& parser) const
{
	if (spriteSource == nullptr)
	{
		parser.WriteVariable("spriteSource", "null");
	}
	else
	{
		parser.WriteVariable("spriteSource", spriteSource->GetName());
	}

	parser.WriteVariable("color", color);
	parser.WriteVariable("zDepth", zDepth);
}

// Load object data from file
// Params:
//   parser = The parser object used to load the object's data.
void Sprite::Deserialize(Parser& parser)
{
	std::string spriteSourceText;
	parser.ReadVariable("spriteSource", spriteSourceText);

	// Is there a sprite source?
	if (spriteSourceText == "null")
	{
		spriteSource = nullptr;
	}
	else
	{
		// If so, retrieve using resource manager
		//spriteSource = GetOwner()->GetSpace()->GetResourceManager().GetSpriteSource(spriteSourceText);	
	}

	parser.ReadVariable("color", color);
	parser.ReadVariable("zDepth", zDepth);
}

//Set camera to draw this sprite with
void Sprite::SetCamera(Camera& newCamera)
{
	camera = &newCamera;
}
