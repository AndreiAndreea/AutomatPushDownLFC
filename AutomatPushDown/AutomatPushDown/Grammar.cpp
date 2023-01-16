#include "Grammar.h"
#include <cstdlib>
#include <utility>

void Grammar::ReadGrammar()
{
	std::ifstream components;
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

bool Grammar::IsContextFree()
{
	for (Production& production : m_P) {
		//check if the left has size of 1
		if (production.GetLeftMember().size() != 1) {
			return false;
		}

		//check if the right member is lambda
		if (production.GetRightMember() == lambda) {
			return false;
		}

		//checking for valid grammar is done in VerifyGrammar 
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
	bool foundAplicableRule = false;
	int randRuleIndex, randSymbolIndex;
	std::string currentWord;
	std::pair<std::string, int> randSymbol;
	std::string newWord, replaceableSymbol;

	std::vector<std::string> rightMember, aplicableRules;
	std::vector < std::pair<std::string, int>> replaceableSymbols;

	//adunam intr-un vector toti membrii drepti ai regulilor care au membrul stang simbolul de start
	for (auto& production : m_P)
		if (production.GetLeftMember() == "S")
			rightMember.push_back(production.GetRightMember());

	//alegem random o regula aplicabila pentru a genera un cuvant pornind de la simbolul de start
	srand(time(NULL));
	randRuleIndex = rand() % rightMember.size();
	currentWord = rightMember[randRuleIndex];

	while (!IsFinalWord(currentWord))
	{
		newWord = "";
		replaceableSymbols.clear();
		aplicableRules.clear();

		//alegem random o pozitie inlocuibila din cuvant si aplicam random o regula potrivita
		//1. retinem simbolurile inlocuibile din cuvant si pozitia lor in cuvant
		for (int pos = 0; pos < currentWord.size(); pos++)
		{
			if (m_VN.find(currentWord[pos]) != std::string::npos)
			{
				replaceableSymbol = currentWord[pos];
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
		for (int pos = 0; pos < currentWord.size(); pos++)
		{
			if (pos != replaceableSymbols[randSymbolIndex].second)
			{
				newWord += currentWord[pos];
			}
			else
			{
				newWord += aplicableRules[randRuleIndex];
			}
		}
		currentWord.assign(newWord);
	}
	return currentWord;
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
	PrintGrammar();
	RemoveInaccessibleSymbols();
	PrintGrammar();
	RemoveRenames();
}
void Grammar::GetChomskyNormalForm()
{
	SimplifyGrammar();
	PrintGrammar();
	const int minNewNonTerminalSymbol = GetLastNonTerminal() + 1;
	//max New Non Terminal Symbol is 82('R') because S is present in every grammar
	const int maxNewNonTerminalSymbol = 82;
	char newNonTerminalSymbol = minNewNonTerminalSymbol; //these are the intermediate symbols (between 33 and 64) - common symbols (see https://en.cppreference.com/w/cpp/language/ascii)
	//for every A->B1B2...Bn production replace all Bi from VT with a new nonterminal Ai, add the production Ai->Bi and replace occurence in the original production
	//check before adding the production if another one exists

	for (int i = 0; i < m_P.size(); i++) {
		Production production = m_P[i];
		if (production.GetLeftMember()[0] >= minNewNonTerminalSymbol && production.GetLeftMember()[0] <= maxNewNonTerminalSymbol) {
			continue;
		}

		if (production.GetRightMember().size() >= 2) {
			std::string newRightMember = production.GetRightMember();
			for (char& symbol : newRightMember) {
				//if it is terminal, then we have to replace it with a new nonterminal
				if (m_VT.find(symbol) != std::string::npos) {
					//create the new production
					std::string tmpLeftMember(1, newNonTerminalSymbol);
					std::string tmpRightMember(1, symbol);
					Production prod = Production(tmpLeftMember, tmpRightMember);

					//check if the production already exists
					bool exists = false;
					for (auto p : m_P) {
						//consider that it already exists only if it is a production we made
						if (p.GetRightMember() == prod.GetRightMember() && p.GetLeftMember()[0] >= minNewNonTerminalSymbol && p.GetLeftMember()[0] <= maxNewNonTerminalSymbol) {
							exists = true;
							prod = p;
							break;
						}
					}

					//replace symbol in original production
					symbol = prod.GetLeftMember()[0];

					//if the production doesn't exist, add it and update the counter
					if (!exists) {
						m_P.push_back(prod);
						m_VN.append(prod.GetLeftMember());
						newNonTerminalSymbol++;
					}
				}
			}
			m_P[i].SetRightMember(newRightMember);
		}
	}

	//for every A->B1B2...Bn production with n>2 replace the production with n productions of the form A->B1A' and A'->B2A'' and A''->B3...Bn
	for (int i = 0; i < m_P.size(); i++) {
		Production& production = m_P[i];
		//they are the ones we made - no need to check them
		if (production.GetLeftMember()[0] >= minNewNonTerminalSymbol && production.GetLeftMember()[0] <= maxNewNonTerminalSymbol) {
			continue;
		}
		if (production.GetRightMember().size() > 2) {
			//check if all members are nonterminals
			bool allNonTerminals = true;
			for (char& symbol : production.GetRightMember()) {
				if (m_VT.find(symbol) != std::string::npos) {
					allNonTerminals = false;
					break;
				}
			}

			if (allNonTerminals) {
				//create the new productions
				std::string newRightMember = production.GetRightMember();
				while (newRightMember.size() != 2) {
					int lastSymbolIndex = newRightMember.size() - 1;
					std::string tmpLeftMember(1, newNonTerminalSymbol);
					std::string tmpRightMember;
					tmpRightMember.append(1, newRightMember[lastSymbolIndex - 1]);
					tmpRightMember.append(1, newRightMember[lastSymbolIndex]);
					Production prod = Production(tmpLeftMember, tmpRightMember);

					//check if the production already exists
					bool exists = false;
					for (auto p : m_P) {
						//consider that it already exists only if it is a production we made
						if (p.GetRightMember() == prod.GetRightMember() && p.GetLeftMember()[0] >= minNewNonTerminalSymbol && p.GetLeftMember()[0] <= maxNewNonTerminalSymbol) {
							exists = true;
							prod = p;
							break;
						}
					}

					//replace the two non-terminal symbols with the left member of the production we found/created
					newRightMember.erase(lastSymbolIndex);
					newRightMember[lastSymbolIndex - 1] = prod.GetLeftMember()[0];

					//if prod does not exist then update the counter and the productions list
					if (!exists) {
						m_P.push_back(prod);
						m_VN.append(prod.GetLeftMember());
						newNonTerminalSymbol++;
					}
				}
				m_P[i].SetRightMember(newRightMember);
			}
		}
	}
}

//Verifica daca productia are forma C->A.. (indicele lui C > indicele lui A)
bool Grammar::CheckLema1(Production prod, std::unordered_map<char, int> symbolToIndex)
{
	if (symbolToIndex[prod.GetLeftMember()[0]] > symbolToIndex[prod.GetRightMember()[0]])
		return true;
	return false;
}

bool Grammar::CheckLema2(Production prod)
{
	if (prod.GetLeftMember()[0] == prod.GetRightMember()[0])
		return true;
	return false;
}

//Se elimina toate productiile de tip Ai -> Ak, unde i < k
void Grammar::ApplyLema1(Production prod, int symbolToApplyToIndex)  //mereu se aplica la al doilea simbol din membrul stang?? idk
{
	std::string rightMember = prod.GetRightMember();

	std::vector<std::string> allApplicableRightMembers;
	for (int i = 0; i < m_P.size(); i++)
	{
		if (m_P[i].GetLeftMember()[0] == rightMember[symbolToApplyToIndex])
		{
			allApplicableRightMembers.push_back(m_P[i].GetRightMember());
		}
	}

	for (int i = 0; i < allApplicableRightMembers.size(); i++)
	{
		//construct the new right member -> all symbols untill index + replace index with the applicable right member + all symbols after index
		std::string newRightMember = 
			prod.GetRightMember().substr(0, symbolToApplyToIndex) + 
			allApplicableRightMembers[i] + 
			prod.GetRightMember().substr(symbolToApplyToIndex + 1, prod.GetRightMember().size() - symbolToApplyToIndex - 1);
		//todo?verif daca exista deja
		Production newProd = Production(prod.GetLeftMember(), newRightMember);
		m_P.push_back(newProd);
	}
}

//Se elimina recursivitatea la stanga
void Grammar::ApplyLema2(Production prod, std::unordered_map<char, int>& symbolToIndex)
{
	static int nextNonTerminalIndex = 1;
	//noile simboluri intre T->Z
	char newNonTerminalSymbol = 'S' + nextNonTerminalIndex++;
	std::vector<Production> allApplicableProductions;
	char terminalSymbolToReplaceWith;
	for (int i = 0; i < m_P.size(); i++)
	{
		if (m_P[i].GetLeftMember() == prod.GetLeftMember()) {
			if (CheckLema2(m_P[i])) {
				allApplicableProductions.push_back(m_P[i]);
				m_P.erase(m_P.begin() + i);
				i--;
			}
			else if (m_P[i].GetRightMember().size() == 1 //?? nush daca e bn cu size 1
				&& std::find(m_VT.begin(), m_VT.end(), m_P[i].GetRightMember()[0]) != m_VT.end())
			{
				terminalSymbolToReplaceWith = m_P[i].GetRightMember()[0];
			}
		}
	}

	//aplica tranformarile pt toate A-productiile gasite
	for (auto p : allApplicableProductions)
	{
		//genereaza noile productii
		Production newProd1 = Production(std::string(1, newNonTerminalSymbol), p.GetRightMember().substr(1, prod.GetRightMember().size() - 1));
		Production newProd2 = Production(std::string(1, newNonTerminalSymbol), newProd1.GetRightMember() + std::string(1, newNonTerminalSymbol));

		//adauga noile productii
		m_P.push_back(newProd1);
		m_P.push_back(newProd2);
	}
	//adauga productia de forma A->aZ
	std::string newRightMember = std::string(1, terminalSymbolToReplaceWith) + std::string(1, newNonTerminalSymbol);
	
	Production newProd = Production(prod.GetLeftMember(), newRightMember);
	m_P.push_back(newProd);
	//updateaza alfabetul si indexii
	m_VN.append(std::string(1, newNonTerminalSymbol));
	symbolToIndex.emplace(newNonTerminalSymbol, m_VN.size());

}

void Grammar::GetGreibachNormalForm()
{
	GetChomskyNormalForm();
	const int maxNewNonTerminalSymbol = 'Z';
	char newNonTerminalSymbol = GetLastNonTerminal();

	//map the symbols to their index values
	std::unordered_map<char, int> symbolToIndex;
	symbolToIndex[m_S[0]] = 0;

	for (int i = 0; i < m_VN.size(); i++) {
		symbolToIndex[m_VN[i]] = i + 1;
	}

	//step 1 -> check all productions for lema 1 & lema 2 & apply them accordingly
	for (int i = 0; i < m_P.size(); i++)
	{
		Production prod = m_P[i];
		if (std::find(m_VT.begin(), m_VT.end(), prod.GetRightMember()[0]) == m_VT.end()) {

			if (CheckLema1(prod, symbolToIndex))
			{
				//this adds the new productions
				ApplyLema1(prod, 0);
				//now delete the old production
				m_P.erase(m_P.begin() + i--);
			}
			else if (CheckLema2(prod))
			{
				//this automatically erases the old production so we only need to decrement i
				ApplyLema2(prod, symbolToIndex);
				i--;
			}
		}
	}
	//step 2 -> update all S-productions
	for (int i = 0; i < m_P.size(); i++)
	{
		Production prod = m_P[i];
		if (prod.GetLeftMember() == m_S 
			&& std::find(m_VT.begin(), m_VT.end(), prod.GetRightMember()[0]) == m_VT.end())
		{
			ApplyLema1(prod, 0);
			m_P.erase(m_P.begin() + i--);
		}
	}
	//step 3 -> update all new productions -> this is done automatically during step 1

}
void Grammar::RemoveUnusableSymbols()
{
	//1. determinam daca un limbaj este vid
	//SKIP

	//2.simplificare simboluri care nu genereaza cuvinte din VT*
	std::unordered_set<std::string>vii;
	int lastSize;
	std::set<std::string> v;
	do {
		lastSize = vii.size();
		for (auto& production : m_P)
		{
			bool ok = true;
			for (auto& p : production.GetRightMember()) {
				if (m_VT.find(p) == std::string::npos && vii.find(std::string(1,p)) == vii.end())
				{
					ok = false;
				}
			}
			if(ok)
			{
				v.insert(production.GetLeftMember());
			}
		}
		for (auto& index : v)
		{
			vii.insert(index);
		}
		v.clear();
	} while (vii.size() != lastSize);
	m_VN.clear();
	for (auto& index : vii)
	{
		m_VN.append(index);
	}

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

void Grammar::RemoveInaccessibleSymbols()
{
	//verifica daca exista simboluri inaccesibile, ca in curs 8 Simplificarea gramaticilor, pg 27
	std::unordered_set<std::string> vii;
	vii.insert( m_S);
	int lastSize;
	std::set<std::string> v;
	do {
		lastSize = vii.size();
		for (auto& production : m_P)
		{
			if (vii.find(production.GetLeftMember()) != vii.end())
			{
				for (int i = 0; i < production.GetRightMember().size(); i++)
				{
					if (m_VN.find(production.GetRightMember()[i]) != std::string::npos)
					{
						std::string s(1,production.GetRightMember()[i]);
						v.insert(s);
					}
				}
			}
		}
		for (auto& index : v)
		{
			vii.insert(index);
		}
		v.clear();
	} while (lastSize!=vii.size());
	m_VN.clear();
	for (auto& index : vii)
	{
		m_VN.append(index);
	}

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

char Grammar::GetLastNonTerminal()
{
	//skip S - always present
	std::string tmpVN = m_VN;
	tmpVN.erase(std::find(tmpVN.begin(), tmpVN.end(), m_S[0]));
	return *std::max_element(tmpVN.begin(), tmpVN.end());
}

bool Grammar::isTerminal(std::string symbol)
{
	if (m_VT.find(symbol) != std::string::npos)
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
