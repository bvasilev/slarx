#pragma once
#ifndef SLARX_COMMAND_LINE_H_INCLUDED
#define SLARX_COMMAND_LINE_H_INCLUDED

// This file contains the command line interface for the slarx library
#include <string>
#include <set>
#include "dfa.h"

namespace slarx
{
	void PrintActiveAutomataIdentifiers(std::set<DFA*>& s);
	const DFA* GetAutomatonByID(uint32_t id, std::set<DFA*>& active_automata);

	enum class Command{ kOpen, kList, kPrint, kSave, kIsEmpty, kRecognize, kUnion, kConcatenation, kKleeny, kKleenyPositive, kExit, kInvalid, kInfinite };
	const std::string kOpen = "open";
	const std::string kList = "list";
	const std::string kPrint = "print";
	const std::string kSave = "save";
	const std::string kIsEmpty = "empty";
	const std::string kRecognize = "reco";
	const std::string kUnion = "union";
	const std::string kConcatenation = "concat";
	const std::string kKleeny = "kleeny";
	const std::string kKleenyPositive = "kleeny+";
	const std::string kExit = "exit";
	const std::string kInfinite = "inf";

	// Initializes execution. Should be used only once at the start of execution
	void Run();
	void PerfromCommand(const std::string& command, std::set<DFA*>& active_automata);
	Command DetermineCommand(const std::string& command);
	bool OpenCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool ListCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool PrintCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool SaveCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool IsEmptyCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool RecognizeCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool UnionCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool IsInfiniteCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool ConcatenationCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool KleenyClosureCommand(const std::string& command, std::set<DFA*>& active_automata);
	bool KleenyPositiveClosureCommand(const std::string& command, std::set<DFA*>& active_automata);
}

#endif // SLARX_COMMAND_LINE_H_INCLUDED
