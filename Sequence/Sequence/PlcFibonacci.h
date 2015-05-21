//--------------------------PlcFibonacci.h-------------------------------------
#ifndef PLC_FIBONACCI_H
#define PLC_FIBONACCI_H

#include <iostream>
#include "PlcMathSequence.h"
using namespace std;

template <typename T>
class PlcFibonacci : public PlcMathSequence<T>
{
public:
	PlcFibonacci(T* startingValues) : PlcMathSequence<T>(2, startingValues) {}

	PlcFibonacci(T* startingValues, bool isBegin) : 
		PlcMathSequence<T>(2, startingValues, isBegin) {}

private:
	virtual void getNext()
	{
		T temp = currentValues[0];
		currentValues[0] = currentValues[1];
		currentValues[1] = temp + currentValues[0];
	}

	virtual void getPrev()
	{
		T temp = currentValues[1];
		currentValues[1] = currentValues[0];
		currentValues[0] = temp - currentValues[1];
	}
};

#endif