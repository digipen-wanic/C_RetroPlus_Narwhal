//==================================================================-
/*
/file  GameObjectFactory.cpp
/author Jakob McFarland
/date   2/19/2019
/brief

This is the implementation file for all member functions
of the class GameObjectManager, as specified in specification
file GameObjectManager.h.

*/
//==================================================================-

#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Parser.h"
#include "Transform.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderPoint.h"
#include "ColliderRectangle.h"
#include "ColliderTilemap.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteTilemap.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a single instance of the specified game object.
// Loads the object from a text file (if it exists).
// Params:
//   name = The name of the object.
//   mesh = The mesh used for the object's sprite (if any).
//   spriteSource = The sprite source used for the object's sprite (if any).
// Returns:
//	 If the object file exists and can be loaded, then return a pointer to a new instance 
//    of the specified game object type, else nullptr.
GameObject* GameObjectFactory::CreateObject(const std::string& name,
	Mesh* mesh, SpriteSource* spriteSource)
{
	GameObject* object = new GameObject(name);
	std::string filePath = "Assets/Objects/" + name + ".txt";
	Parser parser( filePath, std::fstream::in );

	try
	{
		object->Deserialize(parser);

		Sprite* sprite = object->GetComponent<Sprite>();

		if ( sprite != nullptr)
		{
			sprite->SetMesh(mesh);
			sprite->SetSpriteSource(spriteSource);
		}
	}
	catch (ParseException& e)
	{
		std::cout << e.what();

		delete object;
	}

	return object;
}

// Create a single instance of the specified component.
// Params:
//   name = The name of the component.
// Returns:
//	 If the component exists, then return a pointer to a new instance 
//    of the specified component, else nullptr.
Component* GameObjectFactory::CreateComponent(const std::string& name)
{
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); ++it)
	{
		std::string type = std::string( typeid( **it ).name() ).substr(6);

		if (type == name)
		{
			return (*it)->Clone();
		}
	}

	return nullptr;
}

// Saves an object to an archetype file so it can be loaded later.
void GameObjectFactory::SaveObjectToFile(GameObject* object)
{
	std::string filePath = "Assets/Objects/" + object->GetName() + ".txt";
	Parser parser("Assets/Objects/" + object->GetName() + ".txt", std::fstream::out);

	try
	{
		object->Serialize(parser);
	}
	catch (ParseException& e)
	{
		std::cout << e.what();

		delete object;
	}

	std::cout << "Wrote the object " + object->GetName() + " to " + filePath << std::endl;
}

// Returns an instance of the factory.
GameObjectFactory& GameObjectFactory::GetInstance()
{
	static GameObjectFactory gameObjectFactory;

	return gameObjectFactory;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
GameObjectFactory::GameObjectFactory()
{
	//low level components
	RegisterComponent<Transform>();
	RegisterComponent<ColliderCircle>();
	RegisterComponent<ColliderPoint>();
	RegisterComponent<ColliderRectangle>();
	RegisterComponent<ColliderTilemap>();
	RegisterComponent<Animation>();
	RegisterComponent<Sprite>();
	RegisterComponent<SpriteTilemap>();
	RegisterComponent<Physics>();
}

// Destructor is private to prevent accidental destruction
GameObjectFactory::~GameObjectFactory()
{
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); ++it)
	{
		delete *it;
	}

	registeredComponents.clear();
}