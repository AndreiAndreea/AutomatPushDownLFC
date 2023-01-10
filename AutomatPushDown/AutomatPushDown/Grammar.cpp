#include "Grammar.h"
#include <cstdlib>
#include <utility>

/*
FiniteAutomaton Grammar::createFiniteAutomaton(Grammar grammar)
{
	std::string states = m_VN;
	std::string alphabet = m_VT;
	std::string initialState = m_S;
	std::string finalStates;
	Transitions transitionFunction;
	std::ifstream transitions;
	transitions.open("transitions.txt");
	if (transitions.is_open())
	{
		transitions >> finalStates;
		//adaugam in Q (multimea starilor) multimea starilor finale
		for (int i = 0; i < finalStates.size(); i++)
			if (states.find(finalStates[i]) == std::string::npos)
				states += finalStates[i];

		std::string transition;
		while (std::getline(transitions, transition))
		{
			bool foundOpenBracket = false;
			bool foundCloseBracket = false;
			bool foundComma = false;
			bool foundEqualSign = false;
			std::string transitionState, transitionSymbol, transitionResultStates;
			for (auto c : transition)
			{
				if (c == '(')
				{
					foundOpenBracket = true;
				}
				else if (c == ')')
				{
					foundCloseBracket = true;
				}
				else if (c == ',')
				{
					foundComma = true;
				}
				else if (c == '=')
				{
					foundEqualSign = true;
				}
				else if (foundOpenBracket && !foundComma)
				{
					transitionState = c;
				}
				else if (c != ' ' && foundOpenBracket && foundComma && !foundCloseBracket)
				{
					transitionSymbol = c;
				}
				else if (c != ' ' && foundOpenBracket && foundComma && foundCloseBracket && foundEqualSign)
				{
					transitionResultStates += c;
				}
			}
			transitionFunction.InsertTransition(transitionState, transitionSymbol, transitionResultStates);
		}
	}
	transitions.close();
	FiniteAutomaton finiteAutomaton(states, alphabet, initialState, finalStates, transitionFunction);
	return finiteAutomaton;
}
*/

void Grammar::ReadGrammar()
{
	std::ifstream components;
	//components.open("C:\\Users\\Chindea Cosmin\\Documents\\Anul II\\LFC\\AutomatPushDown\\AutomatPushDown\\AutomatPushDown\\components.txt");
	components.open("components.txt");
	if (components.is_open())
	{
		components >> m_VN;
		components >> m_VT;
		components >> m_S;
		std::string production;
		while (components >> production)
		{
			bool foundArrow = false;
			std::string leftMember, rightMember;
			for (auto& c : production)
				if (!foundArrow && c != '-' && c != '>')
				{
					leftMember += c;
				}
				else if (c == '-' && !foundArrow)
				{
					foundArrow = true;
				}
				else if (foundArrow && c != '>')
				{
					rightMember += c;
				}
			m_P.push_back(Production(leftMember, rightMember));
		}
	}
	components.close();
}

bool Grammar::VerifyGrammar()
{
	bool foundS = false, foundNonterminal = false, leftMemberIsS = false;

	//1. intersectia VN si VT trebuie sa fie vida
	for (auto& vn : m_VN)
		for (auto& vt : m_VT)
			if (vn == vt)
				return false;

	//2. S trebuie sa apartina multimii VN
	for (auto& vn : m_VN)
		if (vn == m_S[0]) {
			foundS = true;
			break;
		}
	if (!foundS)
		return false;

	//3. pentru fiecare regula, membrul stang contine cel putin un neterminal 
	//4. exista cel puin o productie care are in stranga doar S
	for (auto& production : m_P)
	{
		if (production.GetLeftMember() != "S")
			leftMemberIsS = true;

		foundNonterminal = false;
		for (auto& neterminal : m_VN)
			if (production.GetLeftMember().find(neterminal) != std::string::npos) {
				foundNonterminal = true;
				break;
			}
		if (!foundNonterminal)
			return false;
	}
	if (!leftMemberIsS) //4
		return false;

	//5. fiecare productie contine doar elemente din VN si VT
	for (auto& production : m_P)
	{
		for (auto& c : production.GetLeftMember())
			if (m_VN.find(c) == std::string::npos && m_VT.find(c) == std::string::npos)
				return false;
		for (auto& c : production.GetRightMember())
			if (m_VN.find(c) == std::string::npos && m_VT.find(c) == std::string::npos)
				return false;
	}
	return true;
}

bool Grammar::IsRegular()
{
	//forma productiilor: A->aA | a
	for (auto& production : m_P)
	{
		//verificam daca membrul stang este format dintr-un singur simbol
		if (production.GetLeftMember().size() > 1)
			return false;
		//verificam daca membrul stang este un alt fel de simbol decat neterminal
		if (m_VN.find(production.GetLeftMember()[0]) == std::string::npos)
			return false;
		//verificam ca membrul drept sa fie format din doua simboluri -> terminalNeterminal 
		//SAU dintr-un singur simbol terminal
		if (production.GetRightMember().size() == 2)
		{
			if (m_VT.find(production.GetRightMember()[0]) == std::string::npos ||
				m_VN.find(production.GetRightMember()[1]) == std::string::npos)
				return false;
		}
		else if (production.GetRightMember().size() == 1)
		{
			if (m_VT.find(production.GetRightMember()[0]) == std::string::npos)
				return false;
		}
	}
	return true;
}

bool Grammar::IsContextFree()
{
	for (Production& production : m_P) {
		//check if the left member is from VN
		if (m_VN.find(production.GetLeftMember()) == std::string::npos) {
			return false;
		}

		//check if the right member is empty
		if (production.GetRightMember().empty()) {
			return false;
		}
		//checking if the right member is from VN or VT is done in VerifyGrammar 
	}
	return true;
}

bool Grammar::IsFinalWord(const std::string& word)
{
	//verifica daca un cuvant este format doar din simboluri terminale
	for (auto& symbol : word)
		if (m_VN.find(symbol) != std::string::npos)
			return false;
	return true;
}

std::string Grammar::GenerateWord()
{
	std::cout << "Acestea sunt etapele de generare ale cuvantului:" << std::endl;
	std::cout << "S => ";
	bool foundAplicableRule = false;
	int randRuleIndex, randSymbolIndex;
	std::pair<std::string, int> randSymbol;
	std::string word, newWord, replaceableSymbol;
	std::vector<std::string> rightMember, aplicableRules;
	std::vector < std::pair<std::string, int>> replaceableSymbols;

	//adunam intr-un vector toti membrii drepti ai regulilor care au membrul stang simbolul de start
	for (auto& production : m_P)
		if (production.GetLeftMember() == "S")
			rightMember.push_back(production.GetRightMember());

	//alegem random o regula aplicabila pentru a genera un cuvant pornind de la simbolul de start
	srand(time(NULL));
	randRuleIndex = rand() % rightMember.size();
	word = rightMember[randRuleIndex];
	std::cout << word << std::endl;

	while (!IsFinalWord(word))
	{
		newWord = "";
		replaceableSymbols.clear();
		aplicableRules.clear();

		//alegem random o pozitie inlocuibila din cuvant si aplicam random o regula potrivita
		//1. retinem simbolurile inlocuibile din cuvant si pozitia lor in cuvant
		for (int pos = 0; pos < word.size(); pos++)
		{
			if (m_VN.find(word[pos]) != std::string::npos)
			{
				replaceableSymbol = word[pos];
				replaceableSymbols.push_back(std::make_pair(replaceableSymbol, pos));
			}
		}

		//2. alegem random un simbol inlocuibil din cuvant
		randSymbolIndex = rand() % replaceableSymbols.size() + 0;

		//3. gasim regulile aplicabile pentru membrul stang selectat random (simbolul inlocuibil)
		for (int pos = 0; pos < m_P.size(); pos++)
			if (m_P[pos].GetLeftMember() == replaceableSymbols[randSymbolIndex].first)
				aplicableRules.push_back(m_P[pos].GetRightMember());

		//4. alegem random o regula pe care sa o aplicam
		randRuleIndex = rand() % aplicableRules.size() + 0;

		//5. aplicam regula aleasa pt simbolul ales pe pozitia lui si generam un nou cuvant
		for (int pos = 0; pos < word.size(); pos++)
		{
			if (pos != replaceableSymbols[randSymbolIndex].second)
			{
				newWord += word[pos];
			}
			else
			{
				newWord += aplicableRules[randRuleIndex];
			}
		}
		std::cout << word << " => " << newWord << std::endl;
		word = newWord;
	}
	return word;
}

void Grammar::PrintGrammar()
{
	std::cout << "----------\n";
	std::cout << "Elementele gramaticii sunt: " << std::endl;
	std::cout << "Multimea neterminalelor - VN:{";
	for (int i = 0; i < m_VN.size(); i++)
	{
		if (i != (m_VN.size() - 1))
			std::cout << m_VN[i] << ", ";
		else
			std::cout << m_VN[i] << "}" << std::endl;
	}
	std::cout << "Multimea terminalelor - VT:{";
	for (int i = 0; i < m_VT.size(); i++)
	{
		if (i != (m_VT.size() - 1))
			std::cout << m_VT[i] << ", ";
		else
			std::cout << m_VT[i] << "}" << std::endl;
	}
	std::cout << "Simbolul de start: " << m_S << std::endl;
	std::cout << "Productiile - P:" << std::endl;
	for (int i = 0; i < m_P.size(); i++)
		std::cout << m_P[i].GetLeftMember() << "->" << m_P[i].GetRightMember() << std::endl;
	std::cout << "----------\n";
}
void Grammar::SimplifyGrammar()
{
	RemoveUnusableSymbols();
	RemoveInaccessibleSymbols();
	RemoveRenames();
}
void Grammar::RemoveUnusableSymbols()
{
	//1. determinam daca un limbaj este vid
	//SKIP

	//2.simplificare simboluri care nu genereaza cuvinte din VT*
	std::string v;
	std::string vi;
	do {
		vi = v;
		v.clear();
		for (auto& production : m_P)
		{
			if (m_VT.find(production.GetRightMember()) != std::string::npos || vi.find(production.GetRightMember()) != std::string::npos)
			{
				v.append(production.GetLeftMember());
			}
		}
	} while (vi.compare(v) != 0);
	m_VN = vi;

	std::vector<Production> P;
	for (auto& production : m_P)
	{
		if (m_VN.find(production.GetLeftMember()) == std::string::npos)
		{
			continue;
		}
		int ok = 0;
		for (int i = 0; i < production.GetRightMember().size(); i++)
		{
			if (m_VN.find(production.GetRightMember()[i]) == std::string::npos && m_VT.find(production.GetRightMember()[i])==std::string::npos)
			{
				ok = 1;
			}
		}
		if (ok == 0) {
			P.push_back(production);
		}
	}
	m_P = P;
}

void Grammar::RemoveInaccessibleSymbols()
{
	//verifica daca exista simboluri inaccesibile, ca in curs 8 Simplificarea gramaticilor, pg 27
	std::string v = m_S;
	std::string vi;
	do {
		vi.append(v);
		v.clear();
		for (auto& production : m_P)
		{
			if (vi.find(production.GetLeftMember()) != std::string::npos)
			{
				for (int i = 0; i < production.GetRightMember().size(); i++)
				{
					if (m_VN.find(production.GetRightMember()[i]) != std::string::npos && vi.find(production.GetRightMember()[i])==std::string::npos)
					{
						v.push_back(production.GetRightMember()[i]);
					}
				}
			}
		}
	}while (!v.empty());
	m_VN = vi;

	std::vector<Production> P;
	for (auto& production : m_P)
	{
		if (m_VN.find(production.GetLeftMember()) == std::string::npos)
		{
			continue;
		}
		int ok = 0;
		for (int i = 0; i < production.GetRightMember().size(); i++)
		{
			if (m_VN.find(production.GetRightMember()[i]) == std::string::npos && m_VT.find(production.GetRightMember()[i]) == std::string::npos)
			{
				ok = 1;
			}
		}
		if (ok == 0) {
			P.push_back(production);
		}
	}
	m_P = P;
}
void Grammar::RemoveRenames()
{
	//simplifica redenumirile, ca in curs 8 Simplificarea gramaticilor, pg 34
	std::vector<Production > p0;
	std::vector<Production>  renames;
	do {
		for (auto& prod : m_P)
		{
			//auto [left, right] = prod.GetProduction(); - eroare
			std::string left = prod.GetLeftMember();
			std::string right = prod.GetRightMember();
			
			if (left.length() == 1 && right.length() == 1)
			{
				bool leftOk = false;
				bool rightOk = false;
				for (char element : m_VN)
				{
					if (element == left[0])
					{
						leftOk = true;
					}
					if (element == right[0])
					{
						rightOk = true;
					}
				}
				if (leftOk && rightOk)
				{
					renames.push_back(prod);
				}
				else {
					p0.push_back(prod);
				}
			}
			else {
				p0.push_back(prod);
			}
		}
		for (Production prod : renames)
		{
			std::queue <Production> finals;
			finals.push(prod);
			while (!finals.empty())
			{
				if (!isNeterminal(finals.front().GetRightMember())) {
					p0.push_back(Production(prod.GetLeftMember(), finals.front().GetRightMember()));
				}
				else {
					for (Production p : m_P)
					{
						if (finals.front().GetRightMember() == p.GetLeftMember())
						{
							finals.push(p);
						}
					}
				}
				finals.pop();
			}
		}
		m_P = p0;
		renames.clear();
	} while (!renames.empty());
}

bool Grammar::isTerminal(std::string symbol)
{
	if (m_VT.find(symbol)!=std::string::npos)
	{
		return true;
	}
	return false;

}

bool Grammar::isNeterminal(std::string symbol)
{
	if (m_VN.find(symbol) != std::string::npos)
	{
		return true;
	}
	return false;
}

std::string Grammar::GetNonTerminalSymbols()
{
	return m_VN;
}

std::string Grammar::GetTerminalSymbols()
{
	return m_VT;
}

std::string Grammar::GetStartSymbol()
{
	return m_S;
}

std::vector<Production> Grammar::GetProductions()
{
	return m_P;
}
