#include "utility.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <exception>
#include <set>

namespace slarx
{
	// Parses an std::string for integers, returning a vector<int> of everything found
	std::vector<int> IntegerParse(const std::string& source)
	{
		if(std::find_if_not(source.begin(), source.end(), [](int x) -> bool{ return (isdigit(x) || isblank(x) || (x == '\n') || (x == '\r') ); } ) != source.end())
		{
			throw(std::invalid_argument("Failed to parse string of integers - noninteger character(s) detected."));
		}
		std::vector<int> integers;
		std::stringstream input_stream(source);
		int x;
		while(input_stream >> x)
		{
			integers.push_back(x);
		}

		return integers;
	}

	// Converts a string into tokens. Removes trailing '\r' if present
	std::vector<std::string> Tokenize(std::string& source, char delimiter)
	{
		// TODO - Should probably replace this with proper tokenizer (like boost Tokenizer)
		std::vector<std::string> tokens;
		std::stringstream input_stream(source);
		std::string temp;
		if(*source.rbegin() == '\r')
		{
			source.erase(source.end()-1);
		}
		while(getline(input_stream, temp, delimiter))
		{
			tokens.push_back(temp);
		}
		return tokens;
	}

	void Debug(const std::string& debug_message)
	{
		std::cerr << debug_message << std::endl;
	}
}