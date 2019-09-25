/*
* Student Name: <Uğur Demir>
* Student ID : <150160138>
* Date: <24-Dec-18> */

#include <iostream>
#include <fstream>

using namespace std;

struct StackNode {
	int data;
	StackNode *next;
};

struct stack {
	StackNode *head;
	void initialize(); //intialize headptr
	int pop(); //pop a nubmer formthe stack
	void push(int); //pusha number into stack
	bool isempty(); //return true if stack is empty
};

struct Node {
	int key, index;
	Node *left = NULL;
	Node *right = NULL;
};

struct tree {
	Node *root = NULL;
};

void stack::initialize()
{
	head = NULL;
	return;
}

int stack::pop()
{
	int result;
	StackNode *interim;
	interim = head;
	if (!isempty()) {
		if (interim->next) {
			head = interim->next;
			result = interim->data;
			//cout << "Poped an element off the stack " << result << endl;
			delete interim;
			return result;
		}
		else {
			head = NULL;
			result = interim->data;
			//cout << "Poped the last element off the stack " << result << endl;
			delete interim;
			return result;
		}
	}
	return 0;
}

void stack::push(int a)
{
	StackNode *newNode;
	newNode = new StackNode;
	newNode->data = a; //create a node to be placed in to stack LL

	//if head is alrady loking at a node place the newnode to the front
	if (head) {
		newNode->next = head;
		head = newNode;
	}
	else { //if the stack is empty directly add newnode to the head
		head = newNode;
		head->next = NULL;
	}

	return;
}

bool stack::isempty()
{
	return(!head);
}
/*Uses the index of inputs inorder to construct the tree.
Continues until the amount of read elements is equal to arraysize
*/
void addToTree(Node* parent, int inputArray[], int NumberofReadElements, int arraySize) {
	int index = parent->index; 
	if(NumberofReadElements < arraySize){
		if(parent->left == NULL && 2 * index <= arraySize){ //Makes sure the left child is empty and the array can support the ith to 2ith rule.
			Node* newNodeleft = new Node; //creates new Node and fills with data.
			newNodeleft->key = inputArray[2 * index - 1];
			newNodeleft->index = 2 * index;
			parent->left = newNodeleft; //Assigns as the child node
			NumberofReadElements++;
			addToTree(parent->left, inputArray, NumberofReadElements, arraySize); //Calls the function for the left child
		}
		//once the left is done continues with the right and does the same thing.
		if(parent->right == NULL && 2 * index + 1 <= arraySize){ //Makes sure we are within the array.Also making sure the child of a certain parent node exists
			Node* newNoderight = new Node;
			newNoderight->key = inputArray[2 * index];
			newNoderight->index = 2 * index + 1;
			parent->right = newNoderight;
			NumberofReadElements++;
			addToTree(parent->right, inputArray, NumberofReadElements, arraySize);
		}
	}
	return;
}

bool Pathfinder(Node* parent, int target, int currentSum, stack* path) {
	int ongoingSum = currentSum;
	ongoingSum += parent->key; //Adds the current nodes key value to the sum of keys so far.

	if (ongoingSum == target) { // Target achieved with the current Node.
		path->push(parent->key); 
		return true;
	}

	if (ongoingSum > target) { //if current sum is bigger than target 

		if (currentSum == target) { //if root is the target
			return true;
		}
		return false; // returns false if the current node key is too big for the target taking account the target value.
	}
	//if no left node then no right node since written in level order
	//value is not bigger than target and there exist other nodes to look at.
	if (parent->left != NULL) { 
		path->push(parent->key); //We push the current value onto stack.
		
		if (ongoingSum == target) { return true; } // We achieve target with current node.
		
		// There exists a Node the left of the parent node.
		if (parent->left != NULL) { 
			// Since preorder we see if we can achieve target going left of the parent node
			// if not we now move to the right.
			if (!Pathfinder(parent->left, target, ongoingSum, path)) { 

				if(parent->right){ //makes sure there is a right node to be checked
					bool twostepsback = Pathfinder(parent->right, target, ongoingSum, path); 
					//if a parent node gets false from both child nodes we must take 
					//one step back from the parent node therefore we must remove the key value of parent node from the stack.
					if (!twostepsback) { //checks ifs seach on the right node returns false.
						path->pop();
					}
					return twostepsback; // returns the result of the right search.(the path is valid or not).
				}
				//left Node failed to give us a valid path and there exists no right Node.
				path->pop(); // We remove parent Nodes key value from stack.
				return false; // Return false to indicate no valid path is found.
			}
			return true; //Left Node results in a valid path.
		}
	}
	if (ongoingSum != target) { //no child nodes and result is not target.
		return false; // no valid path.
	}
	return false;
}

int main(int argc, char* argv[]){
	tree jeff;
	fstream fptr;
	fptr.open(argv[1]);
	int treeSize = -1;
	int key, target;
	//Amount of elements that will be included in the tree
	while (fptr >> key) {
		treeSize++;
	}
	//sets the ifstream pointer to the beginning of the file
	fptr.clear();
	fptr.seekg(0, ios::beg);
	//read the tree content into an array
	int *inputArray = new int[treeSize];
	for (int i = 0; i < treeSize; i++) {
		fptr >> key;
		inputArray[i] = key;
	}
	//initiates the tree
	jeff.root = new Node;
	jeff.root->key = inputArray[0];
	jeff.root->index = 1;	
	addToTree(jeff.root, inputArray, 1, treeSize); //constructs the tree.
	fptr >> target;

	stack Leftpath;
	Leftpath.initialize();
	Leftpath.push(jeff.root->key);
	int currentSum = 0;
	currentSum += jeff.root->key;
	bool LeftPathFound = Pathfinder(jeff.root->left, target, currentSum, &Leftpath);

	stack RightPath;
	RightPath.initialize();
	RightPath.push(jeff.root->key);
	currentSum = 0;
	currentSum += jeff.root->key;
	bool RightPathFound = Pathfinder(jeff.root->right, target, currentSum, &RightPath);

	if (!LeftPathFound) {
		cout << "No Path Found" << endl; 
	}
	else{
		cout << "Path Found: ";
		int *outputArray = new int[treeSize / 2];
		int i = -1;
		while (!Leftpath.isempty()) {
			i++;
			outputArray[i] = Leftpath.pop();
		}
		for (; i >= 0; i--) {
			cout << outputArray[i] << " ";
		}
		cout << endl;
		delete [] outputArray;
	}
	if (!RightPathFound) {
		cout << "No Path Found";
	}
	else{
		cout << "Path Found: ";
		int *outputArray = new int[treeSize / 2];
		int i = -1;
		while (!RightPath.isempty()) {
			i++;
			outputArray[i] = RightPath.pop();
		}
		for (; i >= 0; i--) {
			cout << outputArray[i] << " ";
		}
		delete[] outputArray;
	}
}
