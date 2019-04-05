//------------------------------------------------------------------------------PlayerController
//
// File Name:	PlayerController.h
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
class Mesh;
class Crawler;
class Sprite;
class SpriteSource;
class Animation;
class ColliderRectangle;
class SpriteText;
struct MapCollision;
class SoundManager;
class ResourceManager;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class Health;

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
		jumpRtRoll,

		rollLt,
		rollRt,

		waiting,
	};

	class PlayerController : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerController();

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
		void MoveHorizontal(float dt);

		// Moves vertically based on input
		void MoveVertical();

		// shooting logic
		void Shoot(float dt);

		// fire a single shot
		void Fire();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		//world data
		float tileUnit = 100.0f;

		// Movement properties
		float PlayerWalkSpeed;
		float PlayerJumpSpeed;
		float maxJumpHeight;
		Vector2D gravity;
		float maxGravity;

		//shooting properties
		float firingSpeed;
		float firingTimer;
		float bulletSpeed;
		GameObject* bulletArchetype;

		// Components
		Transform* transform;
		Physics* physics;
		Sprite* sprite;
		Animation* animation;
		ColliderRectangle* colliderRect;
		Health* health;

		//sound
		float footstepInterval;
		float footstepTimer;

		// Misc
		float jumpStartY;
		bool onGround;
		bool jumping;

		SpriteText* healthDisplay;
		PlayerState playerState;
		SoundManager* soundManager;
		ResourceManager* resourceManager;
	};
}

//------------------------------------------------------------------------------
