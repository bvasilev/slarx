#pragma once
#ifndef SLARX_AUTOMATON_H_INCLUDED
#define SLARX_AUTOMATON_H_INCLUDED

#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <iostream>
#include <memory>

namespace slarx
{
	class Identifier
	{
	public:
		int GetValue() const { return id_; }
		Identifier() : id_(kUninitialized) { }
		explicit Identifier(uint32_t id) : id_(id) { }
		Identifier(const Identifier& other) = delete;
		Identifier& operator=(const Identifier& other) = delete;
		Identifier(Identifier&& other){ swap(*this, other); }
		~Identifier() = default;

		bool operator<(const Identifier& other) const { return id_ < other.id_; }

		friend void swap(Identifier& a, Identifier& b) noexcept;
	private:
		static const int kUninitialized = -1;
		int id_;
	};

	class State
	{
	public:
		int GetValue() const { return value_; }
		bool IsInitialized() const { return value_ != kUninitialized; }
		State() : value_(kUninitialized) { }
		explicit State(uint32_t value) : value_(value) { }
		State(std::string& source);
		State(const State& other) = default;
		State(State&& other) { swap(*this, other); }
		State& operator=(State other) { swap(*this, other); return *this; };
		~State() = default;

		bool operator==(const State& other) const { return value_ == other.value_; }
		bool operator<(const State& other) const { return value_ < other.value_; }
		bool operator!=(const State& other) const { return !(*this == other); }
		friend void swap(State& a, State& b) noexcept;
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
		Alphabet(const Alphabet& other) : characters_(other.characters_) { }
		Alphabet& operator=(Alphabet other) { swap(*this, other); return *this; }
		~Alphabet() = default;

		bool ReadAlphabet(const std::string& source);
		bool Contains(char c) const { return std::find(characters_.begin(), characters_.end(), c) != characters_.end(); }
		size_t Size(){ return characters_.size(); }
		const std::set<char>& GetCharacters() const { return characters_; }
		void AddCharacter(char c){ characters_.insert(c); }
		void RemoveCharacter(char c){ characters_.erase(c); }

		friend void swap(Alphabet& a, Alphabet& b) noexcept;
	private:
		std::set<char> characters_;
		// Used only for debugging purposes
		void Debug_PrintAlphabet(std::ostream& out){ for(auto i : characters_ ) out << i << ' '; }
	};

	// Merges the characters of two alphabets, returning the result (Set union)
	Alphabet MergeAlphabets(const Alphabet& a, const Alphabet& b);

	class Automaton
	{
	public:
		static constexpr char* kUnspecifiedType(){ return "Automaton"; }
		static constexpr char* kDFAType(){ return "DFA"; }
		static constexpr char* kNFAType(){ return "NFA"; }
		static constexpr char* kEpsilonNFAType(){ return "ENFA"; }

		static std::set<Automaton*>& GetActiveAutomata() { return active_automata_; }
		// Retrieves a pointer to the automaton with specified id or nullptr if it is not active
		static const Automaton* GetAutomatonByIdentifier(uint32_t id);

		Automaton() {  }
		// TODO - Test this
		Automaton(const Automaton& other) : id_(Automaton::CreateIdentifier()), number_of_states_(other.number_of_states_),
											alphabet_(other.alphabet_), start_state_(other.start_state_), 
											accepting_states_(other.accepting_states_)  { }
		Automaton(uint32_t&& number_of_states, Alphabet&& alphabet, 
				  State&& start_state, std::set<State>&& accepting_states) : id_(CreateIdentifier()), number_of_states_(number_of_states),
																			alphabet_(alphabet), start_state_(start_state), accepting_states_(accepting_states) { }
		virtual ~Automaton() { }
		// Returns a std::set containing the identifiers of all currently availiable Automata
		static const std::set<int> ListActiveAutomataIndentifiers();
		// Prints the contents of ListActiveAutomataIndentifiers() to stdout
		static void PrintActiveAutomataIdentifiers();
		
		// Reads information for an Automaton from the file located at path 
		virtual bool ReadFromFile(const std::string& path) = 0;
		// Prints all transitions of the Automaton to target std::ostream
		virtual void PrintTransitions(std::ostream& output_stream) const = 0;
		// Exports the Automaton to a .at file at location path
		virtual void Export(const std::string& path) const = 0;

		// Returns true if word is in the automaton's language and false otherwise
		virtual bool Recognize(std::string& word) const = 0;
		// Answers questions about the properties of the language the Automaton describes
		virtual bool IsLanguageEmpty() const = 0;
		virtual bool IsLanguageInfinite() const = 0;

		uint32_t GetNumberOfStates() const { return number_of_states_; }
		const Identifier& GetIdentifier() const { return id_; }
		uint32_t Size() const { return number_of_states_; }
		const Alphabet& GetAlphabet() const { return alphabet_; }
		const std::set<char>& GetAlphabetCharacters() const { return alphabet_.GetCharacters(); }
		State GetStartState() const { return start_state_; }
		const std::set<State>& GetAcceptingStates() const { return accepting_states_; }
		bool IsAccepting(State state) const { return (accepting_states_.find(state) != accepting_states_.end()); }
		// TODO - Decide if necessary
		friend void swap(Automaton& a, Automaton& b) noexcept;

	protected:
		// Returns an identifier and increments last_assigned_id_
		// Should be used only when constructing a new Automaton
		static Identifier CreateIdentifier();
		bool ReadAlphabet(std::string& source){ return alphabet_.ReadAlphabet(source); }
		void SetNumberOfStates(uint32_t number){ number_of_states_ = number; }
		void SetStartState(State state) { start_state_ = state; }
		void SetAcceptingStates(std::set<State> accepting){ accepting_states_ = std::move(accepting); }
		void ReportAutomatonWasCreated(){ std::cout << "Automaton with ID: " << id_.GetValue() << " was created!" << std::endl; }

	private:
		static std::set<Automaton*> active_automata_;
		// ID number, which will be assigned to next created Automaton
		static uint32_t last_assigned_id_;
		Identifier id_;
		uint32_t number_of_states_;
		Alphabet alphabet_;
		State start_state_;
		std::set<State> accepting_states_;
	};

	// Reads data, which is common for all automata (reads first 5 lines of file)
	void ReadAutomatonData(std::ifstream& input_file, uint32_t& number_of_states, Alphabet& alphabet, State& start_state, std::set<State>& accepting_states);
}




#endif // SLARX_AUTOMATON_H_INCLUDED
