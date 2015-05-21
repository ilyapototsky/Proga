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
//---------------------Использование стратегии PlcString-----------------------
	
	//Инициализация последовательности вещественных чисел в строке
	Sequence<PlcString, double, char*> seqStrDouble("1 2 3 3.5");
	//Вывод числа элементов данной последовательности
	cout << count(seqStrDouble) << endl;

	cout << endl;

	//Инициализация последовательности слов в строке
	Sequence<PlcString, char*, char*> seqStrChar("My name is Ilya");
	//Вывод числа элементов данной последовательности
	cout << count(seqStrDouble) << endl;
	//Итерация по всем элементам последовательности с выводом каждого на экран
	Sequence<PlcString, char*, char*>::iterator beg = seqStrChar.begin(),
		end = seqStrChar.end();
	for (; beg != end; ++beg)
	{
		cout << *beg << "; ";
	}
	cout << endl;

	cout << endl;

//---------------------Использование стратегии PlcBinaryFile-------------------

	//Инициализация записи бинарного файла
	BinaryFileRecord r(10, "hello", 1.5);

	//Формирование бинарного файла для последующей демонстрации
	ofstream fout("input.txt", ios::binary);
	fout.write((char*)&r, sizeof(r));
	strcpy_s(r.cField, "hi");
	fout.write((char*)&r, sizeof(r));
	fout.close();

	//Инициализация последовательности записей BinaryFileRecord в бинарном файле
	ifstream fin("input.txt", ios::binary);
	Sequence<PlcBinaryFile, BinaryFileRecord, ifstream&> seqBin(fin);
	//Вывод числа элементов данной последовательности
	cout << count(seqBin) << endl;
	//Вывод первых двух записей (в данном примере их ровно 2)
	cout << seqBin.getCurrentElement() << endl;
	seqBin.next();
	cout << seqBin.getCurrentElement() << endl;

	cout << endl;

//----------------------Использование стратегии PlcFile	-----------------------
	
	//Инициализация последовательности записей Record в файле
	ifstream file("data.txt");
	Sequence<PlcFile, Record, ifstream&> seqFile(file);
	//Вывод числа элементов данной последовательности
	cout << count(seqFile) << endl;
	//Вывод трех первых элементов последовательности
	cout << seqFile.getCurrentElement() << endl;
	seqFile.next();
	cout << seqFile.getCurrentElement() << endl;
	seqFile.next();
	cout << seqFile.getCurrentElement() << endl;
	
	cout << endl;

//--------------------Использование стратегии PlcFibonacci---------------------

	//Инициализация последовательности Фибоначчи
	int* start = new int[2];
	start[0] = 0;
	start[1] = 1;
	Sequence<PlcFibonacci, int, int*> seqFib(start);
	//Вывод количества элементов данной последовательности
	//Для бесконечных математических послдовательностей выведется 0
	cout << count(seqFib) << endl;

	cout << endl;

	//Вывод первых десяти членов последовательности Фибоначчи
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
