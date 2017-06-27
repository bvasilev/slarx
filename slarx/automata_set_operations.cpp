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

		AutomataUnion_AUX_AddTransitionsToUnion(result_nfa_transition_table, a, a_offset);
		AutomataUnion_AUX_AddTransitionsToUnion(result_nfa_transition_table, b, b_offset);
		
		return ConversionNFA(result_nfa_number_of_states, result_nfa_alphabet, result_nfa_start_state, result_nfa_accepting_states, result_nfa_transition_table);
	}

	void AutomataUnion_AUX_AddTransitionsToUnion(ConversionNFATransitionTable& result_transition_table, const ConversionNFA& input_nfa, uint32_t offset)
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
	ConversionNFA AutomataConcatenation(const ConversionNFA& a, const ConversionNFA& b);
	ConversionNFA AutomataKleenyStar(const ConversionNFA& a, const ConversionNFA& b);


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