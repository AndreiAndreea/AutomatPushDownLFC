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
	
	std::string GetNonTerminalSymbols();
	std::string GetTerminalSymbols();
	std::string GetStartSymbol();
	std::vector<Production> GetProductions();

private:
	std::string m_VN;
	std::string m_VT;
	std::string m_S;
	std::vector<Production> m_P;

	//forma regulilor productiilor m_P pt gram.IDC este: A -> w
	// A - este un singur neterminal (apartine VN)
	// w - este un cuvant format din simboluri din VN si VT, oricate combinatii
	//IN PLUS daca S -> lambda => S nu mai exista in membrul stang al altei productii
	
	//forma regulilor productiilor m_P pt gram.IDC in FNG este: A -> aALFA
	// A - este un singur neterminal (apartine VN)
	// a - este un singur terminal (apartine VT)
	// ALFA - grup de neterminale (oricate, inclusiv niciunul = lambda)
};
