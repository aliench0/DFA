/*
 * DFA.cpp
 *
 *  Created on: May 16, 2015
 *      Author: Hristiyan Nikolov
 *  Copyright (c) 2015. All rights reserved.
 */
#include "DFA.h"

int DFA::transitionFunction(int from, char letter) {
	if (table.find(from) != table.end()
			&& table[from].find(letter) != table[from].end())
		return table[from][letter];
	return -2;
}

int DFA::extendedTransitionFunction(int from, const char* word) {
	for (int i = 0; i < (int) strlen(word); i++)
		from = transitionFunction(from, word[i]);
	return from;
}

bool DFA::isFinalState(int state) {
	if (isValidState(state)) {
		for (int i = 0; i < countOfFinalStates; i++)
			if (finalStates[i] == state)
				return true;
	}
	return false;
}

bool DFA::isValidState(int state) {
	for (int i = 0; i < countOfStates; i++)
		if (states[i] == state)
			return true;
	return false;
}

int DFA::getCountOfStates() {
	int count = 0, k = 0;
	int allStates[table.size() * 2]; // в него записваме абсолютно всички състояния от таблицата, дори да се повтарят
	bool isIn = false;

	for (std::map<int, std::map<char, int> >::const_iterator i = table.begin();
			i != table.end(); i++)
		for (std::map<char, int>::const_iterator j = i->second.begin();
				j != i->second.end(); j++) {
			allStates[k++] = i->first;
			allStates[k++] = j->second;
		}

	int newAllStates[k];
	for (int i = 0; i < k; i++)
		newAllStates[i] = -2;
	for (int i = 0; i < k; i++) { // в newAllStates записваме числата (т.е. състоянията), които не се повтарят и след това връщаме броя им
		isIn = false;
		for (int j = 0; j < k; j++)
			if (allStates[i] == newAllStates[j])
				isIn = true;
		if (isIn == false) {
			newAllStates[count++] = allStates[i];
		}
	}
	return count;
}

int* DFA::getStates() {
	int count = 0, k = 0;
	int allStates[table.size() * 2]; // в него записваме абсолютно всички състояния от таблицата, дори да се повтарят
	int* pointerToStates;
	bool isIn = false;

	for (std::map<int, std::map<char, int> >::iterator i = table.begin();
			i != table.end(); i++)
		for (std::map<char, int>::iterator j = i->second.begin();
				j != i->second.end(); j++) {
			allStates[k++] = i->first;
			allStates[k++] = j->second;
		}

	int newAllStates[countOfStates];
	pointerToStates = new int[countOfStates];
	for (int i = 0; i < countOfStates; i++)
		newAllStates[i] = -2;
	for (int i = 0; i < k; i++) { // в newAllStates записваме числата (т.е. състоянията), които не се повтарят
		isIn = false;
		for (int j = 0; j < countOfStates; j++)
			if (allStates[i] == newAllStates[j])
				isIn = true;
		if (isIn == false) {
			newAllStates[count] = allStates[i];
			pointerToStates[count] = allStates[i];
			count++;
		}
	}
	std::sort(pointerToStates, pointerToStates + countOfStates); // Сортираме състоянията във възходящ ред
	return pointerToStates;
}

int DFA::maxNameOfState() {
	int max = states[0];
	for (int i = 1; i < countOfStates; i++) {
		if (max < states[i])
			max = states[i];
	}
	return max;
}

void DFA::changeAllStatesName(int start) {
	int newStates[countOfStates];
	int k = 0, oldState, index;
	for (int i = 0; i < countOfStates; i++) {
		newStates[i] = start;
		if (isFinalState(states[i]))
			finalStates[k++] = start;
		start++;
	}

	for (int i = 0; i < countOfStates; i++) { // Презаписваме таблицата с новите състояния
		for (int j = 0; j < countOfAlphabet; j++) {
			if (transitionFunction(states[i], alphabet[j]) != -2
					&& transitionFunction(states[i], alphabet[j]) != -1) {
				oldState = table[states[i]][alphabet[j]]; //трябва да вземем индекса на oldState и да вземем новото име така newStates[индекс на oldState]

				for (int z = 0; z < countOfStates; z++)
					if (states[z] == oldState)
						index = z;

				table[newStates[i]][alphabet[j]] = newStates[index];
				table[states[i]].erase(alphabet[j]); // Изтриваме старите състояния
			}
		}
	}

	for (int i = 0; i < countOfStates; i++) {
		if (startState == states[i])
			startState = newStates[i];
		states[i] = newStates[i];
	}
}

DFA::DFA(const DFA& automat) {
	countOfAlphabet = automat.countOfAlphabet;
	for (int i = 0; i < countOfAlphabet; i++)
		alphabet[i] = automat.alphabet[i];
	table = automat.table;
	startState = automat.startState;
	countOfFinalStates = automat.countOfFinalStates;
	countOfStates = automat.countOfStates;
	finalStates = new int[countOfFinalStates];
	states = new int[countOfStates];
	for (int i = 0; i < countOfFinalStates; i++)
		finalStates[i] = automat.finalStates[i];
	for (int i = 0; i < countOfStates; i++)
		states[i] = automat.states[i];
}

DFA::DFA(int _startState, int* _finalStates, int _countOfFinalStates,
		std::map<int, std::map<char, int> > _table) {
	int k = 0;
	for (int i = 97; i <= 122; i++)
		alphabet[k++] = (char) i;
	for (int i = 48; i <= 57; i++)
		alphabet[k++] = (char) i;
	alphabet[k] = '\0';
	countOfAlphabet = k;
	table = _table;
	countOfFinalStates = _countOfFinalStates;
	countOfStates = getCountOfStates();
	states = new int[countOfStates];
	states = getStates();
	finalStates = new int[countOfFinalStates];
	std::sort(_finalStates, _finalStates + countOfFinalStates);
	for (int i = 0; i < countOfFinalStates; i++)
		finalStates[i] = _finalStates[i];
	if (isValidState(_startState))
		startState = _startState;
	else
		throw "Error: start state is not valid.";
}

DFA::~DFA() {
	delete[] states;
	delete[] finalStates;
}

bool DFA::checkWord(char* word) {
	if (isFinalState(extendedTransitionFunction(startState, word)))
		return true;
	return false;
}

void DFA::addErrorState() {
	for (int i = 0; i < countOfStates; i++)
		for (int j = 0; j < countOfAlphabet; j++)
			if (table.find(states[i]) == table.end()
					|| table[states[i]].find(alphabet[j])
							== table[states[i]].end())
				table[states[i]][alphabet[j]] = -1;

}

std::ostream& operator<<(std::ostream& os, DFA automat) {
	os << "Start state: " << automat.startState << std::endl;
	os << "Final states: ";
	for (int i = 0; i < automat.countOfFinalStates; i++)
		if (i != automat.countOfFinalStates - 1)
			os << automat.finalStates[i] << ",";
		else
			os << automat.finalStates[i] << std::endl;
	for (int i = 0; i < automat.countOfStates; i++) {
		for (int j = 0; j < automat.countOfAlphabet; j++) {
			std::cout << "i:" << i << " j:" << j << std::endl;
			os << "table[" << automat.states[i] << "]['" << automat.alphabet[j]
					<< "']="
					<< automat.transitionFunction(automat.states[i],
							automat.alphabet[j]) << ";" << std::endl;
		}
	}
	return os;
}

void unionOfTwoAutomata(DFA automat1, DFA automat2) {
	int newStartState, newFinalStates[automat1.countOfFinalStates
			+ automat2.countOfFinalStates];
	int maxStateName1 = automat1.maxNameOfState() + 1;
	int maxStateName2 = automat2.maxNameOfState() + 1;
	std::map<int, std::map<char, int> >::iterator i;
	std::map<char, int>::iterator j;

	if (maxStateName2 >= maxStateName1) { // променяме автомат2 и извеждаме пъвро автомат1 след това автомат2
		automat2.changeAllStatesName(maxStateName2);
		newStartState = automat2.maxNameOfState() + 1;
		for (i = automat1.table.begin(); i != automat1.table.end(); i++)
			for (j = i->second.begin(); j != i->second.end(); j++)
				std::cout << "table[" << i->first << "][" << j->first << "]="
						<< j->second << std::endl;

		for (i = automat2.table.begin(); i != automat2.table.end(); i++)
			for (j = i->second.begin(); j != i->second.end(); j++)
				std::cout << "table[" << i->first << "][" << j->first << "]="
						<< j->second << std::endl;
	} else { // променяме автомат1 и извеждаме пъвро автомат2 след това автомат1
		automat1.changeAllStatesName(maxStateName1);
		newStartState = automat1.maxNameOfState() + 1;
		for (i = automat2.table.begin(); i != automat2.table.end(); i++)
			for (j = i->second.begin(); j != i->second.end(); j++)
				std::cout << "table[" << i->first << "][" << j->first << "]="
						<< j->second << std::endl;
		for (i = automat1.table.begin(); i != automat1.table.end(); i++)
			for (j = i->second.begin(); j != i->second.end(); j++)
				std::cout << "table[" << i->first << "][" << j->first << "]="
						<< j->second << std::endl;
	}

	// извеждаме прегодите от новото начално състояния
	for (int i = 0; i < automat1.countOfAlphabet; i++) {
		if (automat1.transitionFunction(automat1.startState,
				automat1.alphabet[i]) != -2) {
			std::cout << "table[" << newStartState << "]["
					<< automat1.alphabet[i] << "]="
					<< automat1.transitionFunction(automat1.startState,
							automat1.alphabet[i]) << std::endl;
		}

		if (automat2.transitionFunction(automat2.startState,
				automat2.alphabet[i]) != -2) {
			std::cout << "table[" << newStartState << "]["
					<< automat2.alphabet[i] << "]="
					<< automat2.transitionFunction(automat2.startState,
							automat2.alphabet[i]) << std::endl;
		}
	}

	int k = 0;
	// вземаме финалните състояния на новия автомат
	for (int i = 0; i < automat1.countOfFinalStates; i++)
		newFinalStates[k++] = automat1.finalStates[i];
	for (int i = 0; i < automat2.countOfFinalStates; i++)
		newFinalStates[k++] = automat2.finalStates[i];
	// ако поне едно от двете начални състояния на двата автомата е финално, правим и началното на новия автомат финално
	if (automat1.isFinalState(automat1.startState)
			|| automat2.isFinalState(automat2.startState)) {
		newFinalStates[k++] = newStartState;
	}
	// извеждаме автомата
	std::cout << "Start state: " << newStartState << std::endl
			<< "Final States: ";
	for (int i = 0; i < k; i++)
		std::cout << newFinalStates[i] << " ";
}

