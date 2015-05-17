#include <iostream>
using namespace std;

template <typename T>
class PlcMathSequence
{
    public:
        PlcMathSequence(int numOfStartingValues, T* startingValues)
        {
            num = numOfStartingValues;
            currentValues = new T[numOfStartingValues];
            for(int i=0; i<numOfStartingValues; ++i)
            {
                currentValues[i] = startingValues[i];
            }
        }
    
    T const& operator *()
    {
        return currentValues[num-1];
    }
    
    protected:
        int num;
        T* currentValues;    
};

template <typename T>
class PlcFibonacci: public PlcMathSequence<T>
{
    public:
        PlcFibonacci(T* startingValues): PlcMathSequence<T>(2, startingValues) {}
    
    PlcFibonacci& operator ++()
    {
        T temp = this.currentValues[0];
        this.currentValues[0] = this.currentValues[1];
        currentValues[1] = temp + currentValues[0];
        return (*this);
    }
    
    PlcFibonacci operator ++(int)
    {
        PlcFibonacci temp = (*this);
        ++(*this);
        return temp;
    }
    
    PlcFibonacci& operator --()
    {
        T temp = currentValues[1];
        currentValues[1] = currentValues[0];
        currentValues[0] = temp - currentValues[1];
        return (*this);
    }

    PlcFibonacci operator --(int)
    {
        PlcFibonacci temp = (*this);
        --(*this);
        return temp;
    }
};

int main()
{
    int* start = new int[2];
    start[0] = 0;
    start[1] = 1;
    PlcFibonacci<int> plc(start);
    for(int i=0; i<10; ++i)
    {
        cout << *(plc) << endl;
        plc++;
    }
    system("pause");
    return 0;
}
