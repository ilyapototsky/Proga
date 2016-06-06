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
//---------------------Использование стратегии PlcString-----------------------
	//Инициализация последовательности вещественных чисел в строке
	Sequence<PlcString, double, char*> seqStrDouble("1 2 3.5");
	//Вывод всех чисел, меньших 3
	seqStrDouble.filter([](double n) {return n < 3; }).print(", ");
	cout << "\n\n";

	//Инициализация последовательности слов в строке
	Sequence<PlcString, char*, char*> seqStrChar("Hello, my name is Ilya");
	//Вывод элементов данной последовательности со стандартными разделителями
	seqStrChar.print("; ");
	cout << endl;
	//Установка разделителей ", " и вывод полученных членов последовательности
	seqStrChar.getPolicy().setSeparators(", ");
	seqStrChar.print("; ");
	cout << "\n\n";

//---------------------Использование стратегии PlcBinaryFile-------------------
	//Инициализация записи бинарного файла
	BinaryFileRecord r(10, "hello", 1.5);

	//Формирование бинарного файла для последующей демонстрации
	ofstream fout("input.txt", ios::binary);
	fout.write((char*)&r, sizeof(r));
	strcpy_s(r.cField, "hi");
	fout.write((char*)&r, sizeof(r));
	strcpy_s(r.cField, "ni hao");
	fout.write((char*)&r, sizeof(r));
	fout.close();

	//Инициализация последовательности записей BinaryFileRecord в бинарном файле
	ifstream fin("input.txt", ios::binary);
	Sequence<PlcBinaryFile, BinaryFileRecord, ifstream&> seqBin(fin);
	//Вывод числа элементов данной последовательности
	cout << seqBin.count() << endl;
	//Вывод первых двух записей
	int n = 2;
	seqBin.limit(n).print();
	cout << endl;

//--------------------Использование стратегии PlcFibonacci---------------------
	//Инициализация последовательности Фибоначчи
	int* start = new int[2];
	start[0] = 0;
	start[1] = 1;
	Sequence<PlcFibonacci, int, int*> seqFib(start);
	//Вывод количества элементов данной последовательности
	//Для бесконечных математических последовательностей выведется 0
	cout << seqFib.count() << endl;

	//Вывод первых десяти членов последовательности Фибоначчи
	Sequence<PlcFibonacci, int, int*>::iterator beginIt = seqFib.begin(),
		endIt = seqFib.end();
	endIt.setLast(10);
	for (; beginIt != endIt; ++beginIt) {
		cout << *beginIt << " ";
	}
	cout << "\n\n";

//--------------Комбинирование стратегий PlcFile и PlcRegexString--------------
	//Инициализация последовательности записей Record в файле
	ifstream file("data.txt");
	Sequence<PlcFile, Record, ifstream&> seqFile(file);
	// Вывод всех студентов, указанных в файле
	seqFile.print();
	cout << endl;
	// Нахождение телефонов для каждого студента, если таковые указаны в поле info
	auto begin = seqFile.begin(), end = seqFile.end();
	for (; begin != end; ++begin) {
		cout << (*begin).surname << " " << (*begin).name << ": ";
		// Инициализация последовательности для работы с regex по полю info
		Sequence<PlcRegexString, char*, char*> seqRegStr((*begin).info);
		seqRegStr.getPolicy().setRegex(R"(\(\d{2}\)\d{3}-\d{2}-\d{2})");
		seqRegStr.print(", ");
		cout << endl;
	}

	system("pause");
	return 0;
}
