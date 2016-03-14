/*
 * DFA.h
 *
 *  Created on: May 16, 2015
 *      Author: Hristiyan Nikolov
 *  Copyright (c) 2015. All rights reserved.
 */

#ifndef DFA_H_
#define DFA_H_
#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>
class DFA {
private:
	char alphabet[37]; 						 // символите в азбуката на автомата
	int countOfAlphabet;					   // брой символи в азбуката
	std::map<int, std::map<char, int> > table; // таблица на прехода (table[състояние1][буква]=състояние2)
											   // От "състояние1" с символа от азбуката "буква" отиваме в "състояние2"
	int startState;							  // началното състояние на автомата
	int* finalStates;                          // финални състояния
	int countOfFinalStates;					   // броя на финалните състояния
	int* states;							   // всички състояния
	int countOfStates;						   // броя на всички състояния


	int transitionFunction(int from, char letter);
	int extendedTransitionFunction(int from, const char* word);
	bool isFinalState(int state);
	bool isValidState(int state);
	int getCountOfStates(); // връща броя на състоянията
	int* getStates();
	int maxNameOfState(); // връща най-големия номер от номерата на всички състояния
	void changeAllStatesName(int start); // променя имената на състоянията във автомата, като започва от start
										 // функцията променя и таблицата на автомата
public:
	DFA(const DFA&);
	DFA(int _startState, int* _finalStates, int _countOfFinalStates,
			std::map<int, std::map<char, int> > _table);
	~DFA();

	bool checkWord(char* word);
	void addErrorState();
	friend std::ostream& operator<<(std::ostream& os, DFA automat);
};

void unionOfTwoAutomata(DFA automat1, DFA automat2);

#endif /* DFA_H_ */
