#include <iostream>
#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <any>
#include <optional>
#include <fstream>
#include <algorithm>

struct Person {
	std::string FirstName;
	std::optional<std::string> OptMiddleName;
	std::string LastName;
};

std::ostream& operator<<(std::ostream& os, const Person& pers)
{
	if (pers.OptMiddleName.has_value())
	os << pers.LastName << "\t" << pers.FirstName << "\t" << pers.OptMiddleName.value() << "\t";
	else os << pers.LastName << "\t" << pers.FirstName << "\t-\t";
	return os;
}

bool operator< (const Person& pers1, const Person& pers2) {
			return std::make_tuple(pers1.LastName, pers1.FirstName, pers1.OptMiddleName.value_or(".")) <
			std::make_tuple(pers2.LastName, pers2.FirstName, pers2.OptMiddleName.value_or("."));
}

bool operator== (const Person& pers1, const Person& pers2) {
			return std::make_tuple(pers1.LastName, pers1.FirstName, pers1.OptMiddleName.value_or(".")) ==
			std::make_tuple(pers2.LastName, pers2.FirstName, pers2.OptMiddleName.value_or("."));

}

struct PhoneNumber {
	unsigned short CountryCode;
	unsigned short CityCode;
	std::string Number;
	std::optional<std::string> AdditionalNumber;
};
std::ostream& operator<<(std::ostream& os, const PhoneNumber& phone)
{
	if (phone.AdditionalNumber.has_value())
	os << "+" << phone.CountryCode << "(" << phone.CityCode << ")" << phone.Number << " " << phone.AdditionalNumber.value();
	else os << "+" << phone.CountryCode << "(" << phone.CityCode << ")" << phone.Number;
	return os;
}

bool operator< (const PhoneNumber& num1, const PhoneNumber& num2) {
	return std::make_tuple(num1.CountryCode, num1.CityCode, num1.Number, num1.AdditionalNumber.value_or(" ")) <
		std::make_tuple(num1.CountryCode, num1.CityCode, num1.Number, num1.AdditionalNumber.value_or(" "));
}

bool operator== (const PhoneNumber& num1, const PhoneNumber& num2) {
	return std::make_tuple(num1.CountryCode, num1.CityCode, num1.Number, num1.AdditionalNumber.value_or("/")) ==
		std::make_tuple(num2.CountryCode, num2.CityCode, num2.Number, num2.AdditionalNumber.value_or("/"));
}

class PhoneBook {
private:
	std::vector<std::pair<Person, PhoneNumber>> HumanNumbers;
public:
	PhoneBook(std::ifstream& ifs) {
		if (ifs.is_open())
		{
			std::string	LastName;
			std::string	FirstName;
			std::string	MiddleName;

			unsigned short CountryCode;
			unsigned short CityCode;
			std::string Number;
			std::string AddictionNumber;

			while (!ifs.eof())
			{
				ifs >> LastName >> FirstName >> MiddleName >> CountryCode >> CityCode >> Number >> AddictionNumber;
				std::optional<std::string> OptMiddleName;
				if (MiddleName != "-")
					OptMiddleName = MiddleName;
				std::optional<std::string> Add;
				if (AddictionNumber != "-")
					Add = AddictionNumber;
				HumanNumbers.emplace_back(Person{ FirstName , OptMiddleName, LastName }, PhoneNumber{ CountryCode, CityCode, Number, Add });
			}
			ifs.close();
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const PhoneBook& book);

	void SortByPhone() {
		std::sort(HumanNumbers.begin(), HumanNumbers.end(), 
			[](const auto& LeftPerson, const auto& RightPerson)->bool {
				return LeftPerson.second < RightPerson.second;
			});
	}
	void SortByName() {
		std::sort(HumanNumbers.begin(), HumanNumbers.end(),
			[](const auto& LeftPerson, const auto& RightPerson)->bool {
				if (LeftPerson.first.LastName == RightPerson.first.LastName)
				{
					if (LeftPerson.first.FirstName == RightPerson.first.FirstName)
					{
						return LeftPerson.first.OptMiddleName < RightPerson.first.OptMiddleName;
					}
					return LeftPerson.first.FirstName < RightPerson.first.FirstName;
				}
				return LeftPerson.first.LastName < RightPerson.first.LastName;
			});
	}
	std::tuple<std::string, PhoneNumber>& GetPhoneNumber(const std::string& target) {
		int count = 0u;
		PhoneNumber PhoneAnswer;
		for_each(HumanNumbers.begin(), HumanNumbers.end(),
			[target, &count, &PhoneAnswer](const auto& HumanPers)
			{
				if (HumanPers.first.LastName == target)
				{
					count++;
					PhoneAnswer = HumanPers.second;
				}
			});
		if (count > 1) {
			return std::make_tuple(std::string{ "found more than 1" }, PhoneAnswer);
		}		else if (count == 1) {
			return std::make_tuple(std::string{ 0 }, PhoneAnswer);
				} else return std::make_tuple("not found", PhoneAnswer);
	}

	void ChangePhoneNumber(const Person& p, PhoneNumber n) {
		for_each(HumanNumbers.begin(), HumanNumbers.end(),
			[&](auto&& HumanPers)
			{
				if (HumanPers.first == p)
				{
					HumanPers.second = n;
				/*	HumanPers.second.AddictionNumber = n.second.AddictionNumber;
					HumanPers.second.CountryCode = n.second.CountryCode;
					HumanPers.second.CityCode = n.second.CityCode;
					HumanPers.second.Number = n.second.number;
					*/
				}
			});
	}
};
std::ostream& operator<< (std::ostream& os, const PhoneBook& book) {
	for (std::pair<Person, PhoneNumber> HN : book.HumanNumbers) {
		os << HN.first << "|||" << HN.second << "\n";
	}
	return os;
}

int main()
{
	{
		std::cout << "\t\tTask1\n\n";
		Person p1{ "Ivanchenko", "Ivan", "Asphaltovich" }, p2{ "Sobolev", "Buldar" }, p3{ "Mukhtar", "Pupa", "Apelsinovich" };
		std::cout << std::boolalpha << "Person: " << p1 << " is more than " << p2 << "?\nAnswer: " << bool(p2 < p1) <<
			"\nPerson: " << p1 << "is equal " << p3 << "?\nAnswer: " << bool(p1 == p3) << "\n";
		
	}
	{
		std::cout << "\t\tTask2\n\n";
		PhoneNumber number1{ 7, 911, "1234567", "10"};
		PhoneNumber number2{ 7, 911, "1234565"};
		PhoneNumber number3{ 7, 952, "1234567", "11"};
		PhoneNumber number4{ 7, 952, "1234532", "10" };
		std::cout << "Compare number3(" << number3 <<"), number4(" << number4 << ") - number4 is more ? \nAnswer : " << bool(number3 < number4) <<
			"\nIs number1(" << number1 << ") equal to number2(" << number2 << ")?\nAnswer: " << bool(number1 == number2);
	}
	{
		std::cout << "\n\t\tTask3\n\n";
		std::ifstream file("MyFile.txt"); // путь к файлу PhoneBook.txt
		PhoneBook book(file);
		std::cout << book;

		std::cout << "------SortByPhone-------" << std::endl;
		book.SortByPhone();
		std::cout << book;

		std::cout << "------SortByName--------" << std::endl;
		book.SortByName();
		std::cout << book;

		std::cout << "-----GettPhoneNumber-----" << std::endl;
		// лямбда функция, которая принимает фамилию и выводит номер телефона
		//этого человека, либо строку с ошибкой
		auto print_phone_number = [&book](const std::string& surname) {
			std::cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (std::get<0>(answer).empty())
			std::cout << std::get<1>(answer);
		else
			std::cout << std::get<0>(answer);
		std::cout << std::endl;
	}; 
		// вызовы лямбды
		print_phone_number("Ivanov");
		print_phone_number("Mukhtar");

		/*std::cout << "----ChangePhoneNumber----" << std::endl;
		book.ChangePhoneNumber(Person{ "Ivanchenko", "Ivan", "Asphaltovich"},
		PhoneNumber{ 7, 123, "15344458", "10"});
		book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
		PhoneNumber{ 16, 465, "9155448", std::nullopt});
		std::cout << book;*/
	}
}