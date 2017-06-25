#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "slarx.h"

int main(int argc, char* argv[])
{
	slarx::DFA d("D:\\dev\\c++\\slarx\\slarx\\Tests\\test1.txt");
	d.PrintTransitions(std::cout);

	char c;
	std::cin >> c;
	
	return 0;
}