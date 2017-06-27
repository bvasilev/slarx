#include "conversion_nfa.h"
#include "dfa.h"
#include "automata_set_operations.h"
#include "utility.h"

namespace slarx
{
	// Creates an epsilon NFA of the union of two conversion NFA's
	// Constructs it by adding a new start state and adding two
	// epsilon transitions from it to a's start state and from it to
	// b's start state
	DFA AutomataUnion(const DFA& a, const DFA& b)
	{
		return AutomataUnion(ConversionNFA(a), ConversionNFA(b)).ToDFA();
	}

	ConversionNFA AutomataUnion(const ConversionNFA& a, const ConversionNFA& b)
	{
		uint32_t result_nfa_number_of_states = a.Size() + b.Size() + 1;
		uint32_t a_offset = 1, b_offset = 1 + a.Size();
		Alphabet result_nfa_alphabet = AlphabetUnion(a.GetAlphabet(), b.GetAlphabet());
		result_nfa_alphabet.AddCharacter(kEpsilon);
		State result_nfa_start_state = State(0);
		std::set<State> result_nfa_accepting_states;
		for(State fs : a.GetAcceptingStates())
		{
			result_nfa_accepting_states.insert(State(fs.GetValue() + a_offset));
		}
		for(State fs : b.GetAcceptingStates())
		{
			result_nfa_accepting_states.insert(State(fs.GetValue() + b_offset));
		}
		ConversionNFATransitionTable result_nfa_transition_table(result_nfa_number_of_states, result_nfa_alphabet);
		result_nfa_transition_table.AddTransition(result_nfa_start_state, kEpsilon, State(a.GetStartState().GetValue() + a_offset));
		result_nfa_transition_table.AddTransition(result_nfa_start_state, kEpsilon, State(b.GetStartState().GetValue() + b_offset));

		AddInitialTransitionsToNewTransitionTable(result_nfa_transition_table, a, a_offset);
		AddInitialTransitionsToNewTransitionTable(result_nfa_transition_table, b, b_offset);
		
		return ConversionNFA(result_nfa_number_of_states, result_nfa_alphabet, result_nfa_start_state, result_nfa_accepting_states, result_nfa_transition_table);
	}

	void AddInitialTransitionsToNewTransitionTable(ConversionNFATransitionTable& result_transition_table, const ConversionNFA& input_nfa, uint32_t offset)
	{
		const ConversionNFATransitionTable& input_transition_table = input_nfa.GetTransitionTable();
		for(int i = 0; i < input_transition_table.GetTransitions().size(); ++i)
		{
			for(char c : input_nfa.GetAlphabet().GetCharacters())
			{
				auto transition = input_transition_table.GetTransition(State(i), c);
				for(State to : transition)
				{
					result_transition_table.AddTransition(State(i + offset), c, State(to.GetValue() + offset));
				}
			}
		}
	}
	ConversionNFA AutomataConcatenation(const ConversionNFA& a, const ConversionNFA& b)
	{
		uint32_t result_nfa_number_of_states = a.Size() + b.Size();
		uint32_t a_offset = 0, b_offset = a.Size();
		Alphabet result_nfa_alphabet = AlphabetUnion(a.GetAlphabet(), b.GetAlphabet());
		result_nfa_alphabet.AddCharacter(kEpsilon);
		State result_nfa_start_state = a.GetStartState();
		std::set<State> result_nfa_accepting_states;
		for(State fs : b.GetAcceptingStates())
		{
			result_nfa_accepting_states.insert(State(fs.GetValue() + b_offset));
		}
		ConversionNFATransitionTable result_nfa_transition_table(result_nfa_number_of_states, result_nfa_alphabet);
		for(State fs : a.GetAcceptingStates())
		{
			result_nfa_transition_table.AddTransition(fs, kEpsilon, State(b.GetStartState().GetValue() + b_offset));
		}
		AddInitialTransitionsToNewTransitionTable(result_nfa_transition_table, a, a_offset);
		AddInitialTransitionsToNewTransitionTable(result_nfa_transition_table, b, b_offset);

		return ConversionNFA(result_nfa_number_of_states, result_nfa_alphabet, result_nfa_start_state, result_nfa_accepting_states, result_nfa_transition_table);
	}

	DFA AutomataConcatenation(const DFA& a, const DFA& b)
	{
		return AutomataConcatenation(ConversionNFA(a), ConversionNFA(b)).ToDFA();
	}

	ConversionNFA AutomataKleenyStar(const ConversionNFA& a)
	{
		uint32_t result_nfa_number_of_states = a.Size() + 1;
		Alphabet result_nfa_alphabet = a.GetAlphabet();
		result_nfa_alphabet.AddCharacter(kEpsilon);
		State result_nfa_start_state = State(a.Size());
		std::set<State> result_nfa_accepting_states;
		result_nfa_accepting_states.insert(result_nfa_start_state);

		ConversionNFATransitionTable result_nfa_transition_table(result_nfa_number_of_states, result_nfa_alphabet);
		result_nfa_transition_table.AddTransition(result_nfa_start_state, kEpsilon, a.GetStartState());
		for(State fs : a.GetAcceptingStates())
		{
			result_nfa_transition_table.AddTransition(fs, kEpsilon, result_nfa_start_state);
		}
		AddInitialTransitionsToNewTransitionTable(result_nfa_transition_table, a, 0);

		return ConversionNFA(result_nfa_number_of_states, result_nfa_alphabet, result_nfa_start_state, result_nfa_accepting_states, result_nfa_transition_table);
	}
	DFA AutomataKleenyStar(const DFA& a)
	{
		return AutomataKleenyStar(ConversionNFA(a)).ToDFA();
	}


	Alphabet AlphabetUnion(const Alphabet& a, const Alphabet& b)
	{
		Alphabet result_alphabet;
		for(char c : a.GetCharacters())
		{
			result_alphabet.AddCharacter(c);
		}
		for(char c : b.GetCharacters())
		{
			result_alphabet.AddCharacter(c);
		}
		return result_alphabet;
	}
}