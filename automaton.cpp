#include "automaton.h"
#include <string>
#include <sstream>
#include <set>

namespace slarx
{
    std::set<Identifier> Automaton::active_automata_ = std::set<Identifier>();
    uint32_t Automaton::next_available_id_ = 1;
    
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
}

