//------------------------BinaryFileRecord.h-----------------------------------
#ifndef BINARY_FILE_RECORD_H
#define BINARY_FILE_RECORD_H

#include <iostream>
using namespace std;

struct BinaryFileRecord {
	BinaryFileRecord(): iField(0), dField(0) {
		strcpy_s(cField, "\0");
	}

	BinaryFileRecord(int iF, char cF[], double dF): iField(iF), dField(dF) {
		strcpy_s(cField, cF);
	}

	friend ostream& operator << (ostream& os, const BinaryFileRecord &r) {
		os << r.iField << ", " << r.cField << ", " << r.dField;
		return os;
	}

	int iField;
	char cField[128];
	double dField;
};

#endif