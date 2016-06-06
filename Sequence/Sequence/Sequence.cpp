//----------------------------Sequence.cpp-------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include "PlcBinaryFile.h"
#include "PlcFibonacci.h"
#include "BinaryFileRecord.h"
#include "PlcFile.h"
#include "Record.h"

#include "PlcRegexString.h"
using namespace std;

template <template <typename> class Policy, typename ElementType, typename InputType>
class Sequence {
public:
	Sequence(InputType _input): input(_input), plc(_input) {}

	typedef Policy<ElementType> iterator;

	iterator begin() { return iterator(input, true); }
	iterator end() { return iterator(input, false); }

	void next() {
		bool found = true;
		do {
			++plc;
			found = true;
			for (int i = 0; i < lambdas.size(); ++i) {
				if (!lambdas[i](*plc)) {
					found = false;
					break;
				}
			}
		} while (!found);
	}

	ElementType get() {
		return *plc;
	}

	Sequence& filter(function<bool(ElementType)> fn) {
		lambdas.push_back(fn);
		return *this;
	}

	Sequence& limit(int &n) {
		//static int n = _n;
		lambdas.push_back([&n](ElementType elem) {
			if (n > 0) {
				--n;
				return true;
			} else {
				return false;
			}
		});
		return *this;
	}
	
	void print(char* delim = "\n") {
		runLambdas([delim](ElementType elem) {
			cout << elem << delim;
		});
	}

	int count() {
		int count = 0;
		runLambdas([&count](ElementType elem) {
			count++;
		});
		return count;
	}

	iterator& getPolicy() { return plc; }

private:
	InputType input;
	iterator plc;
	vector <function<bool(ElementType)> > lambdas;
	void runLambdas(function<void(ElementType)> fn) {
		auto curr = plc;
		auto end = this->end();
		ElementType temp;
		bool clear = true;
		while (curr != end) {
			temp = *curr;
			for (int i = 0; i < lambdas.size(); ++i) {
				if (!lambdas[i](temp)) {
					clear = false;
					break;
				}
			}
			if (clear)
				fn(temp);
			clear = true;
			++curr;
		}
	}
};

int main() {
//---------------------������������� ��������� PlcString-----------------------
	//������������� ������������������ ������������ ����� � ������
	Sequence<PlcString, double, char*> seqStrDouble("1 2 3.5");
	//����� ���� �����, ������� 3
	seqStrDouble.filter([](double n) {return n < 3; }).print(", ");
	cout << "\n\n";

	//������������� ������������������ ���� � ������
	Sequence<PlcString, char*, char*> seqStrChar("Hello, my name is Ilya");
	//����� ��������� ������ ������������������ �� ������������ �������������
	seqStrChar.print("; ");
	cout << endl;
	//��������� ������������ ", " � ����� ���������� ������ ������������������
	seqStrChar.getPolicy().setSeparators(", ");
	seqStrChar.print("; ");
	cout << "\n\n";

//---------------------������������� ��������� PlcBinaryFile-------------------
	//������������� ������ ��������� �����
	BinaryFileRecord r(10, "hello", 1.5);

	//������������ ��������� ����� ��� ����������� ������������
	ofstream fout("input.txt", ios::binary);
	fout.write((char*)&r, sizeof(r));
	strcpy_s(r.cField, "hi");
	fout.write((char*)&r, sizeof(r));
	strcpy_s(r.cField, "ni hao");
	fout.write((char*)&r, sizeof(r));
	fout.close();

	//������������� ������������������ ������� BinaryFileRecord � �������� �����
	ifstream fin("input.txt", ios::binary);
	Sequence<PlcBinaryFile, BinaryFileRecord, ifstream&> seqBin(fin);
	//����� ����� ��������� ������ ������������������
	cout << seqBin.count() << endl;
	//����� ������ ���� �������
	int n = 2;
	seqBin.limit(n).print();
	cout << endl;

//--------------------������������� ��������� PlcFibonacci---------------------
	//������������� ������������������ ���������
	int* start = new int[2];
	start[0] = 0;
	start[1] = 1;
	Sequence<PlcFibonacci, int, int*> seqFib(start);
	//����� ���������� ��������� ������ ������������������
	//��� ����������� �������������� ������������������� ��������� 0
	cout << seqFib.count() << endl;

	//����� ������ ������ ������ ������������������ ���������
	Sequence<PlcFibonacci, int, int*>::iterator beginIt = seqFib.begin(),
		endIt = seqFib.end();
	endIt.setLast(10);
	for (; beginIt != endIt; ++beginIt) {
		cout << *beginIt << " ";
	}
	cout << "\n\n";

//--------------�������������� ��������� PlcFile � PlcRegexString--------------
	//������������� ������������������ ������� Record � �����
	ifstream file("data.txt");
	Sequence<PlcFile, Record, ifstream&> seqFile(file);
	// ����� ���� ���������, ��������� � �����
	seqFile.print();
	cout << endl;
	// ���������� ��������� ��� ������� ��������, ���� ������� ������� � ���� info
	auto begin = seqFile.begin(), end = seqFile.end();
	for (; begin != end; ++begin) {
		cout << (*begin).surname << " " << (*begin).name << ": ";
		// ������������� ������������������ ��� ������ � regex �� ���� info
		Sequence<PlcRegexString, char*, char*> seqRegStr((*begin).info);
		seqRegStr.getPolicy().setRegex(R"(\(\d{2}\)\d{3}-\d{2}-\d{2})");
		seqRegStr.print(", ");
		cout << endl;
	}

	system("pause");
	return 0;
}
