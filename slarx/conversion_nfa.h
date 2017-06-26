#pragma once
#ifndef SLARX_CONVERSION_NFA_H_INCLUDED
#define SLARX_CONVERSION_NFA_H_INCLUDED

#include "dfa.h"
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>

namespace slarx
{
	class ConversionNFATransitionTable
	{
	public:
		typedef std::vector<std::unordered_map<char, std::unordered_set<State> > > TransitionTable;
		ConversionNFATransitionTable() = default;
		ConversionNFATransitionTable(const ConversionNFATransitionTable& other) : transitions_(other.transitions_) { }
		ConversionNFATransitionTable(ConversionNFATransitionTable&& other) { swap(*this, other); }
		ConversionNFATransitionTable& operator=(ConversionNFATransitionTable other) { swap(*this, other); return *this; }
		~ConversionNFATransitionTable() = default;

		void AddTransition(State from, char on, State to);
		// Returns the transition if it exists, or an uninitialized state (i.e. which has value_ = State::kUninitialized)
		const std::unordered_set<State> GetTransition(State from, char on) const;
		void SetAlphabet(std::unique_ptr<const Alphabet> alphabet) { conversion_nfa_alphabet_ = std::move(alphabet); }
		friend void swap(ConversionNFATransitionTable& a, ConversionNFATransitionTable& b) noexcept;
	private:
		TransitionTable transitions_;
		std::unique_ptr<const Alphabet> conversion_nfa_alphabet_;
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
		ConversionNFA(const DFA& dfa);
		virtual ~ConversionNFA() = default;
		// Reads information for an Automaton from the file located at path 
		bool ReadFromFile(const std::string& path);

		uint32_t Size() const { return number_of_states_; }
		const Alphabet& GetAlphabet() const { return alphabet_; }
		State GetStartState() const { return start_state_; }
		const std::set<State>& GetAcceptingStates() const { return accepting_states_; }
		// TODO - Decide if necessary
		friend void swap(ConversionNFA& a, ConversionNFA& b) noexcept;

	protected:
		// Returns an identifier and increments last_assigned_id_
		// Should be used only when constructing a new Automaton
		bool ReadAlphabet(std::string& source) { return alphabet_.ReadAlphabet(source); }
		void SetNumberOfStates(uint32_t number) { number_of_states_ = number; }
		void SetStartState(State state) { start_state_ = state; }
		void SetAcceptingStates(std::set<State>&& accepting) { accepting_states_ = std::move(accepting); }

	private:
		uint32_t number_of_states_;
		Alphabet alphabet_;
		State start_state_;
		std::set<State> accepting_states_;
		ConversionNFATransitionTable transition_table_;
	};
}

#endif // SLARX_CONVERSION_NFA_H_INCLUDED