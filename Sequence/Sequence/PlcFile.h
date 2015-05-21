//----------------------------PlcFile.h----------------------------------------
#ifndef PLC_FILE_H
#define PLC_FILE_H

#include <fstream>
#include <sstream>
#include "PlcString.h"
using namespace std;

template <typename Record>
class PlcFile
{
public:
	PlcFile(ifstream &data) : plc("", "")
	{
		init(data, true);
	}

	PlcFile(ifstream &data, bool isBegin) : plc("", "")
	{
		init(data, isBegin);
	}
	
	PlcFile& operator ++()
	{
		++plc;
		++currentNumber;
		return (*this);
	}

	PlcFile operator ++(int)
	{
		PlcFile plcFile = *this;
		++plc;
		return plcFile;
	}

	PlcFile& operator --()
	{
		--plc;
		--currentNumber;
		return (*this);
	}

	PlcFile operator --(int)
	{
		PlcFile plcFile = *this;
		--plc;
		return plcFile;
	}

	Record operator * ()
	{
		return Record(*plc);
	}

	bool operator == (PlcFile const& it) const
	{
		return (!strcmp(text, it.text) && currentNumber == it.currentNumber);
	}

	bool operator != (PlcFile const& it) const
	{
		return (strcmp(text, it.text) || currentNumber != it.currentNumber);
	}

	void setSeparators(char* sep)
	{
		plc.setSeparators(sep);
	}

private:
	void init(ifstream& data, bool isBegin)
	{
		data.seekg(0, ios::end);
		int size = data.tellg();
		data.seekg(0);
		text = new char[size];
		for (int i = 0; i < size; ++i)
		{
			data.seekg(i);
			text[i] = (char)data.peek();
		}
		plc = PlcString<char*>(text, "\n");
		setCurrentNumber(isBegin);
	}

	void setCurrentNumber(bool isBegin)
	{
		currentNumber = 0;
		if (!isBegin)
			while (strcmp(*plc, "\0"))
			{
				currentNumber++;
				plc++;
			}
	}

	PlcString<char*> plc;
	char* text;
	int currentNumber;
};

#endif