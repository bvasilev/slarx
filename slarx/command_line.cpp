#include "command_line.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

namespace slarx
{
	using std::cout; using std::endl;
	void PrintActiveAutomataIdentifiers()
	{
		for(DFA d : gActiveAutomata)
		{
			cout << d.GetIdentifier().GetValue() << " ";
		}

		cout << endl;
	}
	const DFA* GetAutomatonByID(uint32_t id)
	{
		auto iter = find_if(gActiveAutomata.begin(), gActiveAutomata.end(), [&id](const DFA& d){ return (d.GetIdentifier().GetValue() == id); });
		if(iter != gActiveAutomata.end())
		{
			return &(*iter);
		}
		else
		{
			return nullptr;
		}
	}

	void Run()
	{
		cout << "Welcome to the slarx command line interface!" << endl << endl;
		std::string line;
		do
		{
			getline(std::cin, line);
			PerfromCommand(line);
		}while(DetermineCommand(line) != Command::kExit);

		cout << "Goodbye!" << endl;
	}
	void PerfromCommand(const std::string& command)
	{
		switch(DetermineCommand(command))
		{
			case Command::kOpen:
				OpenCommand(command);
				break;
			case Command::kList:
				ListCommand(command);
				break;
			case Command::kPrint:
			case Command::kSave:
			case Command::kIsEmpty:
			case Command::kRecognize:
			case Command::kUnion:
			case Command::kExit:
				break;
			default:
				cout << "Invalid command!" << endl;
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
		else if(beg == kExit)
			return Command::kExit;
		else
			return Command::kInvalid;
	}

	void OpenCommand(const std::string& command)
	{
		std::stringstream s(command);
		auto beg = std::find(command.begin(), command.end(), '\"');
		auto end = beg;
		if(beg != command.end())
			end = std::find(beg+1, command.end(), '\"');
		std::string file_path;
		if(beg != command.end() && end != command.end())
			file_path = std::string(beg+1, end);
		cout << file_path << endl;
		gActiveAutomata.insert(DFA(file_path));
	}
	void ListCommand(const std::string& command)
	{
		PrintActiveAutomataIdentifiers();
	}
	void PrintCommand(const std::string& command);
	void SaveCommand(const std::string& command);
	void IsEmptyCommand(const std::string& command);
	void RegocnizeCommand(const std::string& command);
	void UnionCommand(const std::string& command);
	void ConcatenationCommand(const std::string& command);
}