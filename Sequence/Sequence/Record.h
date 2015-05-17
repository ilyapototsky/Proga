#include <iostream>
#include <string>
#include "PlcText.h"
using namespace std;

struct Record
{
    Record(char* record)
    {
        PlcText iter(record, ", ");
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
    /*bool operator < (record const& r) const
    {
       return (destination<r.destination);
    }
    bool operator == (record const& r)
    {
       return (flight_number==r.flight_number);
    }*/
    
    string surname;
    string name;
    int course;
    int group;
};
