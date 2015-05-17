#include <iostream>
using namespace std;

class PlcText
{
public:
	PlcText(char* text) : separators(" \t\n")
	{
		startingPoint = &text[0];
		int i = -1;
		while (isSeparator(text[++i]));
		begin = &text[i];
		while (!isSeparator(text[++i]) && text[i] != '\0');
		end = &text[i];
	}

	PlcText(char* text, char* sep) : separators(sep)
	{
		startingPoint = &text[0];
		int i = -1;
		while (isSeparator(text[++i]));
		begin = &text[i];
		while (!isSeparator(text[++i]) && text[i] != '\0');
		end = &text[i];
	}

	PlcText& operator ++()
	{
		if ((*end) == '\0')
		{
			begin = end;
			return (*this);
		}
		while (isSeparator(*(++end)));
		begin = end;
		if ((*end) != '\0')
			while (!isSeparator(*(++end)) && (*end) != '\0');
		return (*this);
	}

	PlcText operator ++(int)
	{
		PlcText temp = (*this);
		++(*this);
		return temp;
	}

	PlcText& operator --()
	{
		if (begin == startingPoint)
			end = begin;
		else
		{
			while (isSeparator(*(--begin)))
				if (begin == startingPoint)
				{
					end = begin;
					return (*this);
				}
			end = begin + 1;
			while (!isSeparator(*(--begin)))
				if (begin == startingPoint)
					return (*this);
			begin++;
		}
		return (*this);
	}

	PlcText operator --(int)
	{
		PlcText temp = (*this);
		--(*this);
		return temp;
	}

	char* const& operator *()
	{
		int size = end - begin;
		word = new char[size + 1];
		int i = 0;
		for (char* p = begin; p != end; ++p)
			word[i++] = (*p);
		word[size] = '\0';
		return word;
	}

	PlcText(char* text, bool isBegin) : separators(" \t\n")
	{
		startingPoint = &text[0];
		int i = -1;
		while (isSeparator(text[++i]));
		begin = &text[i];
		while (!isSeparator(text[++i]) && text[i] != '\0');
		end = &text[i];

		if (!isBegin)
		{
			while (begin != end)
			{
				++(*this);
			}
		}
	}

	bool operator == (PlcText const& it) const
	{
		return (begin == it.begin && end == it.end && startingPoint == it.startingPoint);
	}
	bool operator != (PlcText const& it) const
	{
		return (begin != it.begin || end != it.end || startingPoint != it.startingPoint);
	}

private:
	char* begin;
	char* end;
	char* word;
	char* separators;
	char* startingPoint;

	bool isSeparator(char ch)
	{
		int i = 0;
		while (separators[i] != '\0')
		{
			if (ch == separators[i])
				return true;
			++i;
		}
		return false;
	}
};
