#pragma once
#ifndef SLARX_UTILITY_H_INCLUDED
#define SLARX_UTILITY_H_INCLUDED

#include <vector>
#include <string>

namespace slarx
{
    std::vector<int> IntegerParse(const std::string& source);
    std::vector<std::string> Tokenize(std::string& source, char delimiter = ' ');
}

namespace std
{

}

#endif // SLARX_UTILITY_H_INCLUDED
