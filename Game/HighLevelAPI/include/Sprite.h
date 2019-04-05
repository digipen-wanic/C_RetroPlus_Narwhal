//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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

#include "Component.h"
#include "Color.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class Transform;
class SpriteSource;
class Vector2D;
class Camera;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Sprite : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new sprite object.
	Sprite();

	// Returns a dynamically allocated copy of the component.
	// Must be implemented so correct component is copied during copy.
	Component* Clone() const;

	// Grabs mesh and sprite source from resource manager
	void Initialize();

	// Draw a sprite (Sprite can be textured or untextured).
	void Draw() override;

	// Draw a sprite at an offset from the object's translation.
	// Params:
	//   offset = The offset that will be added to the translation when drawing.
	void Draw(const Vector2D& offset);

	// Set a sprite's transparency (between 0.0f and 1.0f).
	// Params:
	//   alpha = New value for the sprite's 'alpha' value.
	void SetAlpha(float alpha);
	
	// Get the current value for a sprite's transparency.
	float GetAlpha() const;

	// Set the sprite's current frame.
	// (NOTE: You must validate the frame index against the texture's frame count.)
	// Params:
	//   frameIndex = New frame index for the sprite (0 .. frame count).
	void SetFrame(unsigned int frameIndex);

	// Set the sprite's mesh.
	// (NOTE: This mesh may be textured or untextured.)
	// (NOTE: This mesh may contain any number of triangles.)
	// Params:
	//   mesh = Pointer to a mesh created using the Beta Framework.
	void SetMesh(Mesh* mesh);

	// Set a new SpriteSource for the specified sprite.
	// Params:
	//	 spriteSource = A new sprite source for the sprite.
	void SetSpriteSource(SpriteSource* spriteSource);

	// Returns the current sprite source being used by the sprite.
	const SpriteSource* GetSpriteSource();

	// Set the blend color for the specified sprite.
	// Params:
	//	 color = A new color for the sprite.
	void SetColor(Color color);

	// Retrieves the blend color for the sprite.
	const Color& GetColor() const;

	// Retrieves the current depth of the sprite in the scene.
	float GetZDepth() const;

	// Sets the current depth of the sprite in the scene.
	void SetZDepth(float depth);

	// Save object data to file.
	// Params:
	//   parser = The parser object used to save the object's data.
	void Serialize(Parser& parser) const;

	// Load object data from file
	// Params:
	//   parser = The parser object used to load the object's data.
	void Deserialize(Parser& parser);

	//Set camera to draw this sprite with
	void SetCamera(Camera& newCamera);

protected:
	//------------------------------------------------------------------------------
	// Protected Variables:
	//------------------------------------------------------------------------------

	// Components
	Transform* transform;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The sprite source used when drawing (simple colored mesh will have no sprite source).
	SpriteSource* spriteSource;

	// The mesh used to draw the sprite.
	Mesh* mesh;

	// Color used for blending/tint
	Color color;

	// For depth
	float zDepth;

	//Camera for drawing sprite
	Camera* camera;
};

//------------------------------------------------------------------------------
