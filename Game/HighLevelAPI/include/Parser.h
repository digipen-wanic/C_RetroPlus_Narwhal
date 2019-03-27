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
	ParseException(const std::string& fileName, const std::string& errorDetails);
};

// Parser class - reads/writes data object data to/from files
// You are free to change the contents of this structure as long as you do not
// modify the public interface (function prototypes) declared in the header.
class Parser
{
public:
	// Opens a file for loading.
	// Returns:
	//   True if load was successful, false otherwise.
	Parser(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

	// Closes the currently open file.
	~Parser();

	// Begins a new scope when writing to a file.
	// Outputs a curly brace and increases the tab count.
	void BeginScope();

	// Ends the current scope when writing to a file.
	// Outputs an end curly brace and decreases tab count.
	void EndScope();

	// Writes a variable name and value to the currently open file.
	template<typename T>
	void WriteVariable(const std::string& name, const T& variable)
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
		{
			stream << "	";
		}

		stream << name << " : " << variable << std::endl;
	}

	// Writes a value to the currently open file.
	template<typename T>
	void WriteValue(const T& value)
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
		{
			stream << "	";
		}

		stream << value << std::endl;
	}

	// Reads the value of a variable with the given name from the currently open file.
	// Returns:
	//   True if read was successful, false otherwise.
	template<typename T>
	void ReadVariable(const std::string& name, T& variable)
	{
		CheckFileOpen();

		std::string input;
		stream >> input;

		if (!input._Equal(name))
		{
			throw ParseException(filename, name);
		}

		ReadSkip(':');

		stream >> variable;
	}

	// Reads the next value from the currently open file.
	// Returns:
	//   True if read was successful, false otherwise.
	template<typename T>
	void ReadValue(T& value)
	{
		CheckFileOpen();

		stream >> value;
	}

	// Reads a piece of text from the currently open file
	// and skips to the next word afterwards.
	void ReadSkip(const std::string& text);

	// Skips characters in the stream up until the next
	// occurrence of the given delimiter.
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

	std::fstream stream;
	std::string filename;
	unsigned indentLevel;
	const char* tab = "  ";
};
