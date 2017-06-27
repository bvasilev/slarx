#include "slarx.h"

#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>

namespace slarx
{
	void DFATransitionTable::AddTransition(State from, char on, State to)
	{
		if(GetTransition(from, on) != State())
		{
			throw std::invalid_argument("Attemped to add more than one transition from a State on same character.");
		}
		else if(!dfa_alphabet_.Contains(on))
		{
			throw std::domain_error("Attempted to add transition on a character, which is not part of the DFA's alphabet.");
		}
		else
		{
			transitions_[from.GetValue()][on] = to;
		}
	}

	const State DFATransitionTable::GetTransition(State from, char on) const
	{
		auto iterator = transitions_[from.GetValue()].find(on);
		if(iterator != transitions_[from.GetValue()].end())
		{
			return iterator->second;
		}
		else
		{
			return State();
		}
	}

	void DFATransitionTable::PrintTransitions(std::ostream& output_stream) const
	{
		for(int from = 0; from < transitions_.size(); ++from)
		{
			for(auto on_to : transitions_[ from ])
			{
				output_stream << from << ' ' << on_to.first << ' ' << on_to.second.GetValue() << std::endl;
			}
		}
	}

	DirectedGraph DFATransitionTable::GetGraph() const
	{
		DirectedGraph G(transitions_.size());
		for(int i = 0; i < G.size(); ++i)
		{
			for(auto on_to : transitions_[ i ])
			{
				G[i].insert(on_to.second.GetValue());
			}
		}

		return G;
	}

	void swap(DFATransitionTable& a, DFATransitionTable& b) noexcept
	{
		using std::swap; 
		swap(a.transitions_, b.transitions_);
		swap(a.dfa_alphabet_, b.dfa_alphabet_);
	}

	void swap(DFA& a, DFA& b) noexcept
	{
		using std::swap;
		swap(static_cast<Automaton&>(a), static_cast<Automaton&>(b));
		swap(a.transition_table_, b.transition_table_);
	}

	DFA::DFA(const std::string& path)
	{
		ReadFromFile(path);
	}

	bool DFA::ReadFromFile(const std::string& path)
	{
		std::ifstream input_file(path);
		//std::stringstream buffer;
		//buffer << input_file.rdbuf();

		// TODO - add checks to account for different line endings (\n ,\r, \r\n)
		std::string line;
		getline(input_file, line);
		if(line == Automaton::kDFAType())
		{
			input_file.close();
			ReadDFA(path);
		}
		else if(line == Automaton::kUnspecifiedType() || line == Automaton::kNFAType() || line == Automaton::kEpsilonNFAType())
		{
			input_file.close();
			ReadNFA(path);
		}
		else
		{
			input_file.close();
			throw std::invalid_argument("Invalid automaton type specified");
		}
		return false;
	}

	bool DFA::ReadDFA(const std::string& path)
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
		DFATransitionTable transition_table(number_of_states, alphabet);
		//transition_table.SetAlphabet(alphabet);
		int line_number = 5;
		while(getline(input_file, line))
		{
			std::vector<std::string> tokens = Tokenize(line, ' ');
			if(tokens.size() == 3)
			{
				unsigned from = IntegerParse(tokens[0])[0];
				char on = tokens[1][0]; // TODO: this should check if tokens[1] is really a single character
				unsigned to = IntegerParse(tokens[2])[0];
				if(from >= 0 && from <= number_of_states && to >= 0 && to <= number_of_states)
				{
					transition_table.AddTransition(State(from), on, State(to));
				}
				else
				{
					std::stringstream error_message;
					error_message << "Line " << line_number << " (transition of DFA) - transition contains an unexisting state!";
					throw(std::invalid_argument(error_message.str()));
				}
			}
			else
			{
				std::stringstream error_message;
				error_message << "Line " << line_number << " (transition of DFA) contains too many or too few arguments! (should be 3)";
				throw(std::invalid_argument(error_message.str()));
			}

			++line_number;
		}

		if(line_number - 5 != number_of_states * alphabet.Size())
		{
			throw(std::invalid_argument("Too few transitions specified for a DFA! It should have a transition from every state on every character!"));
		}

		*this = DFA(std::move(number_of_states), std::move(alphabet), std::move(start_state), std::move(accepting_states), std::move(transition_table));
		
		Debug("Sucessfully read DFA!");

		return true;
	}

	bool DFA::ReadNFA(const std::string & path)
	{
		ConversionNFA conversion_nfa(path);
		(*this) = conversion_nfa.ToDFA();

		return true;
	}

	void DFA::PrintTransitions(std::ostream& output_stream) const
	{
		transition_table_.PrintTransitions(output_stream);
	}

	void DFA::Export(std::string& path) const
	{
		throw std::domain_error("Function not implemented.");
	}

	bool DFA::Recognize(std::string & word) const
	{
		State current_state = GetStartState();
		for(char c : word)
		{
			if(GetAlphabet().Contains(c))
			{
				current_state = transition_table_.GetTransition(current_state, c);
			}
			else
			{
				return false;
			}
		}
		return IsAccepting(current_state);
	}

	// Returns false if any accepting state is reachable from the start state and true otherwise
	bool DFA::IsLanguageEmpty() const
	{
		return IsLanguageEmpty_AUX(transition_table_.GetGraph(), GetStartState(), GetAcceptingStates());
	}

	bool DFA::IsLanguageInfinite() const
	{
		return IsLanguageInfinite_AUX(transition_table_.GetGraph(), GetStartState(), GetAcceptingStates());
	}

}

