//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
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
#include "Animation.h"

// Components
#include "Sprite.h" // SetFrame
#include "GameObject.h" // GetComponent

// Resources
#include <SpriteSource.h>

// Systems
#include <Parser.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new animation object.
Animation::Animation()
	: Component("Animation"), frameIndex(0), frameDelay(0.0f),
	frameDuration(0), isRunning(false), isLooping(false), isDone(false), lastSpriteSource(nullptr)
{
}

// Returns a dynamically allocated copy of the component.
// Must be implemented so correct component is copied during copy.
Component * Animation::Clone() const
{
	return new Animation(*this);
}

// Initialize components.
void Animation::Initialize()
{
	sprite = GetOwner()->GetComponent<Sprite>();
}

// Play a simple animation sequence (0 .. frameCount).
// Params:
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(float frameDuration_, bool isLooping_)
{
	frameDuration = frameDuration_;
	frameDelay = frameDuration;
	isLooping = isLooping_;
	isRunning = true;
	frameIndex = sprite->GetSpriteSource()->GetFrameStart();
	isDone = false;
	sprite->SetFrame(frameIndex);
	lastSpriteSource = sprite->GetSpriteSource();
}

// Update the animation.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::FixedUpdate(float dt)
{
	if (isRunning == false) return;

	// If sprite source was changed
	if (sprite->GetSpriteSource() != lastSpriteSource)
	{
		// Start from beginning of animation
		Play(frameDuration, isLooping);
	}

	isDone = false;
	frameDelay -= dt;

	unsigned frameStart = sprite->GetSpriteSource()->GetFrameStart();
	unsigned frameCount = sprite->GetSpriteSource()->GetFrameCount();

	// SIMPLE ANIMATION
	if (frameDelay <= 0.0f)
	{
		frameIndex++;

		// Animation is done
		if (frameIndex > frameStart + frameCount - 1)
		{
			if (isLooping) frameIndex = frameStart;
			else isRunning = false;

			isDone = true;
		}

		float nextDelay = frameDuration;

		// If animation is unfinished or finished and looping, proceed to next frame
		if (!isDone || isLooping)
		{
			sprite->SetFrame(frameIndex);
		}

		frameDelay = nextDelay;
	}
}

// Determine if the animation has reached the end of its sequence.
// Returns:
//	 The value in isDone.
bool Animation::IsDone() const
{
	return isDone;
}

// Set the time to wait between frames for the currently playing animation.
// Params:
//	 duration = The amount of time to wait between frames (in seconds).
void Animation::SetFrameDuration(float duration)
{
	frameDuration = duration;
}
