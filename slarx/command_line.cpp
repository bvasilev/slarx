#include "command_line.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "utility.h"
#include "automata_set_operations.h"

namespace slarx
{
	using std::cout; using std::endl;
	void PrintActiveAutomataIdentifiers(std::set<DFA*>& s)
	{
		for(DFA* d : s)
		{
			cout << d->GetIdentifier().GetValue() << " ";
		}

		cout << endl;
	}
	const DFA* GetAutomatonByID(uint32_t id, std::set<DFA*>& s)
	{
		auto iter = find_if(s.begin(), s.end(), [&id](const DFA* d){ return (d->GetIdentifier().GetValue() == id); });
		if(iter != s.end())
		{
			return (*iter);
		}
		else
		{
			return nullptr;
		}
	}

	void Run()
	{
		cout << "Welcome to the slarx command line interface!" << endl << endl;
		std::set<DFA*> active_automata;
		std::string line;
		do
		{
			getline(std::cin, line);
			PerfromCommand(line, active_automata);
		}while(DetermineCommand(line) != Command::kExit);

		for(auto i : active_automata)
			delete i;

		cout << endl << "Goodbye!" << endl;
	}
	void PerfromCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		switch(DetermineCommand(command))
		{
			case Command::kOpen:
				OpenCommand(command, active_automata);
				break;
			case Command::kList:
				ListCommand(command, active_automata);
				break;
			case Command::kPrint:
				PrintCommand(command, active_automata);
				break;
			case Command::kSave:
				SaveCommand(command, active_automata);
				break;
			case Command::kIsEmpty:
				IsEmptyCommand(command, active_automata);
				break;
			case Command::kRecognize:
				RecognizeCommand(command, active_automata);
				break;
			case Command::kUnion:
				UnionCommand(command, active_automata);
				break;
			case Command::kConcatenation:
				ConcatenationCommand(command, active_automata);
				break;
			case Command::kKleeny:
				KleenyClosureCommand(command, active_automata);
				break;
			case Command::kInfinite:
				IsInfiniteCommand(command, active_automata);
				break;
			case Command::kExit:
				break;
			default:
				cout << "Invalid command!" << endl << endl;
		}
	}

	Command DetermineCommand(const std::string& command)
	{
		std::stringstream s(command);
		std::string beg;
		s >> beg;
		if(beg == kOpen)
			return Command::kOpen;
		else if(beg == kList)
			return Command::kList;
		else if(beg == kPrint)
			return Command::kPrint;
		else if(beg == kSave)
			return Command::kSave;
		else if(beg == kIsEmpty)
			return Command::kIsEmpty;
		else if(beg == kRecognize)
			return Command::kRecognize;
		else if(beg == kUnion)
			return Command::kUnion;
		else if(beg == kConcatenation)
			return Command::kConcatenation;
		else if(beg == kKleeny)
			return Command::kKleeny;
		else if(beg == kExit)
			return Command::kExit;
		else if(beg == kInfinite)
			return Command::kInfinite;
		else
			return Command::kInvalid;
	}

	std::string ExtractFilePath(const std::string& command)
	{
		std::stringstream s(command);
		auto beg = std::find(command.begin(), command.end(), '\"');
		auto end = beg;
		if(beg != command.end())
			end = std::find(beg + 1, command.end(), '\"');
		std::string file_path;
		if(beg != command.end() && end != command.end())
			file_path = std::string(beg + 1, end);
		return file_path;
	}

	uint32_t ExtractIdFromCommand(const std::string& command)
	{
		std::stringstream s(command);
		std::string text;
		s >> text; // ignore command text
		s >> text;
		uint32_t id = IntegerParse(text)[ 0 ];
		return id;
	}

	void OpenCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		std::string file_path = ExtractFilePath(command);
		if(!file_path.empty())
			active_automata.insert(new DFA(file_path));
		cout << endl;
	}
	void ListCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		PrintActiveAutomataIdentifiers(active_automata);
		cout << endl;
	}
	void PrintCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		uint32_t id = ExtractIdFromCommand(command);
		const DFA* d = GetAutomatonByID(id, active_automata);
		if(d != nullptr)
		{
			d->PrintTransitions(cout);
			cout << endl;
		}
		else
		{
			cout << "Automaton not found!" << endl << endl;
		}
	}
	void SaveCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		std::stringstream s(command);
		uint32_t id = ExtractIdFromCommand(command);
		std::string file_path = ExtractFilePath(command);
		const DFA* d = GetAutomatonByID(id, active_automata);
		if(d != nullptr)
		{
			if(!file_path.empty())
			{
				d->Export(file_path);
				cout << "Save sucessful!" << endl << endl;
			}
			else
			{
				cout << "Invalid file path" << endl << endl;
			}
		}
		else
		{
			cout << "No such DFA!" << endl << endl;
		}
	}
	void IsEmptyCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		uint32_t id = ExtractIdFromCommand(command);
		const DFA* d = GetAutomatonByID(id, active_automata);
		if(d != nullptr)
		{
			if(d->IsLanguageEmpty())
			{
				cout << "Language is empty" << endl;
			}
			else
			{
				cout << "Language is not empty" << endl;
			}
			cout << endl;
		}
		else
		{
			cout << "Automaton not found!" << endl << endl;
		}
		cout << endl;
	}

	void IsInfiniteCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		uint32_t id = ExtractIdFromCommand(command);
		const DFA* d = GetAutomatonByID(id, active_automata);
		if(d != nullptr)
		{
			if(d->IsLanguageInfinite())
			{
				cout << "Language is infinite" << endl;
			}
			else
			{
				cout << "Language is finite" << endl;
			}
			cout << endl;
		}
		else
		{
			cout << "Automaton not found!" << endl << endl;
		}
		cout << endl;
	}

	void RecognizeCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		std::stringstream s(command);
		uint32_t id = ExtractIdFromCommand(command);
		std::string text;
		s >> text; s >> text; s >> text; // Ignore command text and id
		const DFA* d = GetAutomatonByID(id, active_automata);
		if(d != nullptr)
		{
			if(d->Recognize(text))
			{
				cout << "Yes!" << endl;
			}
			else
			{
				cout << "No." << endl;
			}
		}
		else
		{
			cout << "Automaton not found!" << endl;
		}
		cout << endl;
	}
	void UnionCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		std::stringstream s(command);
		std::string text;
		s >> text; s >> text;
		uint32_t id1 = IntegerParse(text)[0];
		s >> text;
		uint32_t id2 = IntegerParse(text)[0];
		const DFA* d1 = GetAutomatonByID(id1, active_automata);
		const DFA* d2 = GetAutomatonByID(id2, active_automata);
		if(d1 != nullptr && d2 != nullptr)
		{
			active_automata.insert(new DFA(AutomataUnion(*d1, *d2)));
			cout << "Union successful!" << endl;
		}
		else
		{
			cout << "One or both of these automata don't exist" << endl;
		}

		cout << endl;
	}

	void ConcatenationCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		std::stringstream s(command);
		std::string text;
		s >> text; s >> text;
		uint32_t id1 = IntegerParse(text)[ 0 ];
		s >> text;
		uint32_t id2 = IntegerParse(text)[ 0 ];
		const DFA* d1 = GetAutomatonByID(id1, active_automata);
		const DFA* d2 = GetAutomatonByID(id2, active_automata);
		if(d1 != nullptr && d2 != nullptr)
		{
			active_automata.insert(new DFA(AutomataConcatenation(*d1, *d2)));
			cout << "Concatenation successful!" << endl;
		}
		else
		{
			cout << "One or both of these automata don't exist" << endl;
		}

		cout << endl;
	}

	void KleenyClosureCommand(const std::string& command, std::set<DFA*>& active_automata)
	{
		uint32_t id = ExtractIdFromCommand(command);
		const DFA* d = GetAutomatonByID(id, active_automata);
		if(d != nullptr)
		{
			active_automata.insert(new DFA(AutomataKleenyStar(*d)));
			cout << "Kleeny closure successful!" << endl;
		}
		else
		{
			cout << "Automaton does not exist" << endl;
		}
		cout << endl;
	}
}