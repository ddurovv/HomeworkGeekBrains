#include <iostream>
#include <algorithm>
#include <execution>
#include <string>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <vector>

//task1
namespace Global {
	static std::mutex mut_a;
}

class pcout {
public:
	pcout() : loc_g(Global::mut_a) {}
	
	template <typename T>
	pcout& operator<<(const T& data) {
		std::cout << data;
		return *this;
	}
private:
	std::lock_guard<std::mutex> loc_g;
};

void function(int i) {
	pcout p;
	p << "Start thread " << i << "\n";
	p << "Stop thread " << i << "\n";
}
//task2
class Timer {
public: 
	Timer(std::chrono::system_clock::time_point start): m_start(start){}
	~Timer() {
		const auto end = std::chrono::system_clock::now();
		std::cout << "\n" << std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count() << " milliseconds\n";
	}
private:
	std::chrono::system_clock::time_point m_start;
};

bool simple_numb(size_t num) {
	size_t check = num / 2;
	if (num > 2)
	{
		for (size_t i = 2; i <= check; ++i) {
			if (num % i == 0)
			{
				return false;
			}
		}
	} return true;
}
//task3
void generate_things(std::vector<int>& v)
{
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<int> urd(0, 1000);

	std::lock_guard lg(Global::mut_a);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::generate(v.begin(), v.end(), [&]()
		{
			return urd(mersenne);
		});
}
void pop_thing(std::vector<int>& v)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << *std::max_element(std::execution::par, v.begin(), v.end()) << std::endl;
}

int main()
{
	{
		std::cout << "\t\tTask 1\n";
		std::thread t1(function, 1);
		std::thread t2(function, 2);
		std::thread t3(function, 3);
		t1.join();
		t2.join();
		t3.join();
	} 
	{
		std::cout << "\n\t\tTask 2\n";
		size_t max = 10000;
		size_t max_threads = std::thread::hardware_concurrency();
		std::vector<size_t> answers;
		std::mutex m;

		{
			Timer threads_timer(std::chrono::system_clock::now());

			for (size_t i = 0; i < max; )
			{
				std::vector<std::thread> vec;

				for (size_t j = i + max_threads; i <= j; ++i)
				{
					vec.push_back(std::thread([=, &m, &answers]()
						{
							if (simple_numb(i))
							{
								std::scoped_lock lg(m);
								answers.push_back(i);
							}
						}));
				}

				for (auto& t : vec)
				{
					t.join();
				}
			}
		}
		answers.clear();
		answers.shrink_to_fit(); //уменьшение количества памяти

		{
			Timer without_threads_timer(std::chrono::system_clock::now());

			for (size_t i = 0; i < max; ++i)
			{
				if (simple_numb(i))
				{
					answers.push_back(i);
				}
			}
		}
	} 
	{
		std::cout << "\n\t\tTask3\n";
		std::vector<int> v(100);
		int count = 100u;
		std::timed_mutex;

		while (count--)
		{
			std::thread owner([&]()
				{
					generate_things(v);
				});
			std::thread thief([&]()
				{
					pop_thing(v);
				});

			owner.join();
			thief.join();
		}
	}
}