//-----------------------------Record.h----------------------------------------
#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>
#include "PlcString.h"
using namespace std;

struct Record {
	Record(): surname(""), name(""), course(0), group(0), info("") {}

	Record(char* record) {
		PlcString<char*> iter(record, ", ");
		surname = *(iter++);
		name = *(iter++);
		course = atoi(*(iter++));
		group = atoi(*(iter++));
		info = *(iter);
	}

	Record(char* _surname, char* _name, int _course, int _group, char* _info):
		surname(_surname), name(_name), course(_course), group(_group), info(_info) {}

	friend ostream& operator << (ostream& os, const Record &r) {
		os << r.surname << " " << r.name << ", " << r.course
			<< " course, " << r.group << " group, info: " << r.info << ".";
		return os;
	}

	string surname;
	string name;
	int course;
	int group;
	char* info;
};

#endif