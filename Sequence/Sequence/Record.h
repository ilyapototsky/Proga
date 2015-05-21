//-----------------------------Record.h----------------------------------------
#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>
#include "PlcString.h"
using namespace std;

struct Record
{
    Record(char* record)
    {
        PlcString<char*> iter(record, ", ");
        surname = *(iter++);
        name = *(iter++);
        course = atoi(*(iter++));
        group = atoi(*iter);
    }
    
    Record(char* _surname, char* _name, int _course, int _group):
       surname(_surname), name(_name), course(_course), group(_group) {}

    friend ostream& operator << (ostream& os, const Record &r)
    {
       os << r.surname << " " << r.name << ", " << r.course
          << " course, " << r.group << " group.";
       return os;
    }
    
	string surname;
    string name;
    int course;
    int group;
};

#endif