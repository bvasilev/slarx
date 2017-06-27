#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "slarx.h"

using namespace std;

void test_dfa();
void test1_aux(slarx::DFA& d);
void test_recognize(slarx::DFA& d, std::vector<std::string>& words);
void test_nfa();

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

void test_all()
{
	slarx::DFA d1("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa1.txt");
	//test1_aux(d1);
	//test_graph(d1);

	slarx::DFA d2("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa2.txt");
	//test1_aux(d2);
	//test_graph(d2);

	slarx::DFA d3("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa3.txt");
	//test1_aux(d3);
	//test_graph(d3);

	slarx::DFA d4("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa4.txt");
	//test1_aux(d4);
	//test_graph(d4);

	slarx::DFA d5("D:\\dev\\c++\\slarx\\slarx\\Tests\\aut1.txt");
	//test_graph(d5);

//	slarx::Automaton::PrintActiveAutomataIdentifiers();
}

void test_operations()
{
	slarx::DFA d1("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa1.txt");
	slarx::DFA d2("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa2.txt");
	slarx::DFA n1("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa1.txt");
	slarx::DFA n2("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa2.txt");
	slarx::DFA un1 = AutomataUnion(d1, d2);
	slarx::DFA un2 = AutomataUnion(n1, n2);
	//test1_aux(un1);
	//test1_aux(un2);

//	slarx::Automaton::PrintActiveAutomataIdentifiers();

	/*slarx::DFA d3("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa3.txt");
	slarx::DFA d4("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa4.txt");
	slarx::DFA d5("D:\\dev\\c++\\slarx\\slarx\\Tests\\aut1.txt");*/
//	slarx::Automaton::GetAutomatonByIdentifier(1)->PrintTransitions(cout);
}

void test_export()
{
	slarx::DFA d1("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa1.txt");
	slarx::DFA d2("D:\\dev\\c++\\slarx\\slarx\\Tests\\dfa2.txt");
	slarx::DFA n1("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa1.txt");
	slarx::DFA n2("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa2.txt");
	slarx::DFA un1 = AutomataUnion(d1, d2);
	slarx::DFA un2 = AutomataUnion(n1, n2);
	//d1.PrintTransitions(cout);
	d1.Export("D:\\dev\\c++\\slarx\\slarx\\Tests\\export_dfa1.txt");
	slarx::DFA d1_test("D:\\dev\\c++\\slarx\\slarx\\Tests\\export_dfa1.txt");
	d1_test.PrintTransitions(cout);
	//n1.PrintTransitions(cout);
	n1.Export("D:\\dev\\c++\\slarx\\slarx\\Tests\\export_nfa1.txt");
	//un2.PrintTransitions(cout);
	un2.Export("D:\\dev\\c++\\slarx\\slarx\\Tests\\export_un1.txt");
}

int main(int argc, char* argv[])
{
	slarx::Run();
	
	return 0;
}



void test_dfa()
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

	//slarx::DFA n1("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa1.txt");
}

void test_nfa()
{
	//slarx::DFA n1("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa1.txt");

	slarx::ConversionNFA n2("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa2.txt");

	slarx::DFA d2 = n2.ToDFA();

	slarx::ConversionNFA n3("D:\\dev\\c++\\slarx\\slarx\\Tests\\nfa3.txt");

	slarx::DFA d3 = n3.ToDFA();

	return;
}

void test1_aux(slarx::DFA& d)
{
	std::vector<std::string> test_set { "aba", "aaa", "bbb", "abb", "abcdef" };

	d.PrintTransitions(std::cout);
	std::cout << "Language: " << d.IsLanguageEmpty() << std::endl;
	//test_recognize(d, test_set);
	//PrintGraph(d.GetTransitionTable().GetGraph());

	std::cout << std::endl << std::endl;
}

void test_recognize(slarx::DFA& d, std::vector<std::string>& words)
{
	for(auto s : words)
	{
		std::cout << d.Recognize(s) << std::endl;
	}
}