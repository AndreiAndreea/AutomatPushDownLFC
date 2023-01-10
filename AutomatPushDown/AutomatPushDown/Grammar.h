#pragma once
#include "Production.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

class Grammar
{
public:
	//FiniteAutomaton createFiniteAutomaton(Grammar grammar);

	void ReadGrammar();
	bool VerifyGrammar();
	bool IsRegular(); //not used
	bool IsContextFree();
	bool IsFinalWord(const std::string& word);
	std::string GenerateWord();
	void PrintGrammar();
	void SimplifyGrammar();
	void RemoveUnusableSymbols();
	void RemoveInaccessibleSymbols();
	void RemoveRenames();


	bool isTerminal(std::string symbol);
	bool isNeterminal(std::string symbol);
	/*std::string GetNonTerminalSymbols();
	std::string GetTerminalSymbols();
	std::string GetStartSymbol();*/

private:
	std::string m_VN;
	std::string m_VT;
	std::string m_S;
	std::vector<Production> m_P;
};
