#include "conversion_nfa.h"

void slarx::swap(ConversionNFATransitionTable & a, ConversionNFATransitionTable & b) noexcept
{
	using std::swap;
	swap(a.transitions_, b.transitions_);
	swap(a.conversion_nfa_alphabet_, b.conversion_nfa_alphabet_);
}

void slarx::swap(ConversionNFA & a, ConversionNFA & b) noexcept
{
	using std::swap;
	swap(a.number_of_states_, b.number_of_states_);
	swap(a.alphabet_, b.alphabet_);
	swap(a.start_state_, b.start_state_);
	swap(a.accepting_states_, b.accepting_states_);
	swap(a.transition_table_, b.transition_table_);
}

void slarx::ConversionNFATransitionTable::AddTransition(State from, char on, State to)
{
	transitions_[from.GetValue()][on].insert(to);
}

bool slarx::ConversionNFA::ReadFromFile(const std::string& path)
{
	return false;
}
