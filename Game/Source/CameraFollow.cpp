//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.cpp
// Author(s):	Nicole Smith
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Component.h" // base class
#include "CameraFollow.h"
#include <Transform.h>
#include <Vector2D.h>
#include <Graphics.h>
#include <Tilemap.h>
#include <GameObject.h>
#include <GameObjectManager.h>
#include <Space.h>
#include <Interpolation.h>
#include <math.h>

namespace Behaviors
{

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   tilemap = tilemap data to find dimensions
	CameraFollow::CameraFollow()
		:	Component("CameraFollow"), playerTransform(nullptr), mapTransform(nullptr), camera(nullptr), tilemap(nullptr)
	{
	}

	// Return a new copy of the component.
	Component* CameraFollow::Clone() const
	{
		return new CameraFollow(*this);
	}

	// Initialize data for this object.
	void CameraFollow::Initialize()
	{
		//get player transform
		playerTransform = GetOwner()->GetComponent<Transform>();
		//get the transform of the tilemap
		mapTransform = GetOwner()->GetSpace()->
			GetObjectManager().GetObjectByName("TileMap")->GetComponent<Transform>();
		//get the transform of the camera
		camera = &Graphics::GetInstance().GetCurrentCamera();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void CameraFollow::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		Vector2D target = playerTransform->GetTranslation();

		//get tilemap dimensions
		float top = -tilemap->GetMinIndexY() * (mapTransform->GetScale().y) + mapTransform->GetTranslation().y;
		float left = tilemap->GetMinIndexX() * (mapTransform->GetScale().x) + mapTransform->GetTranslation().x;
		float bottom = -(tilemap->GetMaxIndexY() - 1.0f) * (mapTransform->GetScale().y) + mapTransform->GetTranslation().y;
		float right = tilemap->GetMaxIndexX() * (mapTransform->GetScale().x) + mapTransform->GetTranslation().x;

		//decrease bounds to fit screen
		top -= Graphics::GetInstance().GetScreenWorldDimensions().extents.y;
		bottom += Graphics::GetInstance().GetScreenWorldDimensions().extents.y;
		left += Graphics::GetInstance().GetScreenWorldDimensions().extents.x;
		right -= Graphics::GetInstance().GetScreenWorldDimensions().extents.x;

		//constrain screen position
		target.x = std::min(right, target.x);
		target.x = std::max(left, target.x);
		target.y = std::min(top, target.y);
		target.y = std::max(bottom, target.y);

		//move camera
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(Interpolate(camera->GetTranslation(), target, 3.0f * dt));
	}

	//Set tilemap
	//Params:
	//	tilemap to set to
	void CameraFollow::SetTileMap(Tilemap* tilemap_)
	{
		tilemap = tilemap_;
	}
}