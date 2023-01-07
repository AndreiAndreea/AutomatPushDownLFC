#include "PushDownAutomaton.h"

PushDownAutomaton::PushDownAutomaton(std::string states, std::string sigmaAlphabet, std::string gamaAlphabet, std::string initialState, std::string Z0, std::string finalStates, Transitions transitions)
    :m_states(states),
    m_sigmaAlphabet(sigmaAlphabet),
    m_gamaAlphabet(gamaAlphabet),
    m_initialState(initialState),
	m_Z0(Z0),
    m_finalStates(finalStates),
    m_transitions(Transitions(transitions.GetDeltaFunction(), transitions.GetUsedStates(), transitions.GetUsedSymbols(), transitions.GetUsedStackSymbols()))
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
    std::cout << "Simbol de start al memoriei push-down: " << m_Z0 << std::endl;
    std::cout << "Multimea starilor finale - F:{";
    for (int i = 0; i < m_finalStates.size(); i++)
    {
        if (i != (m_finalStates.size() - 1))
            std::cout << m_finalStates[i] << ", ";
        else
            std::cout << m_finalStates[i] << "}" << std::endl;
    }
    std::cout << "Functia de tranzitie:" << std::endl;
    m_transitions.PrintTransitions();
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
