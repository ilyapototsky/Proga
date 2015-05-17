#include <iostream>
#include <fstream>
#include <string>
#include "PlcBinaryFile.h"
#include "PlcFibonacci.h"
#include "tempRecord.h"
//#include "PlcString.h"
#include "PlcFile.h"
#include "Record.h"
using namespace std;

template <template <typename> class Policy, typename ElementType, typename InputType>
class Sequence
{
public:
	Sequence(InputType _input) : input(_input), plc(_input) {}

	typedef Policy<ElementType> iterator;
	//friend class Policy;

	iterator begin() { return Policy<ElementType>(input, true); }
	iterator end() { return Policy<ElementType>(input, false); }

	//pair<Policy, Policy> equal_range()

	void next()
	{
		++plc;
	}

	void previous()
	{
		--plc;
	}

	ElementType getCurrentElement()
	{
		return *plc;
	}

private:
	InputType input;
	Policy<ElementType> plc;
};

template <typename I, typename Elem, template <typename> class Plc>
int count(Sequence<Plc, Elem, I> seq)
{
	int result = 0;
	Sequence<Plc, Elem, I>::iterator beginIt = seq.begin();
	Sequence<Plc, Elem, I>::iterator endIt = seq.end();
	for (; beginIt != endIt; ++beginIt)
	{
		result++;
	}
	return result;
}


int main()
{
	//Sequence<PlcString, double, char*> seq("1 2 3 3.5");
	//cout << count(seq) << endl;
	//
	//int* start = new int[2];
	//start[0] = 0;
	//start[1] = 1;
	//Sequence<PlcFibonacci, int, int*> sfib(start);
	//cout << count(sfib) << endl;

	//PlcFibonacci<int> beginIt = sfib.begin();
	//PlcFibonacci<int> endIt = sfib.end();
	//endIt.setLast(10);
	//for (; beginIt != endIt; ++beginIt)
	//{
	//	cout << *beginIt << endl;
	//}

	//BinaryFileRecord r(10, "hello", 1.5);

	//ofstream fout("input.txt", ios::binary);
	//fout.write((char*)&r, sizeof(r));

	//strcpy_s(r.cField, "hi");
	//fout.write((char*)&r, sizeof(r));
	//fout.close();

	//ifstream fin("input.txt", ios::binary);
	//PlcBinaryFile<BinaryFileRecord> plc(fin);

	//Sequence<PlcBinaryFile, BinaryFileRecord, ifstream&> seqBin(fin);
	//cout << count(seqBin) << endl;

	ifstream file("data.txt");
	Sequence<PlcFile, Record, ifstream&> seq2(file);
	cout << count(seq2) << endl;
	system("pause");
	return 0;
}
