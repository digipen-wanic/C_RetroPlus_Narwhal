//------------------------------------------------------------------------------
//
// File Name:	Tilemap.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Serializable.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

struct ObjectInMap
{
	// Variables
	mutable GameObject* object;
	std::string name;
	int x;
	int y;

	// Operators
	bool operator==(const ObjectInMap& rhs) const;
};

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Tilemap : public Serializable
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a tilemap from the given parameters.
	// Params:
	//   width = The width of the map.
	//   height = The height of the map.
	//   name = The name of the map.
	//   data = The array containing the map data.
	Tilemap(unsigned width = 0, unsigned height = 0, const std::string& name = "", int** data = nullptr);

	// Destructor.
	~Tilemap();

	// Gets the width of the map (in tiles).
	unsigned GetWidth() const;

	// Get the height of the map (in tiles).
	unsigned GetHeight() const;

	// Gets the minimum and maximum indices
	int GetMinIndexX() const;
	int GetMinIndexY() const;
	int GetMaxIndexX() const;
	int GetMaxIndexY() const;

	// Gets the value of the cell in the map with the given indices.
	// Params:
	//   column = The column of the cell.
	//   row = The row of the cell.
	// Returns:
	//   -1 if the indices are invalid, 0 if the cell is empty, 
	//   or a positive integer otherwise.
	int GetCellValue(int column, int row) const;

	// Sets the value of the cell in the map with the given indices.
	// Params:
	//   column = The column of the cell.
	//   row = The row of the cell.
	//   value = The new value for the cell.
	//   resize = Whether to automatically resize the map if the selected indices are out of bounds.
	//   verbose = Whether to print the tilemap to the console after changing the map.
	void SetCellValue(int column, int row, int value, bool resize = false, bool verbose = false);

	// Return the name of the tilemap.
	const std::string& GetName() const;

	// Sets the name of the tilemap.
	void SetName(const std::string& name);

	// Resizes the map based on the given parameters
	// Params:
	//   width = The new width of the map.
	//   height = The new height of the map.
	//   xShift = Amount to shift elements horizontally.
	//   yShift = Amount to shift elements vertically.
	void Resize(unsigned width, unsigned height, int xShift = 0, int yShift = 0);

	// Resets all tiles to 0.
	// WARNING: This is permanent!
	void Clear();

	// Shrinks map so that any columns or rows on the edge of the map
	// that are completely empty are removed.
	void ShrinkToFit();

	// Add a game object to the map.
	// Params:
	//   object = The game object being added to the map.
	//   x = The column in which to place the object.
	//   y = The row in which to place the object.
	void AddObject(GameObject* object, int x, int y);

	// Remove an object from the map.
	// Params:
	//   x = The column in which to place the object.
	//   y = The row in which to place the object.
	// Returns:
	//   The object that was removed, or nullptr if no object was at that location.
	GameObject* RemoveObject(int x, int y);

	// Retrieves the current list of objects in the map
	const std::vector<ObjectInMap>& GetObjects() const;

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file we want to read from.
	virtual void Deserialize(Parser& parser);

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file we want to write to.
	virtual void Serialize(Parser& parser) const;

	// Asset path for maps
	static const std::string mapPath;

	// Invalid index
	static const int invalidIndex;

	// Output operator for Tilemap
	friend std::ostream& operator<<(std::ostream& stream, const Tilemap& map);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Dimensions of the map
	unsigned width;
	unsigned height;

	// Current offset
	int offsetX;
	int offsetY;

	// The map data (a 2D array)
	int** data;

	// Objects in map
	std::vector<ObjectInMap> objects;

	// Filename
	std::string name;
};

//------------------------------------------------------------------------------
// Global Functions:
//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& stream, const ObjectInMap& object);
std::istream& operator>>(std::istream& stream, ObjectInMap& object);

//------------------------------------------------------------------------------
