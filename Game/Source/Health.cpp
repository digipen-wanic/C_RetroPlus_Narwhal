

#include <stdafx.h>
#include "Health.h"

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	Health::Health(float maxHealth, float startingHealth)
		: Component("Health"), maxHealth(maxHealth), health(startingHealth)
	{
	}

	// Return a new copy of the component.
	Component* Health::Clone() const
	{
		return new Health(*this);
	}

	// Saves object data to a file.
	void Health::Serialize(Parser& parser) const
	{
		UNREFERENCED_PARAMETER(parser);
	}

	// Loads object data from a file.
	void Health::Deserialize(Parser& parser)
	{
		UNREFERENCED_PARAMETER(parser);
	}

	//returns if dead
	bool Health::adjustHealth(float addition)
	{
		health += addition;

		if (health >= 0.0f)
		{
			health = 0.0f;
			return true;
		}

		return true;
	}
}