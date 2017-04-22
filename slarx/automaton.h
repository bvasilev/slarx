#pragma once
#ifndef SLARX_AUTOMATON_H_INCLUDED
#define SLARX_AUTOMATON_H_INCLUDED

#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <iostream>

namespace slarx
{
    class Identifier
    {
    public:
        int GetValue(){ return id_; }
        Identifier() : id_(kUninitialized) { }
        explicit Identifier(uint32_t id) : id_(id) { }
        Identifier(const Identifier& other) = delete;
        Identifier& operator=(const Identifier& other) = delete;
        Identifier(Identifier&& other){ swap(*this, other); }
        friend void swap(Identifier& a, Identifier& b) { using std::swap; swap(a.id_, b.id_); }
    private:
        static const int kUninitialized = -1;
        int id_;
    };

    class State
    {
    public:
        int GetValue(){ return value_; }
        State() : value_(kUninitialized) { }
        explicit State(uint32_t value) : value_(value) { }
        State(std::string& source);
        State(const State& other) = default;
        State& operator=(State other) { swap(*this, other); return *this; };
        State(State&& other) { swap(*this, other); }
        friend void swap(State& a, State& b) { using std::swap; swap(a.value_, b.value_); }
    private:
        static const int kUninitialized = -1;
        int value_;
    };

    class Alphabet
    {
    public:
        Alphabet() = default;
        Alphabet(const std::set<char>& characters) : characters_(characters) { }
        Alphabet(const std::string& alphabet) { ReadAlphabet(alphabet); }
        Alphabet(const Alphabet& other_alphabet) : characters_(other_alphabet.characters_) { }
        Alphabet& operator=(Alphabet other) { swap(*this, other); return *this; }
        bool ReadAlphabet(const std::string& source);
        friend void swap(Alphabet& a, Alphabet& b) { using std::swap; swap(a.characters_, b.characters_); }
    private:
        std::set<char> characters_;
        // Used only for debugging purposes
        void Debug_PrintAlphabet(std::ostream& out){ for(auto i : characters_ ) out << i << ' '; }
    };

    class Automaton
    {
    public:
        Automaton() = default;
        // TODO - Test this
        Automaton(const Automaton& other) : number_of_states_(other.number_of_states_), alphabet_(other.alphabet_),
            start_state_(other.start_state_), accepting_states_(other.accepting_states_), id_(Automaton::GetIdentifier()) { }
        // Returns a std::set containing the identifiers of all currently availiable Automata
        static const std::set<Identifier>& ListIndentifiers();
        // Reads information for an Automaton from the file located at path 
        virtual bool ReadFromFile(const std::string& path) = 0;
        // Prints all transitions of the Automaton to target std::ostream
        virtual void PrintTransitions(std::ostream& output_stream) = 0;
        // Exports the Automaton to a .at file at location path
        virtual void Export(std::string& path) = 0;

        // Returns true if word is in the automaton's language and false otherwise
        virtual bool Recognize(std::string& word) = 0;
        // Answers questions about the properties of the language the Automaton describes
        virtual bool IsLanguageEmpty() = 0;
        virtual bool IsLanguageInfinite() = 0;

        uint32_t Size() { return number_of_states_; }
        const Alphabet& GetAlphabet() { return alphabet_; }
        State GetStartState() { return start_state_; }
        const std::set<State>& GetAcceptingStates() { return accepting_states_; }
        // TODO - Decide if necessary
        friend void swap(Automaton& a, Automaton& b);

    protected:
        // Returns an identifier and increments last_assigned_id_
        // Should be used only when constructing a new Automaton
        static Identifier GetIdentifier();
    private:
        static std::set<Identifier> active_automata_;
        // ID number, which will be assigned to next created Automaton
        static uint32_t last_assigned_id_;
        Identifier id_;
        uint32_t number_of_states_;
        Alphabet alphabet_;
        State start_state_;
        std::set<State> accepting_states_;
    };
}


#endif // SLARX_AUTOMATON_H_INCLUDED
