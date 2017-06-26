#include "conversion_nfa.h"
#include "utility.h"
#include <sstream>

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
		std::ifstream input_file(path);
		std::string line;

		uint32_t number_of_states;
		Alphabet alphabet;
		State start_state;
		std::set<State> accepting_states;
		std::vector<int> integers;
		
		ReadAutomatonData(input_file, number_of_states, alphabet, start_state, accepting_states);

		// TODO - read transitions
		ConversionNFATransitionTable transition_table(number_of_states, alphabet);
		//transition_table.SetAlphabet(alphabet);
		int line_number = 5;
		while(getline(input_file, line))
		{
			std::vector<std::string> tokens = Tokenize(line, ' ');
			if(tokens.size() == 3)
			{
				unsigned from = IntegerParse(tokens[ 0 ])[ 0 ];
				char on = tokens[ 1 ][ 0 ]; // TODO: this should check if tokens[1] is really a single character
				unsigned to = IntegerParse(tokens[ 2 ])[ 0 ];
				if(from >= 0 && from <= number_of_states && to >= 0 && to <= number_of_states)
				{
					transition_table.AddTransition(State(from), on, State(to));
				}
				else
				{
					std::stringstream error_message;
					error_message << "Line " << line_number << " (transition of NFA) - transition contains an unexisting state!";
					throw(std::invalid_argument(error_message.str()));
				}
			}
			else
			{
				std::stringstream error_message;
				error_message << "Line " << line_number << " (transition of NFA) contains too many or too few arguments! (should be 3)";
				throw(std::invalid_argument(error_message.str()));
			}

			++line_number;
		}

		*this = ConversionNFA(number_of_states, alphabet, start_state, accepting_states, transition_table);

		Debug("Sucessfully read NFA!");
		
		return true;
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

