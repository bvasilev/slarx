#include "conversion_nfa.h"
#include "utility.h"
#include <sstream>
#include <algorithm>
#include <iterator>

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

	const std::set<State> ConversionNFATransitionTable::GetTransition(State from, char on) const
	{
		auto iterator = transitions_[ from.GetValue() ].find(on);
		if(iterator != transitions_[ from.GetValue() ].end())
		{
			return iterator->second;
		}
		else
		{
			return std::set<State>();
		}
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
		alphabet.AddCharacter(kEpsilon);

		ConversionNFATransitionTable transition_table(number_of_states, alphabet);
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
		
		return true;
	}

	ConversionNFA::ConversionNFA(const DFA& dfa) : number_of_states_(dfa.Size()), alphabet_(dfa.GetAlphabet()), start_state_(dfa.GetStartState()), accepting_states_(dfa.GetAcceptingStates()), transition_table_(dfa.Size(), dfa.GetAlphabet())
	{
		alphabet_.AddCharacter(kEpsilon);
		for(size_t from = 0; from < dfa.Size(); ++from)
		{
			for(auto on_to : dfa.GetTransitionTable().GetTransitions()[ from ])
			{
				transition_table_.AddTransition(State(from), on_to.first, on_to.second);
			}
		}
	}

	DFA ConversionNFA::ToDFA()
	{
		std::set<std::set<State> > dfa_states;
		dfa_states.insert(EpsilonClosure(GetStartState())); // Insert start state
		dfa_states.insert(std::set<State>()); // Insert error state
		Alphabet dfa_alphabet = GetAlphabet();
		dfa_alphabet.RemoveCharacter(kEpsilon);

		for(auto powerset_state : dfa_states)
		{
			for(char c : dfa_alphabet.GetCharacters())
			{
				std::set<State> new_state;
				for(State s : powerset_state)
				{
					auto transition = transition_table_.GetTransition(s, c);
					new_state.insert(transition.begin(), transition.end());
				}
				new_state = EpsilonClosure(new_state);
				dfa_states.insert(new_state);
			}
		}

		std::vector<std::set<State> > dfa_states_vector(dfa_states.begin(), dfa_states.end());
		State dfa_start_state = State(std::find(dfa_states_vector.begin(), dfa_states_vector.end(), EpsilonClosure(GetStartState())) - dfa_states_vector.begin());
		State dfa_error_state = State(std::find(dfa_states_vector.begin(), dfa_states_vector.end(), std::set<State>()) - dfa_states_vector.begin());

		DFATransitionTable dfa_transition_table(dfa_states.size(), dfa_alphabet);

		// If there is a transition from any state of the the epsilon closed state i 
		// to any state of the epsilon closed state j on char c
		// we add (i, c, j) to the updated transition table
		for(int i = 0; i < dfa_states_vector.size(); ++i)
		{
			for(int j = i; j < dfa_states_vector.size(); ++j)
			{
				for(char c : dfa_alphabet.GetCharacters())
				{
					std::set<State> i_to_j_transition;
					for(State x : dfa_states_vector[ i ])
					{
						auto transition = transition_table_.GetTransition(x, c);
						i_to_j_transition.insert(transition.begin(), transition.end());
					}
					if(std::equal(i_to_j_transition.begin(), i_to_j_transition.end(), dfa_states_vector[ j ].begin(), dfa_states_vector[ j ].end()))
					{
						dfa_transition_table.AddTransition(State(i), c, State(j));
					}

					if(i != j)
					{
						std::set<State> j_to_i_transition;
						for(State y : dfa_states_vector[ j ])
						{
							auto transition = transition_table_.GetTransition(y, c);
							j_to_i_transition.insert(transition.begin(), transition.end());

						}
						if(std::equal(j_to_i_transition.begin(), j_to_i_transition.end(), dfa_states_vector[ i ].begin(), dfa_states_vector[ i ].end()))
						{
							dfa_transition_table.AddTransition(State(j), c, State(i));
						}
					}
				}

			}
		}

		for(int i = 0; i < dfa_states_vector.size(); ++i)
		{
			for(char c : dfa_alphabet.GetCharacters())
			{
				// If no transition from state i on c, it must go to error state
				if(dfa_transition_table.GetTransition(State(i), c) == State())
				{
					dfa_transition_table.AddTransition(State(i), c, dfa_error_state);
				}
			}
		}

		std::set<State> dfa_accepting_states;
		for(int i = 0; i < dfa_states_vector.size(); ++i)
		{
			std::vector<State> intersection;
			std::set_intersection(dfa_states_vector[ i ].begin(), dfa_states_vector[ i ].end(),
								  GetAcceptingStates().begin(), GetAcceptingStates().end(),
								  std::back_inserter(intersection));

			if(intersection.size() > 0)
				dfa_accepting_states.insert(State(i));
		}


		uint32_t dfa_number_of_states = dfa_states.size();

		return DFA(std::move(dfa_number_of_states), std::move(dfa_alphabet), std::move(dfa_start_state)
				   ,std::move(dfa_accepting_states), std::move(dfa_transition_table), true);
	}

	std::set<State> ConversionNFA::EpsilonClosure(State state)
	{
		std::set<State> epsilon_closure;
		epsilon_closure.insert(state);
		for(State i : epsilon_closure)
		{
			auto transition = transition_table_.GetTransition(i, kEpsilon);
			// Set union
			epsilon_closure.insert(transition.begin(), transition.end());
		}

		return epsilon_closure;
	}

	std::set<State> ConversionNFA::EpsilonClosure(const std::set<State>& state)
	{
		std::set<State> epsilon_closure(state.begin(), state.end());
		for(State s : epsilon_closure)
		{
			auto s_epsilon_closure = EpsilonClosure(s);
			epsilon_closure.insert(s_epsilon_closure.begin(), s_epsilon_closure.end());
		}
		return epsilon_closure;
	}
}

