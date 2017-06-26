#include "conversion_nfa.h"

namespace slarx
{
	void swap(ConversionNFATransitionTable & a, ConversionNFATransitionTable & b) noexcept
	{
		using std::swap;
		swap(a.transitions_, b.transitions_);
		swap(a.conversion_nfa_alphabet_, b.conversion_nfa_alphabet_);
	}

	void swap(ConversionNFA & a, ConversionNFA & b) noexcept
	{
		using std::swap;
		swap(a.number_of_states_, b.number_of_states_);
		swap(a.alphabet_, b.alphabet_);
		swap(a.start_state_, b.start_state_);
		swap(a.accepting_states_, b.accepting_states_);
		swap(a.transition_table_, b.transition_table_);
	}

	void ConversionNFATransitionTable::AddTransition(State from, char on, State to)
	{
		transitions_[ from.GetValue() ][ on ].insert(to);
	}

	bool ConversionNFA::ReadFromFile(const std::string& path)
	{
		return false;
	}

	ConversionNFA::ConversionNFA(const DFA& dfa) : number_of_states_(dfa.Size()), alphabet_(dfa.GetAlphabet()), start_state_(dfa.GetStartState()), accepting_states_(dfa.GetAcceptingStates())
	{
		// Add epsilon
		alphabet_.AddCharacter('~');
		for(size_t from = 0; from < dfa.Size(); ++from)
		{
			for(auto on_to : dfa.GetTransitionTable().GetTransitions()[ from ])
			{
				transition_table_.AddTransition(State(from), on_to.first, on_to.second);
			}
		}
	}
}

