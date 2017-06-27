#pragma once
#ifndef SLARX_DFA_H_INCLUDED
#define SLARX_DFA_H_INCLUDED

#include "automaton.h"
#include "graph.h"

#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

namespace slarx
{
	class DFATransitionTable
	{
	public:
		typedef std::vector<std::map<char, State> > TransitionTable;
		DFATransitionTable() = default;
		DFATransitionTable(unsigned dfa_number_of_states, Alphabet& dfa_alphabet) : transitions_(dfa_number_of_states), dfa_alphabet_(dfa_alphabet) { }
		DFATransitionTable(const DFATransitionTable& other) : transitions_(other.transitions_), dfa_alphabet_(other.dfa_alphabet_) { }
		DFATransitionTable(DFATransitionTable&& other) { swap(*this, other); }
		DFATransitionTable& operator=(DFATransitionTable other){ swap(*this, other); return *this; }
		~DFATransitionTable() = default;

		void AddTransition(State from, char on, State to);
		// Returns the transition if it exists, or an uninitialized state (i.e. which has value_ = State::kUninitialized)
		const State GetTransition(State from, char on) const;
		// Returns a graph representation of the transition function (disregarding the characters used for transitions)
		DirectedGraph GetGraph() const;
		// Each DFATransition table should be associated with a single DFA, thus the unique_ptr
		void SetAlphabet(const Alphabet& alphabet) { dfa_alphabet_ = alphabet; }
		void SetNumberOfStates(unsigned dfa_number_of_states){ transitions_.resize(dfa_number_of_states); }
		// Prints all transitions of the DFA formatted one transition on each line, starting with all transitions of state 0, then state 1, etc...
		void PrintTransitions(std::ostream& output_stream) const;
		const TransitionTable& GetTransitions() const { return transitions_; }

		friend void swap(DFATransitionTable& a, DFATransitionTable& b) noexcept;
	private:
		TransitionTable transitions_;
		Alphabet dfa_alphabet_;
	};

	class DFA : public Automaton
	{
	public:
		// Reads a DFA from a file located at path
		DFA(const std::string& path);
		DFA(const DFA& other) : Automaton(other), transition_table_(other.transition_table_) { /*ReportAutomatonWasCreated();*/ /*GetActiveAutomata().insert(std::make_shared<DFA>(this));*/ }
		// Constructor which "cannibalizes" its arguments. Should be used when reading a DFA to ensure that there is sufficient memory before assigning any members.
		DFA(uint32_t&& number_of_states, Alphabet&& alphabet, State&& start_state, 
			std::set<State>&& accepting_states, DFATransitionTable&& transition_table, bool report_automaton_was_created) :
			Automaton(std::move(number_of_states), std::move(alphabet), std::move(start_state), std::move(accepting_states)), transition_table_(transition_table) 
		{ if(report_automaton_was_created) ReportAutomatonWasCreated(); /*GetActiveAutomata().insert(std::make_shared<DFA>(this));*/ }
		
		DFA(DFA&& other, bool report_automaton_was_created) { swap(*this, other); if(report_automaton_was_created) ReportAutomatonWasCreated();  /*GetActiveAutomata().insert(std::make_shared<DFA>(this));*/ }
		DFA& operator=(DFA other){ swap(*this, other); return *this; }
		~DFA() = default;
		
		bool operator<(const DFA& other) const { return (GetIdentifier().GetValue() < other.GetIdentifier().GetValue()); }
		// Reads information for an Automaton from the file located at path 
		virtual bool ReadFromFile(const std::string& path) override;
		// Prints all transitions of the Automaton to target std::ostream
		virtual void PrintTransitions(std::ostream& output_stream) const override;
		// Exports the Automaton to a .at file at location path
		virtual void Export(const std::string& path) const override;

		// Returns true if word is in the automaton's language and false otherwise
		virtual bool Recognize(std::string& word) const override;
		// Answers questions about the properties of the language the Automaton describes
		virtual bool IsLanguageEmpty() const override;
		virtual bool IsLanguageInfinite() const override;

		const DFATransitionTable& GetTransitionTable() const { return transition_table_; }
		friend void swap(DFA& a, DFA& b) noexcept;

	private:
		// Helper funtion for ReadFromFile. Reads a know DFA directly
		bool ReadDFA(const std::string& path);
		// Helper funtion for ReadFromFile. Read an unknown Automaton type or NFA and converts it to a DFA
		bool ReadNFA(const std::string& path);
		State Transition(State from, char on) const { return transition_table_.GetTransition(from, on); }
		DFATransitionTable transition_table_;
	};
}

namespace std
{
	template<>
	struct hash<slarx::State>
	{
		size_t operator()(const slarx::State& a) const { return hash<int>()(a.GetValue()); }
	};
}

#endif // SLARX_DFA_H_INCLUDED