/* @Uğur Demir
* Student Name: <Uğur Demir>
* Student ID : <150160138>
* Date: <01-Dec-18> */
#include <fstream>
#include <iostream>

using namespace std;

struct Node {	
	int data;
	Node *next;
};	

struct stack {
	Node *head;
	void initialize(); //intialize headptr
	int pop(); //pop a nubmer formthe stack
	void push(int); //pusha number into stack
	bool isempty(); //return true if stack is empty
};

bool input(stack*, stack*, stack*, const char*);
int give_card(stack*, stack*, int);

int main(int argc, char* argv[]) {
	(void)argc;
	stack player1, player2, deck;
	player1.initialize();
	player2.initialize();
	deck.initialize();
	
	bool file;
	file = input(&player1, &player2, &deck, argv[1]); //input from file is taken and inserted to the stacks.
	if (!file) { return 0; }

	int turn = 0, data, Thebin = 0;//While turn is even its player1's.

	//Goes into loop if there are still cards on the deck to be pulled and player hand's are not empty
	while (!deck.isempty() && !player1.isempty() && !player2.isempty()) { 
		data = deck.pop(); 
		
		//Deciding who will be giving cards
		if (turn % 2 == 0) { 	//player1's turn
			if (data < 0) { 	//player1  giving cards
				Thebin += give_card(&player1, &player2, abs(data));
			}
			else { 				//player1 taking cards
				Thebin += give_card(&player2, &player1, abs(data));
			}
		}
		else { 					//player2's turn
			if (data < 0) { 	//player2 giving cards
				Thebin += give_card(&player2, &player1, abs(data));
			}
			else { 				//player2 taking cards
				Thebin += give_card(&player1, &player2, abs(data));
			}
		}
		turn++;
	}
	cout << Thebin;

	return 0;
}
//reads file Input in to player hands and the deck
bool input(stack *player1, stack *player2, stack *deck, const char* fileName) {
	ifstream fileIn;
	int deckSize, playerHandSize, numberHold ,i = 0;
	fileIn.open(fileName);
	if (fileIn.fail()) {
		cout << "File could not be oppened";
		return false;
	}
	fileIn >> deckSize >> playerHandSize; // reads hand sizes
	while (i < deckSize) { // reads deck
		fileIn >> numberHold;
		deck->push(numberHold);
		i++;
	}
	i = 0;
	while (i < playerHandSize) { //reads player1's hand
		fileIn >> numberHold;
		player1->push(numberHold);
		i++;
	}
	i = 0;
	while (i < playerHandSize) { //reads player2's hand
		fileIn >> numberHold;
		player2->push(numberHold);
		i++;
	}
	fileIn.close();

	return true;
}

void stack::initialize() {
	head = NULL;
	return;
}

//Pops the last element to be entered in to the stack
int stack::pop() {
	int result;
	Node *interim;
	interim = head;
	if (!isempty()) {
		if (interim->next) {
			head = interim->next;
			result = interim->data;
			//cout << "Poped an element off the stack " << result << endl;
			delete interim;
			return result;
		}else{
			head = NULL;
			result = interim->data;
			//cout << "Poped the last element off the stack " << result << endl;
			delete interim;
			return result;
		}
	}
	return 0;
	
}

//Inserts the given integer to the front of hte linked list as head
void stack::push(int a) {
	Node *newNode;
	newNode = new Node;
	newNode->data = a; //create a node to be placed in to stack LL
	
	//if head is alrady loking at a node place the newnode to the front
	if(head){ 
		newNode->next = head;
		head = newNode;
	}
	else { //if the stack is empty directly add newnode to the head
		head = newNode;
		head->next = NULL;
	}
	
	return;
}

bool stack::isempty() {
	if (!head) { //if head is pointin got null the stack is empty
		return true;
	}
	return false;
}



int give_card(stack *give_stack, stack *take_stack, int amount) {
	int bin = 0, i = 0;
	while(i < amount && !give_stack->isempty() ) { //make sure giving stack is not empty
		if (give_stack->head->data > take_stack->head->data) { //checking if the nubmer to be given is bigger than uppermost number on the taking stack
			take_stack->push(give_stack->pop());
		}
		else { //if not throw it into bin
			give_stack->pop();
			bin++;
		}
		i++;
	}
	return bin; //return the number of cards thrown into bin
}

