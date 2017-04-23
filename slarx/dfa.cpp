#include "slarx.h"

#include <exception>
#include <string>
#include <fstream>
#include <sstream>

namespace slarx
{
	void DFATransitionTable::AddTransition(State from, char on, State to)
	{
		if(transitions_[from.GetValue()].find(on) != transitions_[from.GetValue()].end())
		{
			throw std::invalid_argument("Attemped to add more than one transition from a State on same character.");
		}
		else if(!dfa_alphabet_->Contains(on))
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

	void swap(DFATransitionTable& a, DFATransitionTable& b) noexcept
	{
		using std::swap; 
		swap(a.transitions_, b.transitions_);
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
			ReadDFA(path);
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
		getline(input_file, line);

		uint32_t number_of_states;
		Alphabet alphabet;
		State start_state;
		std::set<State> accepting_states;
		std::vector<int> integers;
		for(int i = 1; i <= 4; ++i)
		{
			switch(i)
			{
				// TODO - Reformat this...
				case 1:
					integers = IntegerParse(line);
					if(integers.size() != 1)
					{
						integers.clear();
						throw std::invalid_argument("The second row specifies an invalid number of states.");
					}
					else
					{
						number_of_states = integers[0];
						integers.clear();
					}
					break;
				case 2:
					alphabet.ReadAlphabet(line);
					break;
				case 3:
					integers = IntegerParse(line);
					if(integers.size() != 1)
					{
						integers.clear();
						throw std::invalid_argument("The fourth row specifies an invalid number for a start state.");
					}
					else
					{
						start_state = State(integers[0]);
						integers.clear();
					}
					break;
				case 4:
					integers = IntegerParse(line);
					for(auto number : integers)
					{
						accepting_states.insert(State(number));
					}
					integers.clear();
					break;
			}
		}

		// TODO - read transitions
		DFATransitionTable transition_table;
		/*
		/ ...
		*/

		*this = DFA(std::move(number_of_states), std::move(alphabet), std::move(start_state), std::move(accepting_states), std::move(transition_table));
		
		return true;
	}

	bool DFA::ReadNFA(const std::string & path)
	{
		throw std::domain_error("Function not implemented.");
		return false;
	}

	void DFA::PrintTransitions(std::ostream& output_stream) const
	{
		throw std::domain_error("Function not implemented.");
	}

	void DFA::Export(std::string& path) const
	{
		throw std::domain_error("Function not implemented.");
	}

	bool DFA::Recognize(std::string & word) const
	{
		throw std::domain_error("Function not implemented.");
		return false;
	}

	bool DFA::IsLanguageEmpty() const
	{
		throw std::domain_error("Function not implemented.");
		return false;
	}

	bool DFA::IsLanguageInfinite() const
	{
		throw std::domain_error("Function not implemented.");
		return false;
	}

}

