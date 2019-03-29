//------------------------------------------------------------------------------
//
// File Name:	Animation.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Sprite;
class SpriteSource;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this class as long as you do not
// change the public functions declared in the header.
class Animation : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	// Allocate a new animation object.
	Animation();

	// Returns a dynamically allocated copy of the component.
	// Must be implemented so correct component is copied during copy.
	Component* Clone() const;

	// Initialize components.
	void Initialize() override;

	// Play a simple animation sequence (0 .. frameCount).
	// Params:
	//	 frameDuration = The amount of time to wait between frames (in seconds).
	//	 isLooping = True if the animation loops, false otherwise.
	void Play(float frameDuration, bool isLooping);

	// Update the animation.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void FixedUpdate(float dt);

	// Determine if the animation has reached the end of its sequence.
	// Returns:
	//	 The value in isDone.
	bool IsDone() const;

	// Set the time to wait between frames for the currently playing animation.
	// Params:
	//	 duration = The amount of time to wait between frames (in seconds).
	void SetFrameDuration(float duration);

private:
	// The current frame being displayed.
	unsigned frameIndex;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each frame.
	// (Used only when playing simple animations (0 .. numFrame).)
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation has been reached, false otherwise.
	bool isDone;

	// Components
	Sprite* sprite;

	// Last sprite source used by sprite
	const SpriteSource* lastSpriteSource;
};

//----------------------------------------------------------------------------
