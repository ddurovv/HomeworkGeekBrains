#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <optional>

struct Person {
	std::string FirstName;
	std::optional<std::string> OptMiddleName;
	std::string LastName;
	std::string print() const { //через функцию
		std::ostringstream os;
		if (OptMiddleName.has_value())
			os << LastName << "\t" << FirstName << "\t" << OptMiddleName.value() << "\t";
		else os << LastName << "\t" << FirstName << "\t-\t";
		return os.str();
	}
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
	std::string print() const { //з функцию
		std::ostringstream os;
	if (AdditionalNumber.has_value())
			os << "+" << CountryCode << "(" << CityCode << ")" << Number << " " << AdditionalNumber.value();
		else os << "+" << CountryCode << "(" << CityCode << ")" << Number;
		return os.str();
	}
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
		std::make_tuple(num2.CountryCode, num2.CityCode, num2.Number, num2.AdditionalNumber.value_or(" "));
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
				return LeftPerson.first < RightPerson.first;
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
		auto retrn = std::make_tuple(std::string{ "" }, PhoneAnswer); //почти пустая инициализация
		if (count > 1) {
			return retrn = std::make_tuple(std::string{ "found more than 1" }, PhoneAnswer);
		}
		if (count == 1) {
			return retrn = std::make_tuple(std::string{ 0 }, PhoneAnswer);
		}
		return retrn = std::make_tuple("not found", PhoneAnswer);
	}

	void ChangePhoneNumber(const Person& p, PhoneNumber n) {
		for_each(HumanNumbers.begin(), HumanNumbers.end(),
			[&](auto& HumanPers)
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