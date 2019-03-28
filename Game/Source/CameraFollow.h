//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.h
// Author(s):	Nicole Smith
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

#include "Component.h" // base class

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Tilemap;
class Camera;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class CameraFollow : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   tilemap = tilemap data to find dimensions
		CameraFollow();

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		//Set tilemap
		//Params:
		// tilemap to set to
		void SetTileMap(Tilemap* tilemap);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Transform* playerTransform;
		Transform* mapTransform;
		Camera* camera;
		Tilemap* tilemap;

		// Properties

		// Other variables

	};
}
