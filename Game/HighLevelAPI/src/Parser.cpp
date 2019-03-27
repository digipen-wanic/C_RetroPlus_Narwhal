//==================================================================-
/*
/file   Parser.cpp
/author Jakob McFarland
/date   2/6/2019
/brief

This is the implementation file for all member functions
of the class Parser as specified in specification
file Parser.h.

*/
//==================================================================-

#include <stdafx.h>
#include <Parser.h>

// Exception class for problems encountered while reading/writing from files
	
	//constructor
	ParseException::ParseException(const std::string& fileName, const std::string& errorDetails)
		: std::exception( ("Error reading file " + fileName + ". " + errorDetails).c_str() )
	{
	}

// Parser class - reads/writes data object data to/from files
// You are free to change the contents of this structure as long as you do not
// modify the public interface (function prototypes) declared in the header.

	// Opens a file for loading.
	// Returns:
	//   True if load was successful, false otherwise.
	Parser::Parser(const std::string& filename_, std::ios_base::openmode mode)
		: stream(filename_, mode), filename(filename_)
	{
	}

	// Closes the currently open file.
	Parser::~Parser()
	{
		CheckFileOpen();

		stream.close();
	}

	// Begins a new scope when writing to a file.
	// Outputs a curly brace and increases the tab count.
	void Parser::BeginScope()
	{
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; ++i)
		{
			stream << "	";
		}

		stream << "{" << std::endl;

		++indentLevel;
	}

	// Ends the current scope when writing to a file.
	// Outputs an end curly brace and decreases tab count.
	void Parser::EndScope()
	{
		CheckFileOpen();

		--indentLevel;

		for (unsigned i = 0; i < indentLevel; ++i)
		{
			stream << "	";
		}

		stream << "}" << std::endl;
	}

	// Reads a piece of text from the currently open file
	// and skips to the next word afterwards.
	void Parser::ReadSkip(const std::string& text)
	{
		CheckFileOpen();

		std::string input;
		stream >> input;

		if (!input._Equal(text))
		{
			throw ParseException( filename, "A variable with that name wasn't be found.");
		}
	}

	// Skips characters in the stream up until the next
	// occurrence of the given delimiter.
	void Parser::ReadSkip(char delimiter)
	{
		CheckFileOpen();
		stream.ignore(10, delimiter);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Checks if the file was opened correctly. If not, throws an exception.
	void Parser::CheckFileOpen()
	{
		if (!stream.is_open())
		{
			throw ParseException(filename, "The stream wasn't open.");
		}
	}