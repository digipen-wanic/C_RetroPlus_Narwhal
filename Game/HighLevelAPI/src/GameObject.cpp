//==================================================================-
/*
/file  GameObject.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class GameObject, as specified in specification
file GameObject.h.

*/
//==================================================================-

#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Parser.h"
#include "Component.h"
#include "Space.h"

//typedef std::vector<Component*> ComponentContainer;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new game object.
// Params:
//	 name = The name of the game object being created.
GameObject::GameObject(const std::string& name)
	: BetaObject(name), numComponents(0), isDestroyed(false)
{
	components.reserve(10);
}

// Clone a game object from another game object.
// Params:
//	 other = A reference to the object being cloned.
GameObject::GameObject(const GameObject& other)
	: BetaObject(other.GetName()), numComponents(0), isDestroyed(other.isDestroyed)
{
	for ( auto i = other.components.begin(); i != other.components.end(); ++i)
	{
		Component* clone = (*i)->Clone();

		clone->SetParent(this);

		AddComponent( clone );
	}
}

// Free the memory associated with a game object.
GameObject::~GameObject()
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		delete (*i);
	}
}

// Saves object data to a file.
void GameObject::Serialize(Parser& parser) const
{
	//Use the parser to write the object's name to the file
	parser.WriteValue( GetName() );

	//	BeginScope
	parser.BeginScope();

	//	Write the number of components to the file as a variable
	parser.WriteVariable("numComponents", numComponents);

	//	For each component
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		//	Write the component's name to the file (use std::string(typeid(*components[i]).name()).substr(6))
		parser.WriteValue(std::string(typeid(*(*i)).name()).substr(6));

		//	BeginScope
		parser.BeginScope();
		//	Serialize the component
		(*i)->Serialize(parser);

		//	EndScope
		parser.EndScope();
	}
	//	EndScope
	parser.EndScope();
}
// Loads object data from a file.
void GameObject::Deserialize(Parser& parser)
{
	//Use the parser's ReadSkip function to check that the name of the object matches the name in the file
	parser.ReadSkip( GetName() );

	//Use ReadSkip('{') to go forward to the item after the next opening bracket
	parser.ReadSkip('{');

	unsigned compenentsToAdd;

	//Use ReadVariable to read the number of components into an unsigned integer
	parser.ReadVariable("numComponents", compenentsToAdd);

	//For each component
	for (unsigned i = 0; i < compenentsToAdd; ++i)
	{
		//Use ReadValue to read the component's name into a string
		std::string compName;
		parser.ReadValue(compName);
		//Use the GameObjectFactory's CreateComponent function to create a component with the given name
		Component* component = GameObjectFactory::GetInstance().CreateComponent(compName);

		//If the component returned by the factory is null, throw a ParseException with the component's
		//name as the file name, and a message about how the given component could not be found.
		if (component == nullptr) throw ParseException(GetName(), "Component wasn't registered with the factory." + compName);

		//Add the component to the game object
		AddComponent(component);

		//ReadSkip the next opening bracket
		parser.ReadSkip('{');

		//Deserialize the component
		component->Deserialize(parser);

		//ReadSkip the next closing bracket
		parser.ReadSkip('}');
	}
	//ReadSkip the next closing bracket
	parser.ReadSkip('}');
}

// Initialize this object's components and set it to active.
void GameObject::Initialize()
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		(*i)->Initialize();
	}
}

// Update any components attached to the game object.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObject::Update(float dt)
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		(*i)->Update(dt);
	}
}

// Updates components using a fixed timestep (usually just physics)
// Params:
//	 dt = A fixed change in time, usually 1/60th of a second.
void GameObject::FixedUpdate(float dt)
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		(*i)->FixedUpdate(dt);
	}
}

// Draw any visible components attached to the game object.
void GameObject::Draw()
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		(*i)->Draw();
	}
}

// Adds a component to the object.
void GameObject::AddComponent(Component* component)
{
	component->SetParent(this);
	components.push_back(component);
	++numComponents;
}

// Retrieves the component with the given name if it exists.
// Params:
//   name = The name of the component to find.
Component* GameObject::GetComponent(const std::string& name_)
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		if ((*i)->GetName()._Equal(name_))
		{
			return *i;
		}
	}

	return nullptr;
}

// Mark an object for destruction.
void GameObject::Destroy()
{
	isDestroyed = true;
}

// Whether the object has been marked for destruction.
// Returns:
//		True if the object will be destroyed, false otherwise.
bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

// Get the space that contains this object.
Space* GameObject::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}