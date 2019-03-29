//------------------------------------------------------------------------------
//
// File Name:	Tilemap.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Tilemap.h"

#include <Engine.h>	// GetFilePath
#include <Parser.h> // ReadVariable
#include <GameObject.h> // GetName

//------------------------------------------------------------------------------

const std::string Tilemap::mapPath = "Levels/";
const int Tilemap::invalidIndex = -1;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a tilemap from the given parameters.
// Params:
//   width = The width of the map.
//   height = The height of the map.
//   name = The name of the map.
//   data = The array containing the map data.
Tilemap::Tilemap(unsigned width_, unsigned height_, const std::string& name, int** data)
	: width(0), height(0), offsetX(0), offsetY(0), data(data), name(name)
{
	// If no data was passed, but we know the size of the map
	if (data == nullptr && width_ != 0 && height_ != 0)
	{
		// Set width and height and allocate memory for the map
		Resize(width_, height_);
	}
}

// Destructor.
Tilemap::~Tilemap()
{
	// Delete data array
	for (unsigned i = 0; i < width; ++i)
		delete[] data[i];
	delete[] data;
}

// Gets the width of the map (in tiles).
unsigned Tilemap::GetWidth() const
{
	return width;
}

// Get the height of the map (in tiles).
unsigned Tilemap::GetHeight() const
{
	return height;
}

int Tilemap::GetMinIndexX() const
{
	return -offsetX;
}

int Tilemap::GetMinIndexY() const
{
	return -offsetY;
}

int Tilemap::GetMaxIndexX() const
{
	return width - offsetX;
}

int Tilemap::GetMaxIndexY() const
{
	return height - offsetY;
}

// Gets the value of the cell with the given index.
// Params:
//   column = The column of the cell.
//   row = The row of the cell.
// Returns:
//   -1 if the indices are invalid, 0 if the cell is empty, 
//   or a positive integer otherwise.
int Tilemap::GetCellValue(int column, int row) const
{
	unsigned actualColumn = column + offsetX;
	unsigned actualRow = row + offsetY;

	// Make sure column and row are valid
	if (actualColumn >= width || actualRow >= height)
		return invalidIndex;
	else
		return data[actualColumn][actualRow];
}

// Sets the value of the cell in the map with the given indices.
// Params:
//   column = The column of the cell.
//   row = The row of the cell.
//   value = The new value for the cell.
//   resize = Whether to automatically resize the map if the selected indices are out of bounds.
//   verbose = Whether to print the tilemap to the console after changing the map.
// Returns:
//   The amount that items in the map were shifted to accommodate the resize (if one occurred).
void Tilemap::SetCellValue(int column, int row, int value, bool resize, bool verbose)
{
	UNREFERENCED_PARAMETER(resize);
	int currentWidth = width;
	int currentHeight = height;
	int actualColumn = column + offsetX;
	int actualRow = row + offsetY;

	int xShift = 0;
	int yShift = 0;

	// If cell does not exist
	if (GetCellValue(column, row) == invalidIndex)
	{
		// No resize? Get out!
		if (!resize)
			return;

		xShift = currentWidth / 2;
		yShift = currentHeight / 2;

		// Outside map width (negative)
		if (actualColumn < 0)
		{
			xShift = max(currentWidth, abs(actualColumn));
		}
		// Outside map height (negative)
		if (actualRow < 0)
		{
			yShift = max(currentHeight, abs(actualRow));
		}

		actualColumn += xShift;
		actualRow += yShift;

		// Double size of map when possible (to reduce likelihood of future resizes)
		unsigned newWidth = max(currentWidth + abs(actualColumn), currentWidth * 2);
		unsigned newHeight = max(currentHeight + abs(actualRow), currentHeight * 2);
		Resize(newWidth, newHeight, xShift, yShift);

		std::cout << "Grow" << std::endl;
	}

	data[actualColumn][actualRow] = value;

	if(verbose)
		std::cout << *this << std::endl;

	return;
}

// Return the name of the tilemap.
const std::string & Tilemap::GetName() const
{
	return name;
}

void Tilemap::SetName(const std::string & name_)
{
	name = name_;
}

// Resizes the map based on the given parameters
// Params:
//   width = The new width of the map.
//   height = The new height of the map.
//   xShift = Amount to shift elements horizontally.
//   yShift = Amount to shift elements vertically.
void Tilemap::Resize(unsigned width_, unsigned height_, int xShift, int yShift)
{
	// No actual change - return
	if (width_ == width && height_ == height)
		return;

	// Allocate new memory
	int** dataNew = new int*[width_];
	for (size_t c = 0; c < width_; ++c)
	{
		dataNew[c] = new int[height_];
		memset(dataNew[c], 0, sizeof(int) * height_);
	}

	// Copy from old into new
	unsigned minHeight = min(height, height_);
	unsigned minWidth = min(width, width_);
	unsigned xShiftNew = xShift > 0 ? xShift : 0;
	unsigned yShiftNew = yShift > 0 ? yShift : 0;
	unsigned xShiftOld = xShift < 0 ? -xShift : 0;
	unsigned yShiftOld = yShift < 0 ? -yShift : 0;

	for (size_t y = 0; y < minHeight; ++y)
	{
		for (size_t x = 0; x < minWidth; ++x)
		{
			size_t xNew = x + xShiftNew;
			size_t yNew = y + yShiftNew;
			size_t xOld = x + xShiftOld;
			size_t yOld = y + yShiftOld;
			dataNew[xNew][yNew]
				= data[xOld][yOld];
		}
	}

	// Delete old data
	for (size_t c = 0; c < width; ++c)
		delete[] data[c];
	delete[] data;

	// Set width and height
	width = width_;
	height = height_;

	// Set data
	data = dataNew;

	// Adjust offsets
	offsetX += xShift;
	offsetY += yShift;
}

// Resets all tiles to 0
void Tilemap::Clear()
{
	for (size_t c = 0; c < width; ++c)
		memset(data[c], 0, sizeof(int) * height);
}

// Shrinks map so that any columns or rows on the edge of the map
// that are completely empty are removed.
// Returns:
//   The amount that items in the map were shifted to accommodate the resize (if one occurred).
void Tilemap::ShrinkToFit()
{
	// Find empty rows
	bool empty = false;
	// Top
	int shrinkTop = 0;
	empty = true;
	for (unsigned r = 0; r < height / 2; ++r)
	{
		for (unsigned c = 0; c < width; ++c)
		{
			if (data[c][r] != 0)
			{
				empty = false;
				break;
			}
		}
		if (!empty) break;
		++shrinkTop;
	}

	// Bottom
	unsigned shrinkBottom = 0;
	empty = true;
	for (unsigned r = height - 1; r >= height / 2; --r)
	{
		for (unsigned c = 0; c < width; ++c)
		{
			if (data[c][r] != 0)
			{
				empty = false;
				break;
			}
		}
		if (!empty) break;
		++shrinkBottom;
	}

	// Find empty columns
	// Left
	int shrinkLeft = 0;
	empty = true;
	for (unsigned c = 0; c < width / 2; ++c)
	{
		for (unsigned r = 0; r < height; ++r)
		{
			if (data[c][r] != 0)
			{
				empty = false;
				break;
			}
		}
		if (!empty) break;
		else ++shrinkLeft;;
	}

	// Right
	unsigned shrinkRight = 0;
	empty = true;
	for (unsigned c = width - 1; c >= width / 2; --c)
	{
		for (unsigned r = 0; r < height; ++r)
		{
			if (data[c][r] != 0)
			{
				empty = false;
				break;
			}
		}
		if (!empty) break;
		else ++shrinkRight;;
	}

	// Resize
	Resize(width - (shrinkLeft + shrinkRight), height - (shrinkBottom + shrinkTop), -shrinkLeft, -shrinkTop);

	std::cout << "Shrink" << std::endl;
	std::cout << *this << std::endl;
}

// Add a game object to the map.
// Params:
//   object = The game object being added to the map.
//   x = The column in which to place the object.
//   y = The row in which to place the object.
void Tilemap::AddObject(GameObject * object, int x, int y)
{
	ObjectInMap objMap = { object, object->GetName(), x, y };
	objects.push_back(objMap);
}

// Remove an object from the map.
// Params:
//   x = The column in which to place the object.
//   y = The row in which to place the object.
// Returns:
//   The object that was removed, or nullptr if no object was at that location.
GameObject * Tilemap::RemoveObject(int x, int y)
{
	GameObject* foundObject = nullptr;
	ObjectInMap objMap = { nullptr, "", x, y };

	auto location = std::find(objects.begin(), objects.end(), objMap);
	if (location != objects.end())
	{
		foundObject = (*location).object;
		objects.erase(location);
	}

	return foundObject;
}

// Retrieves the current list of objects in the map
const std::vector<ObjectInMap>& Tilemap::GetObjects() const
{
	return objects;
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file we want to read from.
void Tilemap::Deserialize(Parser & parser)
{
	parser.ReadValue(name);
	parser.ReadSkip('{');

	// Read data
	unsigned width_, height_;
	parser.ReadVariable("width", width_);
	parser.ReadVariable("height", height_);
	parser.ReadVariable("offsetX", offsetX);
	parser.ReadVariable("offsetY", offsetY);

	// Set width, height, allocate memory
	Resize(width_, height_);
	parser.ReadArrayVariable("tileLayer", data, width, height);

	// Read size of object layer and resize as necessary
	size_t numObjects;
	parser.ReadVariable("numObjects", numObjects);
	objects.resize(numObjects);
	parser.ReadArrayVariable("objectLayer", objects.data(), numObjects);

	parser.ReadSkip('}');
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file we want to write to.
void Tilemap::Serialize(Parser & parser) const
{
	parser.WriteValue(name);
	parser.BeginScope();

	// Small data
	parser.WriteVariable("width", width);
	parser.WriteVariable("height", height);
	parser.WriteVariable("offsetX", offsetX);
	parser.WriteVariable("offsetY", offsetY);

	// Big data
	parser.WriteArrayVariable("tileLayer", data, width, height);
	parser.WriteVariable("numObjects", objects.size());
	parser.WriteArrayVariable("objectLayer", objects.data(), objects.size());

	parser.EndScope();
}

std::ostream & operator<<(std::ostream & stream, const Tilemap & map)
{
	for (size_t y = 0; y < map.height; ++y)
	{
		for (size_t x = 0; x < map.width; ++x)
			stream << map.data[x][y] << " ";
		stream << std::endl;
	}
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const ObjectInMap & object)
{
	return stream << "{ " << object.name << " , " << object.x << " , " << object.y << " }";
}

std::istream & operator>>(std::istream & stream, ObjectInMap & object)
{
	object.object = nullptr;
	stream.ignore(10, '{'); stream >> object.name;
	stream.ignore(10, ','); stream >> object.x;
	stream.ignore(10, ','); stream >> object.y;
	stream.ignore(10, '}');
	return stream;
}

bool ObjectInMap::operator==(const ObjectInMap & rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

//------------------------------------------------------------------------------
