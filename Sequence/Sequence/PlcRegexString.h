//---------------------------PlcRegexString.h---------------------------------------
#ifndef PLCREGEXSTRING_H
#define PLCREGEXSTRING_H

#include <iostream>
#include <regex>
#include "ElementTraits.h"
using namespace std;

template <typename T>
class PlcRegexString {
public:
	PlcRegexString(char* text, bool isBegin = true): reg(R"(\w)") {
		if (isBegin) {
			startingPoint = &text[0];
			initData(text);
		} else {
			begin = nullptr;
			end = nullptr;
		}
	}

	PlcRegexString(char* text, char* r, bool isBegin = true): reg(r) {
		if (isBegin) {
			startingPoint = &text[0];
			initData(text);
		} else {
			begin = nullptr;
			end = nullptr;
		}
	}

	PlcRegexString& operator ++() {
		initData(end);
		return *this;
	}

	PlcRegexString operator ++(int) {
		PlcRegexString temp = (*this);
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

	bool operator == (PlcRegexString const& it) const {
		return (begin == it.begin && end == it.end);
	}

	bool operator != (PlcRegexString const& it) const {
		return (begin != it.begin || end != it.end);
	}

	void setRegex(char* r) {
		reg.assign(r);
		initData(startingPoint);
	}

private:
	char* begin;
	char* end;
	T currentElement;
	regex reg;
	char* startingPoint;

	void initData(char* text) {
		begin = &text[0];
		end = &text[0];
		cmatch cm;
		if (regex_search(begin, cm, reg)) {
			begin += cm[0].first - begin;
			end = begin + cm[0].str().size();
		} else {
			begin = nullptr;
			end = nullptr;
		}
	}
};

#endif