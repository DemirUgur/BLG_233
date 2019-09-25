#ifndef FILEOPERAIONSHEADER_H
#define FILEOPERATIONSHEADER_H
#include <stdio.h>
#include "record.h"

struct File{
	char *filename;
	FILE *phonebook;
	void create();
	void close();
	void add(Phone_Record *);
	void search(char []);
	void update(int recordnum ,Phone_Record*);
	void remove(int recordnum);
};
#endif