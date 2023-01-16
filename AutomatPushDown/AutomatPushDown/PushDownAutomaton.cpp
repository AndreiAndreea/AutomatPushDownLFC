#include "PushDownAutomaton.h"
#include<stack>
#include<cstdlib>

PushDownAutomaton::PushDownAutomaton(std::string states, std::string sigmaAlphabet, std::string gamaAlphabet, Transitions transitions, std::string initialState, std::string startSymbolPushDownMemory, std::string finalState)
	:m_states(states),
	m_sigmaAlphabet(sigmaAlphabet),
	m_gamaAlphabet(gamaAlphabet),
	m_transitions(transitions),
	m_initialState(initialState),
	m_startSymbolPushDownMemory(startSymbolPushDownMemory),
	m_finalState(finalState)
{
}

void PushDownAutomaton::PrintAutomaton()
{
	std::cout << "Elementele automatului sunt: " << std::endl;
	std::cout << "Stari - Q:{";
	for (int i = 0; i < m_states.size(); i++)
	{
		if (i != (m_states.size() - 1))
			std::cout << m_states[i] << ", ";
		else
			std::cout << m_states[i] << "}" << std::endl;
	}
	std::cout << "Alfabet finit al benzii de intrare - Sigma:{";
	for (int i = 0; i < m_sigmaAlphabet.size(); i++)
	{
		if (i != (m_sigmaAlphabet.size() - 1))
			std::cout << m_sigmaAlphabet[i] << ", ";
		else
			std::cout << m_sigmaAlphabet[i] << "}" << std::endl;
	}
	std::cout << "Alfabet finit al memoriei push-down - Gama:{";
	for (int i = 0; i < m_gamaAlphabet.size(); i++)
	{
		if (i != (m_gamaAlphabet.size() - 1))
			std::cout << m_gamaAlphabet[i] << ", ";
		else
			std::cout << m_gamaAlphabet[i] << "}" << std::endl;
	}
	std::cout << "Stare initiala: " << m_initialState << std::endl;
	std::cout << "Simbol de start al memoriei push-down: " << m_startSymbolPushDownMemory << std::endl;
	std::cout << "Multimea starilor finale - F:{";
	for (int i = 0; i < m_finalState.size(); i++)
	{
		if (i != (m_finalState.size() - 1))
			std::cout << m_finalState[i] << ", ";
		else
			std::cout << m_finalState[i] << "}" << std::endl;
	}
	std::cout << "Functia de tranzitie:" << std::endl;
	m_transitions.PrintTransitions();
}

bool PushDownAutomaton::CheckWord(std::string word)
{
	std::stack<std::string> pushDownMemory;
	Unordered_map delta = m_transitions.GetDeltaFunction();
	std::string nextState = m_initialState;
	std::string nextNewStackInput = m_startSymbolPushDownMemory;
	
	pushDownMemory.push(m_startSymbolPushDownMemory);
	
	for (auto& c : word)
	{
		if (pushDownMemory.size() == 0)
		{
			std::cout << "Stiva e goala! Cuvantul NU este acceptat de automat!" << std::endl;
			return false;
		}
		
		std::string currentSymbol(1, c);
		
		auto it = delta.find(std::make_tuple(nextState, currentSymbol, pushDownMemory.top()));
		
		std::cout << "Stack top:" << pushDownMemory.top() << std::endl;
		std::cout << "(" << nextState << ", " << currentSymbol << ", " << pushDownMemory.top() << ") ";

		if (it != delta.end())
		{
			pushDownMemory.pop();

			srand(time(NULL));
			int randTransitionOutput = rand() % it->second.size();
			std::string nextState = it->second[randTransitionOutput].first;
			std::string nextNewStackInput = it->second[randTransitionOutput].second;

			std::cout <<"= ("<< nextState << ", " << nextNewStackInput << ")" << std::endl;

			if (nextNewStackInput != "-")
			{
				//in stiva inseram simbolurile noi (de la sfarsit la inceputul cuv. ca sa avem in top primul caracter)
				reverse(nextNewStackInput.begin(), nextNewStackInput.end());
				for (auto& newStackInput : nextNewStackInput)
					pushDownMemory.push(std::string(1, newStackInput));
				
			}
		}
		else
		{
			std::cout << "Nu exista aceasta tranzitie! Cuvantul nu este acceptat de automat!" << std::endl;
			return false;
		}

	}
	if (pushDownMemory.size() == 0)
	{
		std::cout << "Cuvantul este acceptat de automat!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Cuvantul NU este acceptat de automat ! " << std::endl;
		return false;
	}		
}

bool PushDownAutomaton::IsDeterministic()
{
	Unordered_map transitions = m_transitions.GetDeltaFunction();
	for (const auto& config : transitions)
		if (config.second.size() > 1) //trebuie sa avem o singura tranzitie pt fiecare configuratie
		{
			return false;
		}
		else if (std::get<1>(config.first) == "-") //folosim "-" pt lambda
		{
			//nu putem face din acelasi punct tranzitie cu citire si fara citire
			if (m_transitions.CheckIfInputWithoutLambdaExists(std::get<0>(config.first), std::get<2>(config.first)))
				return false;
		}
	return true;
}
