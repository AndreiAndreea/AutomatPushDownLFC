#include <iostream>
#include<unordered_set>

#include "Grammar.h"
#include "PushDownAutomaton.h"

PushDownAutomaton GeneratePushDownAutomaton(Grammar grammarIDC)
{
	//transformam in FNG
	grammarIDC.GetGreibachNormalForm();
	//construim functia de tranzitie
	Transitions transitions;
	std::vector<Production> productions = grammarIDC.GetProductions();

	for (auto& prod : productions)
	{
		std::string left = prod.GetLeftMember();
		std::string right = prod.GetRightMember();

		if (right.substr(1, right.size() - 1) != "")
			transitions.InsertTransition("q", right.substr(0, 1), left, "q", right.substr(1, right.size() - 1));
		else
			transitions.InsertTransition("q", right.substr(0, 1), left, "q", lambda); //folosim "-" pt lambda
	}

	//construim automat cu acceptare prin stiva vida
	// => F - multimea starilor finale este multimea vida
	return PushDownAutomaton("q", grammarIDC.GetTerminalSymbols(), grammarIDC.GetNonTerminalSymbols(), transitions, "q", grammarIDC.GetStartSymbol(), "-");
}

void printMenu() {
	std::cout << "---MENU---\n";
	std::cout << "0. Exit\n";
	std::cout << "1. Print Grammar.\n";
	std::cout << "2. Generate N words in the grammar.\n";
	std::cout << "3. Print the simplified grammar.\n";
	std::cout << "4. Print the grammar in FNG.\n";
	std::cout << "5. Generate word in grammar and check if the automaton accepts it.\n";
	std::cout << "6. Check if a word is accepted by the automaton.\n";
}

int main()
{
	//citire gramatica din fisier
	Grammar g;
	g.ReadGrammar();
	
	//verificare daca este valida si IDC
	if (g.VerifyGrammar() && g.IsContextFree()) 
	{
		//generare automat pushdown in gramatica
		PushDownAutomaton a = GeneratePushDownAutomaton(g);
		a.PrintAutomaton();
		
		int option;
		const uint8_t exitOptionNo = 0;
		const uint8_t printGrammarOptionNo = 1;
		const uint8_t generateNWordsOptionNo = 2;
		const uint8_t generateSimplifiedGrammarOptionNo = 3;
		const uint8_t generateFNGForGrammarOptionNo = 4;
		const uint8_t generateWordInGrammarAndCheckInAutomatonOptionNo = 5;
		const uint8_t checkSTDINWordInAutomatonOptionNo = 6;

		//menu implementation
		int numWords;
		std::string word;
		std::string input;
		bool isInteger;
		int N;
		std::unordered_set<std::string> ::iterator itr;
		std::unordered_set<std::string> words;
		std::string generatedWord;
		do {
			printMenu();
			std::cout << "Please enter an option: ";
			std::cin >> option;
			switch (option)
			{
			case printGrammarOptionNo:
				g.PrintGrammar();
				break;
			case generateNWordsOptionNo:
				std::cout << "Introduceti numarul de cuvinte distincte care doriti sa fie generate: ";
				std::cin >> input;
				words.clear();
				isInteger = true;
				for (int i = 0; isInteger && i < input.size(); i++)
					if (!isdigit(input[i]))
						isInteger = false;
				while (!isInteger)
				{
					std::cout << "Eroare! Input invalid!" << std::endl;
					std::cout << "Introduceti un numar intreg! : ";
					std::cin >> input;
					isInteger = true;
					for (int i = 0; isInteger && i < input.size(); i++)
						if (!isdigit(input[i]))
							isInteger = false;
				}
				N = stoi(input);
				while (words.size() != N)
				{
					words.insert(g.GenerateWord());
				}
				std::cout << "Cuvintele distincte generate sunt: " << std::endl;
				for (itr = words.begin(); itr != words.end(); itr++)
					std::cout << (*itr) << std::endl;
				break;
			case generateSimplifiedGrammarOptionNo:
				g.SimplifyGrammar();
				g.PrintGrammar();
				break;
			case generateFNGForGrammarOptionNo:
				g.GetGreibachNormalForm();
				g.PrintGrammar();
				break;
			case generateWordInGrammarAndCheckInAutomatonOptionNo:
				generatedWord = g.GenerateWord();
				std::cout << "Word checked by automaton: " << generatedWord << "\n";
				a.CheckWord(generatedWord);
				break;
			case checkSTDINWordInAutomatonOptionNo:
				std::cout << "Please input the word to check: ";
				std::cin >> word;
				a.CheckWord(word);
				break;
			case 0:
				break;
			default:
				std::cerr << "Invalid option. Try again." << std::endl;
				break;
			}
		} while (option != exitOptionNo);
	}
	else {
		std::cout << "Grammar is not valid or context free.\n";
	}
	
	return 0;
}
