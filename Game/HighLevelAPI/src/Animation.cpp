//==================================================================-
/*
/file   Animation.cpp
/author Jakob McFarland
/date   10/23/2018
/brief

This is the implementation file for all member functions
of the class Animation, as specified in specification
file Animation.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Animation.h"

#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"

//==================================================================-
// Public Functions:
//==================================================================-

// Construct a new animation object.
// Params:
//   sprite = The sprite this animation will be manipulating.
Animation::Animation()
	: Component("Animation"), frameIndex(0), frameCount(0), frameStart(0),
	frameDelay(0), frameDuration(0), isRunning(false), isLooping(false),
	isDone(0)
{
}

// Clone an animation, returning a dynamically allocated copy.
Component* Animation::Clone() const
{
	return new Animation(*this);
}

// Loads object data from a file.
void Animation::Deserialize(Parser& parser)
{
	UNREFERENCED_PARAMETER(parser);
}

// Saves object data to a file.
void Animation::Serialize(Parser& parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

// Initialize components.
void Animation::Initialize() 
{
	sprite = static_cast<Sprite*>( GetOwner()->GetComponent("Sprite") );
}

// Play a simple animation sequence (0 .. frameCount).
// Params:
//   frameStart = The starting frame for the sequence.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(unsigned frameStart_, unsigned frameCount_, float frameDuration_, bool isLooping_)
{
	frameStart = frameStart_;
	frameIndex = frameStart;
	frameCount = frameCount_;
	frameDuration = frameDuration_;
	frameDelay = frameDuration;
	isLooping = isLooping_;
	isRunning = true;

	sprite->SetFrame(frameStart);
}

// Update the animation.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update(float dt)
{
	isDone = false;

	if (!isRunning)
	{
		return;
	}
	
	frameDelay -= dt;

	if (frameDelay <= 0)
	{
		frameDelay = frameDuration;
		unsigned nextFrame = frameIndex + 1;

		if (nextFrame > frameCount)
		{
			isDone = true;
			nextFrame = frameStart;
		}

		if (!isDone || isLooping)
		{
			frameIndex = nextFrame;
			sprite->SetFrame(frameIndex);
		}
		else
		{
			isRunning = true;
		}
	}
}

// Determine if the animation has reached the end of its sequence.
// Returns:
//	 The value in isDone.
bool Animation::IsDone() const
{
	return isDone;
}