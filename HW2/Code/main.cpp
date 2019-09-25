/* @Uğur Demir
* Student Name: <Uğur Demir>
* Student ID : <150160138>
* Date: <05-Nov-18> */


#include <fstream>
#include <iostream>
#include <string>
#include "header.h"
#include <ctype.h>
#include <string.h>

using namespace std;

void language_model::readData(const char* fileName) {
	ifstream fileIn;
	string sentence;
	unsigned int i;
	fileIn.open(fileName);
	if (fileIn.fail()) cerr << "File could not be opened.";
	while (!fileIn.eof()) {
		getline(fileIn, sentence);
		for (i = 0; i < sentence.size(); i++) {
			sentence[i] = toupper(sentence[i]);
			vocabularylist->add_char(sentence[i]);
		}

		int lenght = sentence.size();
		int before, after;
		for (int i = 0; i < lenght; i++) {
			before = i;
			after = i + 1;
			if (sentence[before] == sentence[after]) {
				continue;
			}
			if (after == lenght) {
				vocabularylist->add_occurance(sentence[before], ' ');
				break;
			}
			vocabularylist->add_occurance(sentence[before], sentence[after]);
		}
	}
	return;
}

int main(int argc, char* argv[]) {
	language_model model;
	model.initialize();
	model.vocabularylist->create(); //create a Vocab Node in Language Model.
	model.readData(argv[1]);

	if(!argv[2]){
        model.vocabularylist->print();
	}
    else{
        char first,second;
        if(strcmp(argv[2],"<SP>") == 0){
            first = ' ';
            second = toupper(argv[3][0]);
        }else if(strcmp(argv[3],"<SP>") == 0){
            first = toupper(argv[2][0]);
            second = ' ';
        }else{
            first = toupper(argv[2][0]);
            second = toupper(argv[3][0]);
        }
        int BigBoyInteger = model.calculateProbability(second, first);
        if(!BigBoyInteger){
            return 0;
        }else {
             cout <<"Probability of '" << argv[2][0] << argv[3][0] << "' appering is: " << BigBoyInteger;
             return 0;
        }
    }
}



