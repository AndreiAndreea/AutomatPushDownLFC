#pragma once
#include "Grammar.h"
#include "Transitions.h"

class PushDownAutomaton
{
public:
	PushDownAutomaton(std::string states, std::string sigmaAlphabet, std::string gamaAlphabet, std::string initialState, std::string Z0, std::string finalStates, Transitions transitions);
	
	void PrintAutomaton();
	
	bool IsDeterministic();
	
private:
	std::string m_states; //Q = m_VN + F
	std::string m_sigmaAlphabet; //sigma - alfabet finit al benzii de intrare = m_VT
	std::string m_gamaAlphabet; //gama - alfabet finit al memoriei push-down (STIVA) 
	std::string m_initialState; //q0 = m_S - apartine Q
	std::string m_Z0; //Z0 - simbol de start al memoriei push-down (STIVA) - apartine gama
	std::string m_finalStates; //F - multimea starilor finale - inclus in Q

	Transitions m_transitions; //transition function - input:(stare, simbol sigma, simbol gama)
};

