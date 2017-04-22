#include "automaton.h"
#include <string>
#include <sstream>
#include <set>
#include <utility>

namespace slarx
{
    std::set<Identifier> Automaton::active_automata_ = std::set<Identifier>();
    uint32_t Automaton::last_assigned_id_ = 0;
    
    State::State(std::string& source)
    {
        std::stringstream input_stream(source);
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

