//------------------------------------------------------------------------------
//
// File Name:	Parser.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <exception>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Exception class for problems encountered while reading/writing from files
class ParseException : public std::exception
{
public:
	// Constructor
	// Params:
	//   fileName = The name of file that the parser was trying to read or write.
	//   errorDetails = The specific details of the error that occurred.
	ParseException(const std::string& fileName, const std::string& errorDetails);
};

// Parser class - reads/writes data object data to/from files
// You are free to change the contents of this structure as long as you do not
// modify the public interface (function prototypes) declared in the header.
class Parser
{
public:
	// Opens a file for loading.
	// Params:
	//   filename = The name of the file to open.
	//   mode = Whether to open the file for reading/writing, etc.
	Parser(const std::string& filename, std::ios_base::openmode mode);

	// Closes the currently open file.
	~Parser();

	// Begins a new scope when writing to a file.
	// Outputs a curly brace and increases the tab count.
	void BeginScope();

	// Ends the current scope when writing to a file.
	// Outputs an end curly brace and decreases tab count.
	void EndScope();

	// Writes a variable name and value to the currently open file.
	// Params:
	//   name = The name of the variable that will be written to the file.
	//   variable =  The value of the variable that will be written to the file.
	template<typename T>
	void WriteVariable(const std::string& name, const T& variable)
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
			stream << tab;
		stream << name << " : " << variable << std::endl;
	}

	// Writes a variable name and array to the currently open file.
	// Params:
	//   name = The name of the array that will be written to the file.
	//   variable = The value of the variable that will be written to the file.
	//   width = The width of the array.
	//   height = The height of the array.
	template<typename T>
	void WriteArrayVariable(const std::string& name, T** const array, size_t width, size_t height)
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
			stream << tab;
		stream << name << " : " << std::endl;
		WriteArray(array, width, height);
	}

	// Writes a variable name and array to the currently open file.
	// Params:
	//   name = The name of the array that will be written to the file.
	//   variable = The value of the variable that will be written to the file.
	//   size = The number of elements in the array.
	template<typename T>
	void WriteArrayVariable(const std::string& name, T* const array, size_t size)
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
			stream << tab;
		stream << name << " : " << std::endl;
		WriteArray(array, size);
	}

	// Writes a value to the currently open file.
	// Params:
	//   value =  The value that will be written to the file.
	template<typename T>
	void WriteValue(const T& value)
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
			stream << tab;
		stream << value << std::endl;
	}

	// Writes an array to the currently open file.
	// Params:
	//   array =  The array that will be written to the file.
	//   width = The width of the array.
	//   height = The height of the array.
	template<typename T>
	void WriteArray(T** const array, size_t width, size_t height)
	{
		CheckFileOpen();

		BeginScope();
		for (size_t r = 0; r < height; ++r)
		{
			for (unsigned i = 0; i < indentLevel; ++i)
					stream << tab;
      
			for (size_t c = 0; c < width; ++c)
				stream << array[c][r] << " ";
      
			stream << std::endl;
		}
		EndScope();
	}

	// Writes an array to the currently open file.
	// Params:
	//   array =  The array that will be written to the file.
	//   width = The number of elements in the array.
	template<typename T>
	void WriteArray(T* const array, size_t size)
	{
		CheckFileOpen();

		BeginScope();
		for (size_t i = 0; i < size; ++i)
		{
			for (unsigned j = 0; j < indentLevel; ++j)
				stream << tab;

			stream << array[i] << std::endl;
		}
		EndScope();
	}

	// Reads the value of a variable with the given name from the currently open file.
	// Params:
	//   name = The name of the variable that should be read from the file.
	//   variable = The variable that should hold the value from the file.
	template<typename T>
	void ReadVariable(const std::string& name, T& variable)
	{
		CheckFileOpen();

		std::string nextWord;
		stream >> nextWord;

		if (nextWord != name)
			throw ParseException(filename, "Could not find variable " + name + " in file.");

		ReadSkip(':');
		stream >> variable;
	}

	// Reads the next value from the currently open file.
	// Params:
	//   value = The variable that should hold the value from the file.
	template<typename T>
	void ReadValue(T& value)
	{
		stream >> value;
	}

	// Reads the value of a array with the given name from the currently open file.
	// Params:
	//   name = The name of the array that should be read from the file.
	//   array = The array that should hold the value from the file.
	//   width = The width of the array.
	//   height = The height of the array.
	template<typename T>
	void ReadArrayVariable(const std::string& name, T** array, size_t width, size_t height)
	{
		CheckFileOpen();

		std::string nextWord;
		stream >> nextWord;

		if (nextWord != name)
			throw ParseException(filename, "Could not find variable " + name + " in file.");

		ReadSkip(':');
		ReadArray(array, width, height);
	}

	// Reads the value of a array with the given name from the currently open file.
	// Params:
	//   name = The name of the array that should be read from the file.
	//   array = The array that should hold the value from the file.
	//   size = The number of elements in the array.
	template<typename T>
	void ReadArrayVariable(const std::string& name, T* array, size_t size)
	{
		CheckFileOpen();

		std::string nextWord;
		stream >> nextWord;

		if (nextWord != name)
			throw ParseException(filename, "Could not find variable " + name + " in file.");

		ReadSkip(':');
		ReadArray(array, size);
	}

	// Reads the next value from the currently open file.
	// Params:
	//   array = The array that should hold the value from the file.
	//   width = The width of the array.
	//   height = The height of the array.
	template<typename T>
	void ReadArray(T** array, size_t width, size_t height)
	{
		T value;
		ReadSkip('{');

		for (size_t r = 0; r < height; ++r)
		{
			for (size_t c = 0; c < width; ++c)
			{
				ReadValue(value);
				array[c][r] = value;
			}
		}
		ReadSkip('}');
	}

	// Reads the next value from the currently open file.
	// Params:
	//   array = The array that should hold the value from the file.
	//   size = The number of elements in the array.
	template<typename T>
	void ReadArray(T* array, size_t size)
	{
		T value;
		ReadSkip('{');

		for (size_t i = 0; i < size; ++i)
		{
			ReadValue(value);
			array[i] = value;
		}
		ReadSkip('}');
	}

	// Reads a piece of text from the currently open file
	// and skips to the next word afterwards.
	// Params:
	//   text = The text to search for in the file.
	void ReadSkip(const std::string& text);

	// Skips characters in the stream up until the next
	// occurrence of the given delimiter.
	//    delimiter = The delimiter to search for in the file.
	void ReadSkip(char delimiter);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Checks if the file was opened correctly. If not, throws an exception.
	void CheckFileOpen();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	std::fstream stream;	// The actual file stream
	std::string filename;	// File that was opened by the stream
	unsigned indentLevel;	// Number of tabs to print
	const char* tab = "\t";	// String to use for tabs
};
