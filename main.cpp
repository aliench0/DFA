/*
 * main.cpp
 *
 *  Created on: May 16, 2015
 *      Author: Hristiyan Nikolov
 *  Copyright (c) 2015. All rights reserved.
 */
#include <iostream>
#include "DFA.h"

int main() {
	//example
	std::map<int, std::map<char, int> > table, table1, table2;
	table[0]['a'] = 1;
	table[0]['b'] = 2;
	table[0]['c'] = 4;
	table[1]['a'] = 3;
	table[1]['b'] = 2;
	table[1]['c'] = 1;
	table[3]['b'] = 4;
	table[4]['b'] = 5;
	table[5]['b'] = 6;
	int finalStates[] = { 6, 1, 4 };
	int elements = sizeof(finalStates) / sizeof(finalStates[0]);

	table1[0]['c'] = 0;
	table1[0]['a'] = 1;
	table1[0]['b'] = 3;
	table1[1]['a'] = 2;
	table1[2]['b'] = 1;
	table1[3]['a'] = 2;
	table1[3]['c'] = 3;
	int finalStates1[] = { 3, 2 };

	table2[0]['0'] = -1;
	table2[0]['1'] = -1;
	table2[0]['2'] = -1;
	table2[0]['3'] = -1;
	table2[0]['4'] = -1;
	table2[0]['5'] = -1;
	table2[0]['6'] = -1;
	table2[0]['7'] = -1;
	int finalStates2[] = { 0, 3 };

	try {
		DFA automat1(0, finalStates1, 2, table1);
		DFA automat2(0, finalStates2, 2, table2);
		std::cout<<automat2.transitionFunction(0, '7');

	} catch (const char* msg) {
		std::cout << msg;
	}
	return 0;
}

