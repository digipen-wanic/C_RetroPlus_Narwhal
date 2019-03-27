//==================================================================-
/*
/file   Tilemap.cpp
/author Jakob McFarland
/date   1/24/2018
/brief

This is the implementation file for all member functions
of the class Tilemap as specified in specification
file Tilemap.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Tilemap.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a tilemap from the given parameters.
// Params:
//   numColumns = The width of the map.
//   numRows = The height of the map.
//   data = The array containing the map data.
Tilemap::Tilemap(unsigned numColumns, unsigned numRows, int** data)
	: numColumns(numColumns), numRows(numRows), data(data)
{
}

// Destructor
Tilemap::~Tilemap()
{
	for (unsigned row = 0; row < numRows; ++row)
	{
		delete[] data[row];
	}

	delete[] data;
}

// Gets the width of the map (in tiles).
unsigned Tilemap::GetWidth() const
{
	return numColumns;
}

// Get the height of the map (in tiles).
unsigned Tilemap::GetHeight() const
{
	return numRows;
}

// Gets the value of the cell in the map with the given indices.
// Params:
//   column = The column of the cell.
//   row = The row of the cell.
// Returns:
//   -1 if the indices are invalid, 0 if the cell is empty, 
//   or a positive integer otherwise.
int Tilemap::GetCellValue(unsigned column, unsigned row) const
{
	if (column >= numColumns || row >= numRows)
	{
		return 0;
	}

	return data[row][column];
}

// Create a tilemap from the given file.
// Params:
//   filename = The name of the file containing the tilemap data.
Tilemap* Tilemap::CreateTilemapFromFile(const std::string& filename)
{
	int cols = 0;
	int rows = 0;

	std::ifstream file;

	file.open(filename);

	if (file.is_open())
	{
		ReadIntegerVariable(file, "width", cols);
		ReadIntegerVariable(file, "height", rows);
		
		int** data = ReadArrayVariable(file, "data", cols, rows);

		return new Tilemap( cols, rows, data );
	}

	return nullptr;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Helper function for reading in values of integers.
// Params:
//   file = The file stream to read from.
//   name = The text to look for before reading the value.
//   variable = If the name is valid, the variable to put the value in.
// Returns:
//   True if a value with the given name was found, false otherwise.
bool Tilemap::ReadIntegerVariable(std::ifstream& file, const std::string& name, int& variable)
{
	std::string buffer;	

	//search for name in file
	while ( !file.eof() )
	{
		file >> buffer;

		//if found, return next value 
		if (name._Equal( buffer ))
		{
			file >> variable;
			std::cout << variable << std::endl;
			return true;
		}
	}
	return false;
}

// Helper function for reading in arrays of integers.
// Params:
//   file = The file stream to read from.
//   name = The text to look for before reading the array.
//   columns = The number of columns in the array.
//   rows = The number of rows in the array.
// Returns:
//   A pointer to the dynamically allocated array if the data is valid, nullptr otherwise.
int** Tilemap::ReadArrayVariable(std::ifstream& file, const std::string& name,
	unsigned columns, unsigned rows)
{
	std::string buffer;

	//search for name in file
	while (!file.eof())
	{
		file >> buffer;

		//if found, read array data 
		if (name._Equal(buffer))
		{
			int** data = new int*[rows];
			for (unsigned col = 0; col < rows; ++col)
			{
				data[col] = new int[columns];
			}

			for (unsigned row = 0; row < rows; ++row)
			{
				for (unsigned col = 0; col < columns; ++col)
				{
					//foreach cell, get next word
					file >> data[row][col];
					std::cout << data[row][col] << " ";
				}

				std::cout << std::endl;
			}

			return data;
		}
	}

	return nullptr;
}