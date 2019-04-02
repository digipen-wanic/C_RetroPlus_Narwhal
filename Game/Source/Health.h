#pragma once

//==================================================================-
/*
/file   Health.h
/author Jakob McFarland
/date   4/1/2019
/brief

This is the specification file for the class level3
*/
//==================================================================-

#include "Component.h" // base class

namespace Behaviors
{
	class Health : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		Health( float maxHealth, float startingHealth, bool isPlayer = false);

		// Return a new copy of the component.
		Component* Clone() const;

		// Saves object data to a file.
		virtual void Serialize(Parser& parser) const override;

		// Loads object data from a file.
		virtual void Deserialize(Parser& parser) override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		//returns if dead
		bool adjustHealth(float addition);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// data
		float health;
		float maxHealth;
		bool isPlayer;
		bool hit;
	};
}