#pragma once
#include "Production.h"
//#include "FiniteAutomaton.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Grammar
{
public:
	//FiniteAutomaton createFiniteAutomaton(Grammar grammar);

	void ReadGrammar();
	bool VerifyGrammar();
	bool IsRegular();
	bool IsFinalWord(const std::string& word);
	std::string GenerateWord();
	void PrintGrammar();

	/*std::string GetNonTerminalSymbols();
	std::string GetTerminalSymbols();
	std::string GetStartSymbol();*/

private:
	std::string m_VN;
	std::string m_VT;
	std::string m_S;
	std::vector<Production> m_P;
};


