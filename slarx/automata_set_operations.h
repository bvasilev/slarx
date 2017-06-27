#pragma once
#ifndef AUTOMATA_SET_OPERATIONS_H_INCLUDED
#define AUTOMATA_SET_OPERATIONS_H_INCLUDED

#include "conversion_nfa.h"
#include "dfa.h"

namespace slarx
{
	ConversionNFA AutomataUnion(const ConversionNFA& a, const ConversionNFA& b);
	DFA AutomataUnion(const DFA& a, const DFA& b);
	void AutomataUnion_AUX_AddTransitionsToUnion(ConversionNFATransitionTable& result_transition_table, const ConversionNFA& input_nfa, uint32_t offset);
	ConversionNFA AutomataConcatenation(const ConversionNFA& a, const ConversionNFA& b);
	ConversionNFA AutomataKleenyStar(const ConversionNFA& a, const ConversionNFA& b);

	Alphabet AlphabetUnion(const Alphabet& a, const Alphabet& b);
}


#endif // !UTOMATA_SET_OPERATIONS_H_DEFINED
