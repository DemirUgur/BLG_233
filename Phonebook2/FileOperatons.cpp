#include "FileOperationsHeader.h"
#include <iostream>
#include <stdlib.h>//general utilities
#include <string.h>//String operations

using namespace std;

void File::add(Phone_Record *ptr){
	fseek(phonebook, 0 ,SEEK_END);
	fwrite(ptr, sizeof(Phone_Record),1,phonebook);
}
void File::create(){
	filename = "phonebook.txt";
	phonebook = fopen(filename, "r+" );
	if(!phonebook){		
		if(!(phonebook = fopen( filename, "w+" ))){
			cerr << "Cannot open file" << endl;
			exit(1);
		}
	}
}

void file::close(){
	fclose(phonebook);
}

int file::search(char *desired){
	Phone_Record k;
	int index = 0;
	bool all = false;
	int found = 0;
	if(strcmp(desired,"*") == 0){
		all = true;
	}
	fseek(phonebook,0,SEEK_SET);
	while(!feof(phonebook)){
		fread(&k,sizeof(Phone_Record),1,phonebook);
		if(feof(phonebook)) break;
		
		if(all || strnicmp(k.name,desired,strlen(desired)) == 0){
			found ++;
			cout << index << "." << k.name << " " << k.phonenum << endl;
		}
	}
	return found;
}

void File::update(int recordnum, Phone_Record *ptr){
	if(fseek(phonebook, sizeof(Phone_Record) * (recordnum - 1), SEEKS_SET) == 0){
		fwrite(ptr, sizeof(Phone_Record), 1, phonebook);
	}
}

void File::remove(int recordnum){
	Phone_Record emptyRecord = {"",""};
	if(fseek(phonebook, sizeof(Phone_Record) * (recordnum - 1), SEEK_SET) == 0){
		fwrite(&emptyRecord,sizeof(Phone_Record),1,phonebook);
	}
}