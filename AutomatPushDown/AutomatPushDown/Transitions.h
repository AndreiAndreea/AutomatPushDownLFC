#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <functional>
#include <tuple>

struct tuple_hash {
	template <class T1, class T2, class T3>
	std::size_t operator () (const std::tuple<T1, T2, T3>& tup) const {
		auto h1 = std::hash<T1>{}(std::get<0>(tup));
		auto h2 = std::hash<T2>{}(std::get<1>(tup));
		auto h3 = std::hash<T3>{}(std::get<2>(tup));
		return h1 ^ h2 ^ h3;
	}
};

using TransitionFunctionInputs = std::tuple<std::string, std::string, std::string>;
using Unordered_map = std::unordered_map<TransitionFunctionInputs, std::vector<std::pair<std::string, std::string>>, tuple_hash>;

class Transitions
{
public:
	Transitions();
	Transitions(Unordered_map delta, std::string states, std::string symbols, std::string stackSymbols);

	void InsertTransition(std::string transitionState, std::string transitionSymbol, std::string transitionStackSymbol, std::string transitionResultState, std::string transitionResultStackSymbols);
	std::vector<std::pair<std::string, std::string>> GetTransitionResultStates(std::string transitionState, std::string transitionSymbol, std::string transitionStackSymbol);
	
	void PrintTransitions();

	bool CheckIfInputWithoutLambdaExists(std::string state, std::string stackSymbol);

public:
	Unordered_map GetDeltaFunction();
	std::string GetUsedStates();
	std::string GetUsedSymbols();
	std::string GetUsedStackSymbols();

private:
	bool ExistsTransition(std::string transitionState, std::string transitionSymbol, std::string transitionStackSymbol);
	bool ExistsState(std::string state);
	bool ExistsSymbol(std::string symbol);
	bool ExistsStackSymbol(std::string stackSymbol);

private:
	Unordered_map m_delta;
	std::string m_states;
	std::string m_symbols; //simbol sigma
	std::string m_stackSymbols; //simbol gama - ce am pe stiva
};
