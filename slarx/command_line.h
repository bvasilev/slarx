#pragma once
#ifndef SLARX_COMMAND_LINE_H_INCLUDED
#define SLARX_COMMAND_LINE_H_INCLUDED

// This file contains the command line interface for the slarx library
#include <string>
#include <set>
#include "dfa.h"

namespace slarx
{
	static std::set<DFA> gActiveAutomata;
	void PrintActiveAutomataIdentifiers();
	const DFA* GetAutomatonByID(uint32_t id);

	enum class Command{ kOpen, kList, kPrint, kSave, kIsEmpty, kRecognize, kUnion, kConcatenation, kExit, kInvalid };
	const std::string kOpen = "open";
	const std::string kList = "list";
	const std::string kPrint = "print";
	const std::string kSave = "saveas";
	const std::string kIsEmpty = "empty";
	const std::string kRecognize = "recognize";
	const std::string kUnion = "union";
	const std::string kConcatenation = "concat";
	const std::string kExit = "exit";

	// Initializes execution. Should be used only once at the start of execution
	void Run();
	void PerfromCommand(const std::string& command);
	Command DetermineCommand(const std::string& command);
	void OpenCommand(const std::string& command);
	void ListCommand(const std::string& command);
	void PrintCommand(const std::string& command);
	void SaveCommand(const std::string& command);
	void IsEmptyCommand(const std::string& command);
	void RegocnizeCommand(const std::string& command);
	void UnionCommand(const std::string& command);
	void ConcatenationCommand(const std::string& command);
}

#endif // SLARX_COMMAND_LINE_H_INCLUDED
