//==================================================================-
/*
/file  Serializable.cpp
/author Jakob McFarland
/date   1/19/2019
/brief

This is the default implementation file for all member functions
of the class Serializable, as specified in specification
file Serializable.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Serializable.h"

// Saves object data to a file.
void Serializable::Serialize(Parser& parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

// Loads object data from a file.
void Serializable::Deserialize(Parser& parser)
{
	UNREFERENCED_PARAMETER(parser);
}

Serializable::Serializable() 
{
}

Serializable::~Serializable()
{
}

Serializable::Serializable(const Serializable& other)
{
	UNREFERENCED_PARAMETER(other);
}

Serializable& Serializable::operator=(const Serializable& other)
{
	UNREFERENCED_PARAMETER(other);
	return *this;
}