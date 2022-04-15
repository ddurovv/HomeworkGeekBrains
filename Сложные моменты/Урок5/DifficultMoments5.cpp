#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <string>
#include <deque>
#include <queue>
#include <unordered_set>

template <typename T>
void make_unique_words(T it_begin, T it_end) {
	std::unordered_set<std::string> unique_words(it_begin, it_end);
	std::copy(unique_words.begin(), unique_words.end(), std::ostream_iterator<std::string>(std::cout, "| "));
}

std::string get_sentence(std::string& s)
{
    std::string punctuation_marks{ ".?!" };  // разделительные знаки пунктуации
    std::string temp;                        // найденное предложение

    for (const char& c : s)
    {
        temp.push_back(c); //посимвольно добавляем данные в предложение

        if (punctuation_marks.find(c) != std::string::npos)
        {
            s.erase(0, temp.size());
            if (temp[0] == ' ') { temp.erase(0, 1); }
            return temp;
        }
    }

    return {}; // иначе возвращаем пустой объект
}

int main()
{
	{
		std::cout << "\t\tTask1\n";
		std::vector<std::string> vec{ "Hydrogen", "Helium", "Helium", "Carbon", "Lithium", "Carbon", "Neon", "Argon", "Nitrogen", "Iron", "Neodymium", "Iron" };
		std::cout << "Print vector:\n";
		make_unique_words(vec.begin(), vec.end());

		std::list<std::string> lit(vec.begin(), vec.end());
		std::cout << "\nPrint list:\n";
		make_unique_words(lit.begin(), lit.end());

		std::deque<std::string> dec(vec.begin(), vec.end());
		std::cout << "\nPrint deque:\n";
		make_unique_words(dec.begin(), dec.end());
	}
	{
		std::cout << "\n\t\tTask2\n";
        std::unordered_set<std::string, std::hash<std::string>> sentences;
        std::string current_string, temp_string;

        while (std::getline(std::cin, current_string))
        {
            if (!current_string.empty())                                       // пропускаем пустую строку, если она есть
            {
                temp_string += current_string + ' ';                            

                while (true)
                {
                    std::string sentence(std::move(get_sentence(temp_string)));
                    if (sentence.empty())
                    {
                        break;
                    }
                    sentences.insert(std::move(sentence));
                }
            }
        }

        std::priority_queue<std::pair<size_t, std::string>> q;
        for (const auto& sentence : sentences)
        {
            q.push({ sentence.size(), sentence });
        }
        while (!q.empty())
        {
            std::cout << q.top().first << ": " << q.top().second << '\n';
            q.pop();
        }
	}
}