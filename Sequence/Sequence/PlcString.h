//---------------------------PlcString.h---------------------------------------
#ifndef PLCSTRING_H
#define PLCSTRING_H

#include <iostream>
#include "ElementTraits.h"
using namespace std;

template <typename T>
class PlcString {
public:
	PlcString(char* text): separators(" \t\n") {
		initData(text);
	}

	PlcString(char* text, char* sep): separators(sep) {
		initData(text);
	}

	PlcString& operator ++() {
		if ((*end) == '\0') {
			begin = end;
			return (*this);
		}
		while (isSeparator(*(++end)));
		begin = end;
		if ((*end) != '\0')
			while (!isSeparator(*(++end)) && (*end) != '\0');
		return (*this);
	}

	PlcString operator ++(int) {
		PlcString temp = (*this);
		++(*this);
		return temp;
	}
	
	T const& operator *() {
		int size = end - begin;
		char* temp = new char[size + 1];
		int i = 0;
		for (char* p = begin; p != end; ++p)
			temp[i++] = (*p);
		temp[size] = '\0';
		currentElement = ElementTraits<T> ::charToElement(temp);
		return currentElement;
	}

	PlcString(char* text, bool isBegin): separators(" \t\n") {
		initData(text);

		if (!isBegin)
			while (begin != end)
				++(*this);
	}

	bool operator == (PlcString const& it) const {
		return (begin == it.begin && end == it.end && startingPoint == it.startingPoint);
	}

	bool operator != (PlcString const& it) const {
		return (begin != it.begin || end != it.end || startingPoint != it.startingPoint);
	}

	void setSeparators(char* sep) {
		separators = sep;
		end = begin;
		while (!isSeparator(*(++end)));
	}

private:
	char* begin;
	char* end;
	T currentElement;
	char* separators;
	char* startingPoint;

	void initData(char* text) {
		startingPoint = &text[0];
		int i = -1;
		while (isSeparator(text[++i]));
		begin = &text[i];
		while (!isSeparator(text[++i]) && text[i] != '\0');
		end = &text[i];
	}

	bool isSeparator(char ch) {
		int i = 0;
		while (separators[i] != '\0') {
			if (ch == separators[i])
				return true;
			++i;
		}
		return false;
	}
};

#endif