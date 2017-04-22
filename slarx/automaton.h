#pragma once
#ifndef AUTOMATON_H_INCLUDED
#define AUTOMATON_H_INCLUDED

#include <string>
#include <set>
#include <fstream>
#include <vector>

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
        State(const State& other) = delete;
        State& operator=(const State& other) = delete;
    private:
        static const int kUninitialized = -1;
        int value_;
    };

    class Alphabet
    {
    public:
        Alphabet() = default;
        bool ReadAlphabet(const std::string& source);
    private:
        std::set<char> characters_;
    };

    class Automaton
    {
    public:
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

        

    private:
        static std::set<Identifier> active_automata_;
        // ID number, which will be assigned to next created Automaton
        static uint32_t next_available_id_;
        Identifier id_;
        Alphabet alphabet_;
        State start_state_;
        std::set<State> accepting_states_;
    };
}

#endif // AUTOMATON_H_INCLUDED
