#pragma once
#ifndef SLARX_UTILITY_H_INCLUDED
#define SLARX_UTILITY_H_INCLUDED

#include <vector>
#include <string>

namespace slarx
{
	// Parses an std::string for integers, returning a vector<int> of everything found
	std::vector<int> IntegerParse(const std::string& source);
	// Converts a string into tokens, splitting at delimiter
	std::vector<std::string> Tokenize(std::string& source, char delimiter = ' ');
	// Utility function for reporting bugs. Should be used only for debug purposes
	void Debug(const std::string& debug_message);
}

#endif // SLARX_UTILITY_H_INCLUDED
