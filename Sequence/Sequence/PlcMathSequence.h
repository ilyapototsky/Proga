//-------------------------PlcMathSequence.h-----------------------------------
#ifndef PLC_MATH_SEQUENCE_H
#define PLC_MATH_SEQUENCE_H

#include <iostream>
using namespace std;

template <typename T>
class PlcMathSequence {
public:
	PlcMathSequence(int numOfStartingValues, T* startingValues) {
		currentMember = 0;
		num = numOfStartingValues;
		currentValues = new T[numOfStartingValues];
		for (int i = 0; i < numOfStartingValues; ++i)
			currentValues[i] = startingValues[i];
	}

	PlcMathSequence(int numOfStartingValues, T* startingValues, bool isBegin):
		PlcMathSequence(numOfStartingValues, startingValues) {
	}

	PlcMathSequence& operator ++() {
		getNext();
		currentMember++;
		return (*this);
	}

	PlcMathSequence operator ++(int) {
		PlcMathSequence temp = (*this);
		++(*this);
		return temp;
	}

	PlcMathSequence& operator --() {
		getPrev();
		currentMember--;
		return (*this);
	}

	PlcMathSequence operator --(int) {
		PlcMathSequence temp = (*this);
		--(*this);
		return temp;
	}

	T const& operator *() {
		return currentValues[0];
	}

	bool operator == (PlcMathSequence const& it) const {
		return currentMember == it.currentMember;
	}

	bool operator != (PlcMathSequence const& it) const {
		return currentMember != it.currentMember;
	}

	void setLast(int lastNum) {
		currentMember = lastNum;
	}

protected:
	virtual void getNext() {};
	virtual void getPrev() {};
	int num;
	T* currentValues;
	int currentMember;
};

#endif