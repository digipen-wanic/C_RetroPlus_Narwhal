//==================================================================-
/*
/file   TimedDeath.cpp
/author Jakob McFarland
/date   10/15/2018
/brief

This is the implementation file for all member functions
of the class TimedDeath, as specified in specification
file TimedDeath.h.

*/
//==================================================================-

#include "stdafx.h"
#include "TimedDeath.h"
#include "GameObject.h"
#include <Parser.h>
#include <GameObjectFactory.h>

namespace Behaviors
{
	//==================================================================-
	// Public Functions:
	//==================================================================-

	// Constructor
	// Params:
	//   timeUntilDeath = Amount of time until the object self-destructs.
	TimedDeath::TimedDeath(float timeUntilDeath)
		: Component("TimedDeath"), timeUntilDeath(timeUntilDeath)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* TimedDeath::Clone() const
	{
		return new TimedDeath(*this);
	}

	// Saves object data to a file.
	void TimedDeath::Serialize(Parser& parser) const
	{
		parser.WriteVariable("timeUntilDeath", timeUntilDeath);
	}

	// Loads object data from a file.
	void TimedDeath::Deserialize(Parser& parser)
	{
		parser.ReadVariable("timeUntilDeath", timeUntilDeath);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void TimedDeath::Update(float dt)
	{
		timeUntilDeath -= dt;

		if (timeUntilDeath <= 0)
		{
			GetOwner()->Destroy();
		}
	}
}