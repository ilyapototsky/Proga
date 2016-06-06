//-------------------------ElementTraits.h-------------------------------------
#ifndef ELEMENT_TRAITS_H
#define ELEMENT_TRAITS_H

#include <iostream>
using namespace std;

template <typename Element>
class ElementTraits {
public:
	static Element charToElement(char* text) {
		return (Element)text;
	}
};

template <>
class ElementTraits < int > {
public:
	static int charToElement(char* text) {
		return atoi(text);
	}
};

template <>
class ElementTraits < double > {
public:
	static double charToElement(char* text) {
		return atof(text);
	}
};

#endif // !ELEMENT_TRAITS_H