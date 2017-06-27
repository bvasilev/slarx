#include "automaton.h"
#include "utility.h"
#include <string>
#include <sstream>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>

namespace slarx
{
	std::set<Identifier> DFA::active_automata_ = std::set<Identifier>();
	uint32_t Automaton::last_assigned_id_ = 0;
	
	State::State(std::string& source)
	{
		std::stringstream input_stream(source);
	}

	Alphabet MergeAlphabets(const Alphabet& a, const Alphabet& b)
	{
		return Alphabet(SetUnion<char>(a.GetCharacters(), b.GetCharacters()));
	}

	void ReadAutomatonData(std::ifstream& input_file, uint32_t& number_of_states, Alphabet& alphabet, State& start_state, std::set<State>& accepting_states)
	{
		std::string line;
		getline(input_file, line); // Discard automaton type specifier

		std::vector<int> integers;
		for(int i = 1; i <= 4; ++i)
		{
			getline(input_file, line);
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
						number_of_states = integers[ 0 ];
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
						start_state = State(integers[ 0 ]);
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
	}

	Identifier Automaton::GetIdentifier()
	{
		last_assigned_id_++;
		return Identifier(last_assigned_id_);
	}

	const std::set<Identifier>& Automaton::ListIndentifiers()
	{
		return active_automata_;
	}

	bool Alphabet::ReadAlphabet(const std::string& source)
	{
		std::stringstream input_stream(source);
		char input_char;
		while(input_stream >> input_char)
		{
			characters_.insert(input_char);
		}
		return true;
	}

	void swap(Identifier & a, Identifier & b) noexcept
	{
		using std::swap; 
		swap(a.id_, b.id_);
	}

	void swap(State & a, State & b) noexcept
	{
		using std::swap; 
		swap(a.value_, b.value_);
	}

	void swap(Alphabet & a, Alphabet & b) noexcept
	{
		using std::swap;
		swap(a.characters_, b.characters_);
	}

	void swap(Automaton & a, Automaton & b) noexcept
	{
		using std::swap;
		swap(a.id_, b.id_);
		swap(a.number_of_states_, b.number_of_states_);
		swap(a.alphabet_, b.alphabet_);
		swap(a.start_state_, b.start_state_);
		swap(a.accepting_states_, b.accepting_states_);
	}
}

