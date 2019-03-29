//------------------------------------------------------------------------------
//
// File Name:	Parser.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Parser.h"

#include "Vector2D.h"
#include <assert.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Opens a file for loading.
// Returns:
//   True if load was successful, false otherwise.
Parser::Parser(const std::string& filename, std::ios_base::openmode mode)
	: stream(filename, mode), filename(filename), indentLevel(0)
{
}

// Closes the currently open file.
Parser::~Parser()
{
	stream.close();
}

// Begins a new scope when writing to a file.
// Outputs a curly brace and increases the tab count.
void Parser::BeginScope()
{
	CheckFileOpen();

	for (unsigned i = 0; i < indentLevel; ++i)
		stream << tab;

	stream << "{" << std::endl;
	indentLevel++;
}

// Ends the current scope when writing to a file.
// Outputs an end curly brace and decreases tab count.
void Parser::EndScope()
{
	CheckFileOpen();

	indentLevel--;

	for (unsigned i = 0; i < indentLevel; ++i)
		stream << tab;

	stream << "}" << std::endl;
}

// Reads a piece of text from the currently open file
// and skips to the next word afterwards.
void Parser::ReadSkip(const std::string & text)
{
	CheckFileOpen();

	std::string nextWord;

	stream >> nextWord;

	if (nextWord != text)
		throw ParseException(filename, "Could not find variable " + text + " in file.\n");
}

// Skips characters in the stream up until the next
// occurrence of the given delimiter.
void Parser::ReadSkip(char delimiter)
{
	CheckFileOpen();
	stream.ignore(10, delimiter);
}

// Checks if the file was opened correctly. If not, throws an exception.
void Parser::CheckFileOpen()
{
	if (!stream.is_open())
	{
		throw ParseException(filename, "Could not open specified file.");
	}
}

// Exception constructor
ParseException::ParseException(const std::string& fileName, const std::string& errorDetails)
	: exception(("Error reading file " + fileName + ". " +  errorDetails).c_str())
{
}
