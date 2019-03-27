//==================================================================-
/*
/file   Component.cpp
/author Jakob McFarland
/date   11/15/2018
/brief

This is the implementation file for all member functions
of the class Component, as specified in specification
file Component.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Component.h"

#include "GameObject.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor - needed to construct BetaObject.
Component::Component(const std::string& name)
	: BetaObject(name)
{
}

// Retrieve component's owner object (if any).
GameObject* Component::GetOwner() const
{
	return static_cast<GameObject*>(GetParent());
}