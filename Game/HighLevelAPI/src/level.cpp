//==================================================================-
/*
/file   Level.cpp
/author Jakob McFarland
/date   10/23/2018
/brief

This is the implementation file for all member functions
of the class Level, as specified in specification
file Level.h.

*/
//==================================================================-

#include "stdafx.h"
#include "level.h"
#include "Space.h"

//==================================================================-
// Public Methods:
//==================================================================-

// Creates a game state.
// Params:
	//   name = Name to use for this state.
Level::Level(const std::string& name)
	: BetaObject(name)
{
}

// Retrieve the space in which the level is running.
Space* Level::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}