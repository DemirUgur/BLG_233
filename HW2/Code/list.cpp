#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>

using namespace std;
void language_model::initialize() {
	vocabularylist = new vocab_list;
	return;
}

void vocab_node::initialize() {
	next = NULL;
	list = NULL;
	return;
}

void vocab_list::create() {
	head = NULL;
	nodecount = 0;
	return;
}

void vocab_list::printDecoy() {
	vocab_node *traverse;
	traverse = head;
	while (traverse) {
		cout << traverse->character << endl;
		traverse = traverse->next;
	}
	return;
}

void vocab_list::print() {
	vocab_node *traverse;
	occur_node *traverseList;
	traverse = head;

	while (traverse) {
		traverseList = traverse->list;
		if (traverse->character == 32) {
			cout << "<SP>:" << endl;
		}
		else {
			cout << traverse->character << ":" << endl;
		}
		while (traverseList) {
			cout << "\t";
			if (traverseList->character == 32) {
				cout << "<<SP>,";
			} else {
				cout << "<" << traverseList->character << ",";
			}
			if (traverseList->occurance == 32) {
				cout << "<SP>>" << endl;
			}
			else {
				cout << traverseList->occurance << ">" << endl;
			}
			traverseList = traverseList->next;
		}
		traverse = traverse->next;
	}
	return;
}

void vocab_list::add_char(char letter) {//Gets a char from txt file and adds it to the vocab_list using vocab_node.
	//letter = toupper(letter);

	if (!head) {
		head = new vocab_node;
		head->initialize();
		head->character = letter;
		nodecount++;

		return;
	}

	//bool exist = false;
	vocab_node *traverse;
	traverse = head;

	while (traverse) {//Checks if char has already been placed
		if (traverse->character == letter) return;
		traverse = traverse->next;

	}

	traverse = head; //Rewinds to the start
	vocab_node *newnode = new vocab_node;
	newnode->initialize();
	vocab_node *insert_after = NULL;
	newnode->character = letter;

	if (letter < 64) {//Checks if char is a punctuation then adds to the end of the LL

		for (int i = 1; i < nodecount; i++) {
			traverse = traverse->next;
		}
		traverse->next = newnode;
		nodecount++;
		return;
	}

	if (letter < traverse->character) {//insert before head
		newnode->next = head;
		head = newnode;
		nodecount++;
		return;
	}
	while(traverse && (letter > traverse->character) && (traverse->character > 64)){ //Find a middle spot
		insert_after = traverse;
		traverse = traverse->next;
	}
	if(traverse){ //Add to middle spot
		newnode->next = traverse;
		insert_after->next = newnode;
		nodecount++;
	}
	else { //Add to the end
		insert_after->next = newnode;
		newnode->next = NULL;
		nodecount++;
	}
}

void vocab_list::add_occurance(char a, char b) {
	vocab_node *traverse;
	occur_node *occurTraverse;
	occur_node *insert_after = NULL;
	traverse = head;
	while (traverse->character != a) {
		traverse = traverse->next;
	}
	if (!traverse->list) {//No occurance, adding as first occurance

		traverse->list = new occur_node;
		traverse->list->character = b;
		traverse->list->occurance = 1;
		traverse->list->next = NULL;
		return;
	}

	occurTraverse = traverse->list;

	while (occurTraverse && (occurTraverse->character != b)) {
		insert_after = occurTraverse;
		occurTraverse = occurTraverse->next;

	}

	if (occurTraverse) { occurTraverse->occurance++; return; }//Already existent.adding to occurance

	occur_node *newnode = new occur_node;
    newnode->character = b;
	newnode->occurance = 1;
	newnode->next = NULL;

	insert_after->next = newnode; //Placing to the end.
}

int vocab_list::get_occurance(char a) {
	vocab_node *traverse;
	occur_node *yeet;
	int totalOccurance = 0;
	traverse = head;

	while (traverse && traverse->character != a) {
		traverse = traverse->next;
	}
	if (!traverse) {
		return 0;
	}
	yeet = traverse->list;
	while (yeet) {
		totalOccurance += yeet->occurance;
		yeet = yeet->next;
	}
	return totalOccurance;
}

int vocab_list::get_union_occurance(char a, char b) {
	vocab_node *traverse;
	occur_node *yeet;
	//int union_occurance;
	traverse = head;
	while (traverse && traverse->character != a) {
		traverse = traverse->next;
	}
	if (!traverse) {
		return 0;
	}

	yeet = traverse->list;
	while (yeet && yeet->character != b) {
		yeet = yeet->next;
	}
	if(yeet){
		return yeet->occurance;
	}
	else {
		return 0;
	}
}

double language_model::calculateProbability(char a, char b) {
	double beep = vocabularylist->get_union_occurance(b, a);
	double boop = vocabularylist->get_occurance(b);
	if (!boop) {
		cout << "The text does not contain " << b << "." << endl;
		return 0;
	}
	else if (!beep) {
		cout << "No occurance of " << b << a << " is found." << endl;
		return 0;
	}else{
		double probability = beep / boop;
		return probability;
	}
}
