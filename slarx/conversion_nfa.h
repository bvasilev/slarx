#pragma once
#ifndef SLARX_CONVERSION_NFA_H_INCLUDED
#define SLARX_CONVERSION_NFA_H_INCLUDED

#include "dfa.h"
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>
#include <map>

namespace slarx
{
	const char kEpsilon = '~';

	class ConversionNFATransitionTable
	{
	public:
		typedef std::vector<std::map<char, std::set<State> > > TransitionTable;
		ConversionNFATransitionTable() = default;
		ConversionNFATransitionTable(uint32_t automaton_number_of_states, const Alphabet& alphabet) : transitions_(automaton_number_of_states), conversion_nfa_alphabet_(alphabet) { }
		ConversionNFATransitionTable(const ConversionNFATransitionTable& other) : transitions_(other.transitions_), conversion_nfa_alphabet_(other.conversion_nfa_alphabet_) { }
		ConversionNFATransitionTable(ConversionNFATransitionTable&& other) { swap(*this, other); }
		ConversionNFATransitionTable& operator=(ConversionNFATransitionTable other) { swap(*this, other); return *this; }
		~ConversionNFATransitionTable() = default;

		void AddTransition(State from, char on, State to);
		// Returns the transition if it exists, or an uninitialized state (i.e. which has value_ = State::kUninitialized)
		const std::set<State> GetTransition(State from, char on) const;
		void SetAlphabet(const Alphabet& alphabet) { conversion_nfa_alphabet_ = alphabet; }
		friend void swap(ConversionNFATransitionTable& a, ConversionNFATransitionTable& b) noexcept;

	private:
		TransitionTable transitions_;
		Alphabet conversion_nfa_alphabet_;
	};

	// This is a utility class, which is to be used when reading an
	// automaton of unknown type (or a known NFA) or when performing
	// operations on a DFA, which produce an NFA (such as union,
	// concatenation, etc). Its only purpose is to be converted to a
	// DFA and should not be used for anything else
	class ConversionNFA
	{
	public:
		ConversionNFA() = default;
		// TODO - Test this
		ConversionNFA(const ConversionNFA& other) : number_of_states_(other.number_of_states_), alphabet_(other.alphabet_),
			start_state_(other.start_state_), accepting_states_(other.accepting_states_) { }
		ConversionNFA(uint32_t number_of_states, const Alphabet& alphabet, State start_state, const std::set<State>& accepting_states, ConversionNFATransitionTable& transition_table) 
			: number_of_states_(number_of_states), alphabet_(alphabet), start_state_(start_state), accepting_states_(accepting_states), transition_table_(transition_table) { }
		ConversionNFA(const DFA& dfa);
		ConversionNFA(const std::string& path) { ReadFromFile(path); }
		virtual ~ConversionNFA() = default;
		// Reads information for an Automaton from the file located at path 
		bool ReadFromFile(const std::string& path);

		uint32_t Size() const { return number_of_states_; }
		const Alphabet& GetAlphabet() const { return alphabet_; }
		State GetStartState() const { return start_state_; }
		const std::set<State>& GetAcceptingStates() const { return accepting_states_; }
		// TODO - Decide if necessary
		friend void swap(ConversionNFA& a, ConversionNFA& b) noexcept;

		DFA ToDFA();// const;

		// Should be in private!
		// Closes the NFA under epsilon transitions.
		void EpsilonCloseNFA();

	protected:
		// Returns an identifier and increments last_assigned_id_
		// Should be used only when constructing a new Automaton
		bool ReadAlphabet(std::string& source) { return alphabet_.ReadAlphabet(source); }
		void SetNumberOfStates(uint32_t number) { number_of_states_ = number; }
		void SetStartState(State state) { start_state_ = state; }
		void SetAcceptingStates(std::set<State>&& accepting) { accepting_states_ = std::move(accepting); }
		void SetAlphabet(const Alphabet& alphabet) { alphabet_ = alphabet; }
		void SetTransitionTable(ConversionNFATransitionTable&& transition_table){ transition_table_ = std::move(transition_table); }

	private:
		uint32_t number_of_states_;
		Alphabet alphabet_;
		State start_state_;
		std::set<State> accepting_states_;
		ConversionNFATransitionTable transition_table_;

		
		// Produces the epsilon closure of a state
		std::set<State> EpsilonClosure(State state);
	};
}

#endif // SLARX_CONVERSION_NFA_H_INCLUDED