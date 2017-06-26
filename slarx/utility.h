#pragma once
#ifndef SLARX_UTILITY_H_INCLUDED
#define SLARX_UTILITY_H_INCLUDED

#include <vector>
#include <string>
#include <set>
#include <iterator>

namespace slarx
{
	// Parses an std::string for integers, returning a vector<int> of everything found
	std::vector<int> IntegerParse(const std::string& source);
	// Converts a string into tokens, splitting at delimiter
	std::vector<std::string> Tokenize(std::string& source, char delimiter = ' ');
	
	// Produces the union of two sets
	template<typename T>
	std::set<T> SetUnion(const std::set<T>& a, const std::set<T>& b)
	{
		std::set<T> result;
		result.insert(begin(a), end(a));
		result.insert(begin(b), end(b));
		return result;
	}

	// Utility function for reporting bugs. Should be used only for debug purposes
	void Debug(const std::string& debug_message);
}

#endif // SLARX_UTILITY_H_INCLUDED
