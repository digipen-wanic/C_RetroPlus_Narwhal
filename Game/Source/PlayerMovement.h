//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.h
// Author(s):	Jakob McFarland
//
//------------------------------------------------------------------------------

#pragma once
 
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

#include "Vector2D.h" // Vector2D


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	enum PlayerState
	{
		standing,

		idleLtUp,
		idleRtUp,
		idleLt,
		idleRt,

		runLtUp,
		runLt,
		runRtUp,
		runRt,

		jumpLt,
		jumpLtUp,
		jumpRt,
		jumpRtUp,

		jumpLtRoll,
		jumpLtRoll
	};

	class PlayerMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerMovement();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Saves object data to a file.
		virtual void Serialize(Parser& parser) const override;

		// Loads object data from a file.
		virtual void Deserialize(Parser& parser) override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Map collision handler for Monkey objects.
		// Params:
		//   object = The monkey object.
		//   collision = Which sides the monkey collided on.
		friend void PlayerMapCollisionHandler(GameObject& object,
			const MapCollision& collision);

		// Collision handler for monkey.
		// Params:
		//   object = The monkey.
		//   other  = The object the monkey is colliding with.
		friend void PlayerCollisionHandler(GameObject& object, GameObject& other);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal();

		// Moves vertically based on input
		void MoveVertical();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		//world data
		float tileUnit = 200.0f;

		// Movement properties
		float PlayerWalkSpeed;
		float PlayerJumpSpeed;
		float maxJumpHeight;
		Vector2D gravity;

		// Components
		Transform* transform;
		Physics* physics;

		// Misc
		float jumpStartY;
		bool onGround;
		PlayerState playerState;
	};
}

//------------------------------------------------------------------------------
