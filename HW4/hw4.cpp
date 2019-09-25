/*
* Student Name: <Uğur Demir>
* Student ID : <150160138>
* Date: <10-Dec-18> */ 

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct stackNode { //stack node structure
	stackNode *next;
	int data;
};

struct queueNode { //Queue node structure
	queueNode *next;
	int data;
};

struct stackAnt { //Stack structure
	stackNode* head;
	void create();
	int pop();
	void push(int);
	bool isempty();
	void close();
};

struct queueAnt { //Queue structure
	queueNode *front, *back;
	void create();
	void enqueue(int);
	int dequeue();
	bool isempty();
	void close();
};

struct Ants {
	queueAnt ants;
	queueAnt holeDepths;
	stackAnt hole;
	void ReadFile(char*);
	void ShowContents(bool);
	void CrossRoad();
};

void queueAnt::create(){ //Sets queue's pointers to NULL
	front = NULL;
	back = NULL;
}

int main((int argc , char ** argv) {
	Ants a;
	a.ants.create();
	a.holeDepths.create();
	a.hole.create();

	char name[] = "test.txt";
	a. ReadFile ( argv [1]); // store the number of ants and depths of holes
	cout << "The initial Ant sequence is: ";
	a. ShowContents (1); // list ant sequence ( initially : 1, 2, ... , N)
	cout << "The depth of holes are : ";
	a. ShowContents (0); // list depth of holes
	a. CrossRoad ();
	cout << "The final Ant sequence is: ";
	a. ShowContents (1);

	a.ants.close();
	a.holeDepths.close();
	a.hole.close();
	return 0;
}

/********************************************************
 *	@param toadd: integer to be added into the queue    * 
 *  Adds the new integer into the back of the queue     *
 ********************************************************/
void queueAnt::enqueue(int toadd){ 
	queueNode* newnode = new queueNode;
	newnode->data = toadd;
	newnode->next = NULL;
	//If the queue is empty both ptrs point to the same data
	if (isempty()) { 
		back = newnode;
		front = back;
	}
	//If data already exists in the queue new data is added to the end of the queue
	else { 
		back->next = newnode;
		back = newnode;
	}
}

/**********************************************
 *	Removes the next element fromt the queue  *
 ***********************************************/
int queueAnt::dequeue(){
	queueNode* topnode; //Temporary queueNode the node to be removed
	int temp; //Value stored in to be deleted node
	topnode = front; //Since queue the node to be poped is the starting node
	front = front->next; 
	temp = topnode->data;
	delete topnode;
	return temp;
}
/**********************************
 *	Checks if the queue is empty  *
 ***********************************/
bool queueAnt::isempty(){ 
	return front == NULL;
}

/********************************************
 *	Deletes Allocated memory for the queue  *
 *********************************************/
void queueAnt::close()
{
	queueNode *traverse;
	//starting from beginning
	while (front) { 

		traverse = front;
		front = front->next;
		delete traverse;
	}
	return;
}

/*********************************
 *	Sets head of stack to NULL	 *
 *********************************/
void stackAnt::create()
{
	head = NULL;
	return;
}

/*********************************************
 *	Pops the next element from the stack	 *
 *********************************************/
int stackAnt::pop()
{
	stackNode *topnode; // To hold the stack to be poped
	int temp; // To hold the data from the poped stack
	topnode = head; // Since stack the head is poped
	head = head->next;
	temp = topnode->data;
	delete topnode;
	return temp;
}

/**********************************************************
 *	@param toadd: Integer to be added into the stack	  *
 *	Adds Integer to the stack. Adds the new data as a new *
 *  stack node and point head to it  				      *
 **********************************************************/
void stackAnt::push(int toadd)
{
	stackNode *newnode = new stackNode;
	newnode->data = toadd;
	newnode->next = head;
	head = newnode;
	return;
}

/**********************************
 *	Checks if the stack is empty  *
 **********************************/
bool stackAnt::isempty(){
	return head == NULL;
}

/*********************************************
 *	Deletes Allocated memory for the stack   *
 *********************************************/
void stackAnt::close()
{
	stackNode *p;
	while(head){
		p = head;
		head = head->next;
		delete p;
	}
}

/*************************************************
 *  @param filename: Name of the file to be read *
 *	Reads data from the given file and initiates * 
 *  the queues and stacks with the input         *
 *************************************************/
void Ants::ReadFile(char *filename)
{
	ifstream fileRead;
	int AntNumber, hole;
	fileRead.open(filename);
	if (fileRead.fail()) {
		cout << "File could not be oppened." << endl;
	}
	fileRead >> AntNumber; 
	// initiates  ant queue
	for (int i = 0; i < AntNumber; i++) {
		ants.enqueue(i+1);
	}
	//initiates holes queue storing ther sizes
	while (!fileRead.eof()) {
		fileRead >> hole;
		holeDepths.enqueue(hole);
	}
	return;
}

/******************************************************************
 *  @param b: indicates the data which would be printed to screen *
 *  Prints the contents of the queues 1 for ants 0 for holes      *
 ******************************************************************/
void Ants::ShowContents(bool b)
{
	queueNode *traverse;
	if (b) { //show the current ant sequence
		traverse = ants.front;
	}
	else { //shows the depth of holes
		traverse = holeDepths.front;
	}
	while (traverse) {
		cout << traverse->data << " ";
		traverse = traverse->next;
	}
	cout << endl;
	return;
}

/******************************************************************
 *  Simulates the ants through the holes and stores the last      *
 *  formation of ants in the same queue                           *
 ******************************************************************/
void Ants::CrossRoad()
{
	int holeHeight;
	// While there are still holes to be passed while loop is active
	while (!holeDepths.isempty()) { 
		holeHeight = holeDepths.dequeue(); // picks the first hole in the queue
		for (int i = 0; i < holeHeight; i++) { //ants are pushed into the hole's stack from the queue
			hole.push(ants.dequeue());
		}
		// After the hole is full they are pulled back and enqueued until no ants are left in the hole
		while (!hole.isempty()) {
			ants.enqueue(hole.pop());
		}
	}
	return;
}
