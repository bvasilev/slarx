#pragma once
#ifndef SLARX_DFA_H_INCLUDED
#define SLARX_DFA_H_INCLUDED

#include "automaton.h"

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>

namespace slarx
{
    class DFATransitionTable
    {
    public:
        typedef std::vector<std::unordered_map<char, State> > TransitionTable;
        DFATransitionTable() = default;
        DFATransitionTable(const DFATransitionTable& other) : transitions_(other.transitions_) { }
        DFATransitionTable(DFATransitionTable&& other) { swap(*this, other); }
        DFATransitionTable& operator=(DFATransitionTable other){ swap(*this, other); return *this; }
        ~DFATransitionTable() = default;

        void AddTransition(State from, char on, State to);
		// Returns the transition if it exists, or an uninitialized state (i.e. which has value_ = State::kUninitialized)
        const State GetTransition(State from, char on) const; 
		void SetAlphabet(std::unique_ptr<const Alphabet> alphabet) { dfa_alphabet_ = std::move(alphabet); }
        friend void swap(DFATransitionTable& a, DFATransitionTable& b) noexcept;
    private:
        TransitionTable transitions_;
        std::unique_ptr<const Alphabet> dfa_alphabet_;
    };

    class DFA : public Automaton
    {
    public:
        // Reads a DFA from a file located at path
        DFA(const std::string& path);
        DFA(const DFA& other) : Automaton(other), transition_table_(other.transition_table_) { transition_table_.SetAlphabet(std::make_unique<const Alphabet>(GetAlphabet())); }

		// Reads information for an Automaton from the file located at path 
		virtual bool ReadFromFile(const std::string& path);
		// Prints all transitions of the Automaton to target std::ostream
		virtual void PrintTransitions(std::ostream& output_stream) const;
		// Exports the Automaton to a .at file at location path
		virtual void Export(std::string& path) const;

		// Returns true if word is in the automaton's language and false otherwise
		virtual bool Recognize(std::string& word) const;
		// Answers questions about the properties of the language the Automaton describes
		virtual bool IsLanguageEmpty() const;
		virtual bool IsLanguageInfinite() const;

    private:
        DFATransitionTable transition_table_;
    };
}

#endif // SLARX_DFA_H_INCLUDED