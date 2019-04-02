

#include <stdafx.h>
#include "Health.h"
#include "GameObject.h"
#include "Sprite.h"

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	Health::Health(float maxHealth, float startingHealth, bool isPlayer, float hitRecoveryTime)
		: Component("Health"), maxHealth(maxHealth), health(startingHealth),
		isPlayer(isPlayer), hitTimer(0), hitRecoveryTime(hitRecoveryTime)
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

	// Initialize this component (happens at object creation).
	void Health::Initialize()
	{
		sprite = GetOwner()->GetComponent<Sprite>();
	}

	//returns if dead
	bool Health::adjustHealth(float addition)
	{
		if (hitTimer <= 0.0f)
		{
			health += addition;
			hitTimer += hitRecoveryTime;
			

			if (health <= 0.0f)
			{
				health = 0.0f;
				return true;
			}

			if (isPlayer)
			{
				sprite->SetAlpha(0.5f);
			}
			else 
			{
				sprite->SetColor(Colors::Red);
			}
		}

		return false;
	}

	// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
	void Health::Update(float dt)
	{
		if (hitTimer > 0.0f)
		{
			hitTimer -= dt;

			if (isPlayer && static_cast<int>(hitTimer * 100) % 2 == 0)
			{
				sprite->SetAlpha(0.0f);
			}
			else
			{
				sprite->SetAlpha(0.5f);
			}
		}
		else
		{
			if (isPlayer)
			{
				sprite->SetAlpha(1.0f);
			}
			else
			{
				sprite->SetColor(Colors::White);
			}
		}
	}
}