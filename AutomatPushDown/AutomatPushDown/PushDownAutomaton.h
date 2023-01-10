#pragma once
#include "Grammar.h"
#include "Transitions.h"
#include<tuple>

class PushDownAutomaton
{
public:
	PushDownAutomaton(std::string states, std::string sigmaAlphabet, std::string gamaAlphabet, Transitions transitions, std::string initialState, std::string startSymbolPushDownMemory, std::string finalState);
	
	void PrintAutomaton();
	
	bool CheckWord(std::string word); //trebuie modificata pentru acceptare prin stiva goala

	bool IsDeterministic();
	
private:
	std::string m_states; //Q - new
	
	std::string m_sigmaAlphabet; //sigma - alfabet finit al benzii de intrare = m_VT
	
	std::string m_gamaAlphabet; //gama - alfabet finit al memoriei push-down (STIVA) = m_VN
	
	Transitions m_transitions; //transition function (delta) - input:(stare, simbol sigma, simbol gama)
	
	std::string m_initialState; //q0 - apartine Q
	
	std::string m_startSymbolPushDownMemory; //Z0 = m_S - simbol de start al memoriei push-down (STIVA) - apartine gama
	
	std::string m_finalState; //F - multimea starilor finale = multimea vida pentru acceptare prin stiva vida
	
	bool blocaj;
};

