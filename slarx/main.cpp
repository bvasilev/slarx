#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "slarx.h"

void test1();
void test1_aux(slarx::DFA& d);
void test_recognize(slarx::DFA& d, std::vector<std::string>& words);

void PrintGraph(slarx::DirectedGraph& G)
{
	for(int u = 0; u < G.size(); ++u)
	{
		std::cout << u << " : ";
		for(int v : G[u])
			std::cout << v << ", ";
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	test1();

	char c;
	std::cin >> c;
	
	return 0;
}



void test1()
{
	slarx::DFA d1("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa1.txt");
	test1_aux(d1);

	slarx::DFA d2("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa2.txt");
	test1_aux(d2);

	slarx::DFA d3("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa3.txt");
	test1_aux(d3);
}

void test1_aux(slarx::DFA& d)
{
	std::vector<std::string> test_set { "aba", "aaa", "bbb", "abb" };

	d.PrintTransitions(std::cout);
	std::cout << "Language: " << d.IsLanguageEmpty() << std::endl;
	test_recognize(d, test_set);
	PrintGraph(d.GetTransitionTable().GetGraph());

	std::cout << std::endl << std::endl;
}

void test_recognize(slarx::DFA& d, std::vector<std::string>& words)
{
	for(auto s : words)
	{
		std::cout << d.Recognize(s) << std::endl;
	}
}