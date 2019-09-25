#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip>

#include "FileOperationsHeader.h"

using namespace std;
typedef File Datastructure;
Datastructure book; // book is declared.

void print_menu();
bool perform_operation(char);
void search_record;
void add_record;
void delete_record;
void update_record;


int main(){
	book.create();
	bool end = false;
	char choice;
	while(!end){
		print_menu();
		cin >> choice;
		end = perform_operation(choice);
	}
	book.close();
	return EXIT_SUCCESS;
}

void print_menu(){
	system("clear");	
	cout << endl << endl;
	cout << "Phone Book Application" << endl;
	cout << "Choose an operation" << endl;
	cout << "S: Record Search" << endl;
	cout << "A: Record Add" << endl;
	cout << "U: Record Update" << endl;
	cout << "D: Record Delete" << endl;	
	cout << "E: Exit" << endl;	
	cout << endl;
	cout << "Enter a choice {S, A, U, D, E} : ";	
}

bool perform_operation(char choice){
	bool terminate = false;
	switch (choice){
		case'S': case's':
			search_record();
			break;
		case 'A': case 'a': 
			add_record();
			break; 
		case 'U': case 'u': 
			update_record();
			break;
		case 'D': case 'd': 
			delete_record();
			break;
		case 'E': case 'e':
			cout << "Are you sure you want to exit(Y/N): "
			cin >> choice;
			if(choice == 'y' || choice == 'Y'){
				terminate = true;
				break;
			}
		default:
			cout << "Error: You have entered an invalid choice" << endl; 
			cout << "Please try again {S, A, U, D, E} :" ;
			cin >> choice;
			terminate = perform_operation(choice);
			break;
	}
}
void search_record(){
	char name[NAME_LENGHT];
	cout << "Please enter the name of the person you want to search for (press '*' for full list):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(name,NAME_LENGHT);
	if(book.search(name) == 0){
		cout << "No record found" << endl;
	}
	getchar();
}

void add_record(){
	Phone_Record newrecord;
	cout << "Please enter contact information you want to add" << endl;
	cout << "Name : " ;	
	cin.ignore(1000, '\n');
	cin.getline(newrecord.name,NAME_LENGHT);
	cout << "Phone number :";
	cin >> setw(PHONENUM_LENGTH) >> newrecord.phonenum;
	book.add(&newrecord);
	cout << "Record added" << endl;
	getchar();
};

void delete_record(){
	char name[NAME_LENGHT];
	cin.getline(name,NAME_LENGHT);
	int personcount = book.search(name);
	if(personcount == 0){
		cout << "No record found";
	}
	else if(personcount ==1){
		cout << "1 record found do you want to continue?(Y/N)" << endl;
	}