#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "slarx.h"

using namespace std;

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

void test_graph(slarx::DFA& d)
{
	//std::cout << 'a';
	std::vector<std::set<int> > f = slarx::FindSCC(d.GetTransitionTable().GetGraph());
	cout << f.size() << endl;
	for(int i = 0; i < f.size(); ++i)
	{
		std::cout << i << " : ";
		for(int u : f[ i ])
		{
			std::cout << u << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "Language empty: " << d.IsLanguageEmpty() << std::endl;
	std::cout << "Language infinite: " << d.IsLanguageInfinite() << std::endl;
	std::cout << std::endl;
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
	test_graph(d1);

	slarx::DFA d2("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa2.txt");
	test1_aux(d2);
	test_graph(d2);

	slarx::DFA d3("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa3.txt");
	test1_aux(d3);
	test_graph(d3);

	slarx::DFA d4("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa4.txt");
	test1_aux(d4);
	test_graph(d4);
}

void test1_aux(slarx::DFA& d)
{
	std::vector<std::string> test_set { "aba", "aaa", "bbb", "abb", "abcdef" };

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