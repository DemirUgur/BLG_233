#ifndef _HEADER_
#define _HEADER_
struct occur_node {
	char character;
	occur_node *next;
	int occurance;
};
struct vocab_node {
	char character;
	void initialize();
	vocab_node *next;
	occur_node *list;
};

struct vocab_list{
	vocab_node *head;
	void create();
	void print();
	void printDecoy();
	void add_char(char);
	void add_occurance(char, char);
	int get_occurance(char);
	int get_union_occurance(char, char);
	int nodecount;
};

struct language_model{
	vocab_list *vocabularylist;
	void initialize();
	void readData(const char*);
	double calculateProbability(char, char);
};
#endif // _HEADER_
