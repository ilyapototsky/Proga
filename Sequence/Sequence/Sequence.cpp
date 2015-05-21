//----------------------------Sequence.cpp-------------------------------------
#include <iostream>
#include <fstream>
#include "PlcBinaryFile.h"
#include "PlcFibonacci.h"
#include "BinaryFileRecord.h"
#include "PlcFile.h"
#include "Record.h"
using namespace std;

template <template <typename> class Policy, typename ElementType, typename InputType>
class Sequence
{
public:
	Sequence(InputType _input) : input(_input), plc(_input) {}

	typedef Policy<ElementType> iterator;
	
	iterator begin() { return iterator(input, true); }
	iterator end() { return iterator(input, false); }

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
	iterator plc;
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
//---------------------������������� ��������� PlcString-----------------------
	
	//������������� ������������������ ������������ ����� � ������
	Sequence<PlcString, double, char*> seqStrDouble("1 2 3 3.5");
	//����� ����� ��������� ������ ������������������
	cout << count(seqStrDouble) << endl;

	cout << endl;

	//������������� ������������������ ���� � ������
	Sequence<PlcString, char*, char*> seqStrChar("My name is Ilya");
	//����� ����� ��������� ������ ������������������
	cout << count(seqStrDouble) << endl;
	//�������� �� ���� ��������� ������������������ � ������� ������� �� �����
	Sequence<PlcString, char*, char*>::iterator beg = seqStrChar.begin(),
		end = seqStrChar.end();
	for (; beg != end; ++beg)
	{
		cout << *beg << "; ";
	}
	cout << endl;

	cout << endl;

//---------------------������������� ��������� PlcBinaryFile-------------------

	//������������� ������ ��������� �����
	BinaryFileRecord r(10, "hello", 1.5);

	//������������ ��������� ����� ��� ����������� ������������
	ofstream fout("input.txt", ios::binary);
	fout.write((char*)&r, sizeof(r));
	strcpy_s(r.cField, "hi");
	fout.write((char*)&r, sizeof(r));
	fout.close();

	//������������� ������������������ ������� BinaryFileRecord � �������� �����
	ifstream fin("input.txt", ios::binary);
	Sequence<PlcBinaryFile, BinaryFileRecord, ifstream&> seqBin(fin);
	//����� ����� ��������� ������ ������������������
	cout << count(seqBin) << endl;
	//����� ������ ���� ������� (� ������ ������� �� ����� 2)
	cout << seqBin.getCurrentElement() << endl;
	seqBin.next();
	cout << seqBin.getCurrentElement() << endl;

	cout << endl;

//----------------------������������� ��������� PlcFile	-----------------------
	
	//������������� ������������������ ������� Record � �����
	ifstream file("data.txt");
	Sequence<PlcFile, Record, ifstream&> seqFile(file);
	//����� ����� ��������� ������ ������������������
	cout << count(seqFile) << endl;
	//����� ���� ������ ��������� ������������������
	cout << seqFile.getCurrentElement() << endl;
	seqFile.next();
	cout << seqFile.getCurrentElement() << endl;
	seqFile.next();
	cout << seqFile.getCurrentElement() << endl;
	
	cout << endl;

//--------------------������������� ��������� PlcFibonacci---------------------

	//������������� ������������������ ���������
	int* start = new int[2];
	start[0] = 0;
	start[1] = 1;
	Sequence<PlcFibonacci, int, int*> seqFib(start);
	//����� ���������� ��������� ������ ������������������
	//��� ����������� �������������� ������������������ ��������� 0
	cout << count(seqFib) << endl;

	cout << endl;

	//����� ������ ������ ������ ������������������ ���������
	Sequence<PlcFibonacci, int, int*>::iterator beginIt = seqFib.begin(),
		endIt = seqFib.end();
	endIt.setLast(10);
	for (; beginIt != endIt; ++beginIt)
	{
		cout << *beginIt << endl;
	}

	system("pause");
	return 0;
}
