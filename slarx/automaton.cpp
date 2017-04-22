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

    /*void swap(Automaton& other)
    {
        using std::swap;
        swap(id_, other.id_);
        swap(number_of_states_, other.number_of_states_);
        swap(alphabet_, other.alphabet_);
        swap(start_state_, other.start_state_);
        swap(accepting_states_, other.accepting_states_);
    }*/

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

