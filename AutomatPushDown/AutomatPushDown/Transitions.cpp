#include "Transitions.h"

Transitions::Transitions()
{
}

Transitions::Transitions(Unordered_map delta, std::string states, std::string symbols, std::string stackSymbols)
	:m_delta(delta), m_states(states), m_symbols(symbols), m_stackSymbols(stackSymbols)
{
}

void Transitions::InsertTransition(std::string transitionState, std::string transitionSymbol, std::string transitionStackSymbol, std::string transitionResultState, std::string transitionResultStackSymbols)
{
	//verificam daca exista in memorie simbolurile, daca nu - le adaugam
	if (!ExistsState(transitionState))
		m_states += transitionState;
	if (!ExistsSymbol(transitionSymbol))
		m_symbols += transitionSymbol;
	if (!ExistsStackSymbol(transitionStackSymbol))
		m_stackSymbols += transitionStackSymbol;
	
	if (!ExistsState(transitionResultState))
		m_states += transitionResultState;
	if (!ExistsStackSymbol(transitionResultStackSymbols))
		m_stackSymbols += transitionResultStackSymbols;
	
	//adaugam tranzitia in memorie, daca exista deja input-ul adaugam la rezultat ce e nou ( = nu e determinist)
	if (!ExistsTransition(transitionState, transitionSymbol, transitionStackSymbol))
	{
		std::tuple<std::string, std::string, std::string> input = std::make_tuple(transitionState, transitionSymbol, transitionStackSymbol);
		std::vector<std::pair<std::string, std::string>> output;
		output.push_back(std::make_pair(transitionResultState, transitionResultStackSymbols));
		
		m_delta.insert({ input, output });
	}
	else
	{
		auto it = m_delta.find(std::make_tuple(transitionState, transitionSymbol, transitionStackSymbol));
		//it - este iterator in map (un pair<key, value>)
		//it -> first: input (configuratia - tripletul (stare, simbol, ce am pe stiva))
		//it -> second: output (vector de pair - pt ca o configuratie poate avea mai multe tranzitii)

		//exista partea de input a tranzitiei - verificam daca output-ul e diferit si il adaugam
		bool existsOutputTransition = false;
		std::pair<std::string, std::string> transition = std::make_pair(transitionSymbol, transitionStackSymbol);
		
		for (auto& vecOutput : it->second) 
			if (vecOutput == transition)
				existsOutputTransition = true;
		
		if (!existsOutputTransition)
			it->second.push_back(transition);
	}
	
}

std::vector<std::pair<std::string, std::string>> Transitions::GetTransitionResultStates(std::string transitionState, std::string transitionSymbol, std::string transitionStackSymbol)
{
	auto output = m_delta.find(std::make_tuple(transitionState, transitionSymbol, transitionStackSymbol));
	if (output != m_delta.end())
		return output->second;

	std::vector<std::pair<std::string, std::string>> nullOutput;
	return nullOutput;
}

void Transitions::PrintTransitions()
{
	for (const auto& config : m_delta)
	{
		std::cout << "delta (" << std::get<0>(config.first) << ", " << std::get<1>(config.first) << ", " << std::get<2>(config.first) << ") = ";

		std::cout << "{ ";
		for (auto& output : config.second)
		{
			std::cout << "(" << output.first << ", " << output.second << ")";
		}
		std::cout << " }" << std::endl;
	}
}

bool Transitions::CheckIfInputWithoutLambdaExists(std::string state, std::string stackSymbol)
{
	for (const auto& config : m_delta)
	{
		if (std::get<0>(config.first) == state && std::get<2>(config.first) == stackSymbol)
			if (std::get<1>(config.first) != "-") //folosim "-" pt lambda
				return true;
	}
	return false;
}

Unordered_map Transitions::GetDeltaFunction()
{
	return m_delta;
}

std::string Transitions::GetUsedStates()
{
	return m_states;
}

std::string Transitions::GetUsedSymbols()
{
	return m_symbols;
}

std::string Transitions::GetUsedStackSymbols()
{
	return m_stackSymbols;
}

bool Transitions::ExistsTransition(std::string transitionState, std::string transitionSymbol, std::string transitionStackSymbol)
{
	if (m_delta.find(std::make_tuple(transitionState, transitionSymbol, transitionStackSymbol)) != m_delta.end())
		return true;
	return false;
}

bool Transitions::ExistsState(std::string state)
{
	for (const auto& s : m_states)
		if (s == state[0])
			return true;
	return false;
}

bool Transitions::ExistsSymbol(std::string symbol)
{
	for (const auto& s : m_symbols)
		if (s == symbol[0])
			return true;
	return false;
}

bool Transitions::ExistsStackSymbol(std::string stackSymbol)
{
	for (const auto& s : m_stackSymbols)
		if (s == stackSymbol[0])
			return true;
	return false;
}
