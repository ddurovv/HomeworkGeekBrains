#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string_view>

template<typename T>
void Swap(T* a, T* b)//для одного и того же типа
{
    T temp = *a;
    *a = *b;
    *b = temp;
}
template<typename T>
void SortPointers(std::vector<T*>& v)
{
    sort(v.begin(), v.end(), [](const auto& v1, const auto& v2)
        {   
            return *v1 < *v2;
        });
}

class Timer {
    //RAII
public:
    explicit Timer(std::chrono::system_clock::time_point start) : m_start(start) {}
    ~Timer() {
        const auto end = std::chrono::system_clock::now();
        std::cout << "\n" << std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count() << std::endl;
    }
private:
    std::chrono::system_clock::time_point m_start;
};

namespace counting
{
    const std::string_view vowels{ "AEIOUaeiou" };

    void method1(const std::string_view& s)
    {
        Timer timer(std::chrono::system_clock::now());
        size_t count = count_if(s.begin(), s.end(), [&](const auto& WarAndPeace)
            {
                return vowels.find(WarAndPeace) != std::string::npos;
            });
        std::cout << count;
    }

    void method2(const std::string_view& s)
    {
        Timer timer(std::chrono::system_clock::now());
        size_t count = count_if(s.begin(), s.end(), [&](const auto& WarAndPeace)
            {
                for (size_t i = 0; i < vowels.size(); ++i)
                {
                    if (vowels[i] == WarAndPeace)
                        return true;
                }
                return false;
            });
        std::cout << count;
    }

    void method3(const std::string_view& s)
    {
        size_t count = 0;
        Timer timer(std::chrono::system_clock::now());
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (vowels.find(s[i]) != std::string::npos)
            {
                ++count;
            }
        }
        std::cout << count;
    }

    void method4(const std::string_view& s)
    {
        size_t count = 0;
        Timer timer(std::chrono::system_clock::now());
        for (size_t i = 0; i < s.size(); ++i)
        {
            for (size_t j = 0; j < vowels.size(); ++j)
            {
                if (vowels[j] == s[i])
                    ++count;
            }
        }
        std::cout << count;
    }
}

int main()
{
    { 
        std::cout << "\t\tTask 1\n";
        int a(5);
        int b(6);

        Swap(&a, &b);

        std::cout << a << " " << b << "\n";
    }
    {
        std::cout << "\n\n\t\tTask 2\n";
        int length = 5;
        std::vector<int *> v;

        for (int i = 0; i < length; ++i)
        {
            int* a = new int;
            *a = rand() % 10;
            v.push_back(a);
        }

        for (int i = 0; i < length; ++i)
        {
            std::cout << *v[i] << " ";
        }

        std::cout << "\n";

        SortPointers(v);

        for (int i = 0; i < length; ++i)
        {
            std::cout << *v[i] << " ";
        }

        delete[] &v; //правильно? просто v не хочет удалять (выражение должно представлять собой указатель на полный тип объекта)
    }
    {
        std::cout << "\n\n\t\tTask 3\n";
        std::ifstream file("War_and_peace.txt");
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0);
        std::string s(size, ' ');
        file.read(&s[0], size);

        {counting::method1(s);
        }
        {counting::method2(s);
        }
        {counting::method3(s);
        }
        {counting::method4(s);
        }
    }
}