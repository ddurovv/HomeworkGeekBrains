#include <iostream>
#include <list>
#include <algorithm>
#include <array>
#include <random>
#include <iomanip>

//task 1
template <typename T>
void Average_Push_Back(std::list<T>& ls) {
	try {
		T sum = 0;
		for (auto numbers : ls) {
			sum += numbers;
		}
		ls.push_back(sum / ls.size());
	}
	catch (...) { //понял, что забыл как написать сравнение Т с символьными типами, и через throw выбросить исключение
		std::cerr << "\tType should be real number\n";
	}
}

template <typename T>
void PrintList(std::list<T>& ls){ //пытался сделать константным но ide не разрешил (квалификатор типа не разрешён на функции не элементам)
	for (auto numbers : ls) {
		std::cout << numbers << ' ';
	}
	std::cout << "\n";
}

//task 2
class Matrix {
public:
	Matrix(const int size) : m_Size(size), m_Capacity(size * size) {
		Array = new double[m_Capacity];
		std::random_device rd;
		std::mt19937 mersenne(rd());                                   // x32-битное число
		std::uniform_real_distribution<double> urd(-10.0, 10.0);       // диапазон вещественных чисел от -10 до 10
		for (size_t i = 0; i < m_Capacity; ++i)
		{
			*(Array + i) = round(urd(mersenne) * 10) / 10;
		}

		m_Determinant = CalculateDeterminant((*this));
	}
	void print() const {
		std::cout << '[';

		for (size_t i = 0; i < m_Capacity;)
		{
			std::cout << std::setw(5) << *(Array + i++); //setw - параметр ширины потока, удобная хрень

			if (!(i % m_Size)) {
				std::cout << " ]\n";
				if (i < m_Capacity)
				{
					std::cout << '[';
				}

			} else {
				std::cout << ',';
			}
		}
	}
	size_t getSize() const {
		return m_Size;
	}
	double getDeterminant() const {
		return m_Determinant;
	}

	class Iter {
	public:
		Iter(double* ptr) : m_ptr(ptr) {}
		Iter& operator++() {
			++m_ptr;
			return *this;
		}
		bool operator!=(const Iter& iterat) {
			return m_ptr != iterat.m_ptr;
		}
		double& operator*() {
			return *m_ptr;
		}
		double& operator[](size_t i) {
			return *(m_ptr + i);
		}
	private:
		double* m_ptr;
	};

	Iter operator[](size_t i) {
		return Iter(Array + i * m_Size);
	}
	Iter begin() {
		return Iter(Array);
	}
	Iter end() {
		return Iter(Array + m_Capacity);
	}

	~Matrix() {
		//delete[] Array; // раскоментировать нельзя, потому что появляется исключение (Invalid address specified to RtlValidateHeap(адрес, адрес))
						  // на форуме, знатоки расписали, что где-то какой-то указатель удаляется автоматически через старую команду free, а при вызове delete,
						  // компилятор пытается очистить уже очищенную память.
	}

private:
	size_t m_Size;
	size_t m_Capacity;
	double* Array;
	double m_Determinant{ 0.0 };

	Matrix getModifiedMatrix(Matrix& arr, const size_t item){
		Matrix newArray(arr.getSize() - 1);
		size_t currentRow = 0;

		for (size_t i = 0; i < arr.getSize(); ++i)
		{
			if (item == i)
			{
				currentRow = i;
				continue;
			}

			for (size_t j = 0; j < newArray.getSize(); ++j)
			{
				newArray[currentRow][j] = arr[i][j + 1];
			}

			++currentRow;
		}

		return newArray;
	}
	double CalculateDeterminant(Matrix& arr) {
		double determinant = 0;

		if (arr.getSize() == 1)
		{
			determinant = arr[0][0];
		}
		else if (arr.getSize() == 2)
		{
			determinant = arr[0][0] * arr[1][1] - arr[1][0] * arr[0][1];
		}
		else
		{
			int sign = 1;

			for (size_t i = 0; i < arr.getSize(); ++i)
			{
				Matrix newArray = getModifiedMatrix(arr, i);
				determinant += sign * arr[i][0] * CalculateDeterminant(newArray);
				sign = -sign;
			}
		}

		return determinant;
	}
};



int main()
{
	{
		std::cout << "\t\tTask 1\n";
		std::list<float> TestList{ 1.1, 2.2, 3.3, 4.4, 5.5 };
		PrintList(TestList);

		Average_Push_Back(TestList);
		PrintList(TestList);
	}
	{
		std::cout << "\n\t\tTask2\n";
		for (size_t i = 1; i < 6; ++i)
		{
			std::cout << "Matrix " << i << " x " << i << ":\n";
			Matrix m(i);
			m.print();
			std::cout << "Determinant of matrix: " << std::setprecision(10) << m.getDeterminant() << "\n\n";
		}
	} // самое трудоёмкое задание, которое я когда-либо делал. Можно было проще, но я не понял как. Половина кода тупо спилена с формумов и работает благодаря 
	  // методу проб и ошибок. Не знаю стоит ли того разработка игр. Такие задания, скорее всего, для С++ группы. фу

}