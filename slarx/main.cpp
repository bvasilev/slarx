#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "slarx.h"

int main(int argc, char* argv[])
{
    slarx::Identifier id1(1), id2(2);
    using std::swap;
    slarx::Identifier id3 = std::move(slarx::Identifier(3));
    std::cout << id1.GetValue() << ' ' << id2.GetValue() << ' ' << id3.GetValue() << std::endl;

    char c;
    std::cin >> c;
    
    return 0;
}