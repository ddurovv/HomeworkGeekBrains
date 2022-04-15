#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>
#include <random>

template <typename T>
void insert_sorted(std::vector<T>& vec, T target) {
	auto it = std::find_if(vec.begin(), vec.end(), [&](const T& element) { return element >= target; });
	vec.insert(it, target);
}

template <typename T>
void print(std::vector<T>& vec) {
	for (auto numbers : vec) {
		std::cout << numbers << ' ';
	}
	std::cout << "\n\n";
}



int main()
{
	{
		std::cout << "\t\tTask 1\n";
		
		std::vector<int> my_vector{2,5,6,3,7,4,8,9,0,1};
		std::sort(my_vector.begin(), my_vector.end(), [](int a, int b) { return a < b; });
		print(my_vector); 
		
		insert_sorted(my_vector, 10);
		print(my_vector); 
		insert_sorted(my_vector, 5);
		print(my_vector);
	}
	{
		std::cout << "\n\t\tTask 2\n";

		std::vector<double> signal(100);
		double error_DAC = 0;// digital to analog converter и наоборот. Короче ошибка ЦАП и АЦП.

		std::random_device rd;
		std::mt19937 mersenne(rd());
		std::uniform_real_distribution<double> urd(-1000.0, 1000.0);
		std::generate(signal.begin(), signal.end(), [&](){ return urd(mersenne); });
	
		print(signal);
		std::copy(signal.begin(), signal.end(), std::ostream_iterator<int> { std::cout, " " });
		std::cout << "\n\n";

		std::for_each(signal.begin(), signal.end(), [&](const double& it)
			{
				error_DAC += pow(it - static_cast<int>(it), 2);
			});

		std::cout << "Error of Digital-to-Analog Converter is " << error_DAC << "\n";
	}
}
