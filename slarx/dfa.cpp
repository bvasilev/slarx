#include "dfa.h"

#include <exception>
#include <string>

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

	DFA::DFA(const std::string& path)
	{
		ReadFromFile(path);
	}

	bool DFA::ReadFromFile(const std::string& path)
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

