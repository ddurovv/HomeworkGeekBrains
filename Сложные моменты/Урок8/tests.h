#pragma once
#include <gtest/gtest.h>
#include <optional>
#include "DifMom1.h"


class TestBook : public testing::Test {
protected:
	void SetUp() override {
		std::ifstream ss("TestFile.txt");
		testbook = new PhoneBook(ss);
	}
	void TearDown() override {
		delete testbook;
	}
	PhoneBook *testbook;
};

TEST(PersonTests, Equeals_true) {
	Person a("Abbabo", std::nullopt, "Abbabi");
	Person b("Abbabo", std::nullopt, "Abbabi");
	ASSERT_TRUE((a == b)
		==
		(std::make_tuple(a.LastName, a.FirstName, a.OptMiddleName.value_or(".")) == std::make_tuple(b.LastName, b.FirstName, b.OptMiddleName.value_or("."))));
}
TEST(PersonTests, Equeals_false) {
	Person a("Abbabo", std::nullopt, "Abbabi");
	Person b("Baabao", std::nullopt, "Baabai");
	ASSERT_FALSE((a == b)
		==
		(std::make_tuple(a.LastName, a.FirstName, a.OptMiddleName.value_or(".")) == std::make_tuple(b.LastName, b.FirstName, b.OptMiddleName.value_or("."))));
}
TEST(PhoneNumberTests, Equals_true) {
	PhoneNumber a(7, 911, "1234567", "10");
	PhoneNumber b(7, 911, "1234567", "10");
	ASSERT_TRUE((a == b) ==
		(std::make_tuple(a.CountryCode, a.CityCode, a.Number, a.AdditionalNumber.value_or("/")) ==
			std::make_tuple(b.CountryCode, b.CityCode, b.Number, b.AdditionalNumber.value_or("/"))));
}
TEST(PhoneNumberTests, Equals_false) {
	PhoneNumber a(7, 911, "1234567", "10");
	PhoneNumber b(7, 911, "7654321", "1");//другой номер
	ASSERT_FALSE((a == b) ==
		(std::make_tuple(a.CountryCode, a.CityCode, a.Number, a.AdditionalNumber.value_or("/")) ==
			std::make_tuple(b.CountryCode, b.CityCode, b.Number, b.AdditionalNumber.value_or("/"))));
}
TEST(PersonTests, less_true) {
	Person a("Abbabo", std::nullopt, "Abbabi");
	Person b("Baabao", std::nullopt, "Baabai");
	ASSERT_TRUE((a < b) ==
		(std::make_tuple(a.LastName, a.FirstName, a.OptMiddleName.value_or(".")) <
			std::make_tuple(b.LastName, b.FirstName, b.OptMiddleName.value_or("."))));
}
TEST(PersonTests, less_false) {
	Person a("Abbabo", std::nullopt, "Abbabi");
	Person b("Baabao", std::nullopt, "Baabai");
	ASSERT_FALSE((b < a) ==
		(std::make_tuple(b.LastName, b.FirstName, b.OptMiddleName.value_or(".")) <
			std::make_tuple(a.LastName, a.FirstName, a.OptMiddleName.value_or(".")))); //поменял местами a и b
}
TEST(PhoneNumberTests, less_true) {
	PhoneNumber a(7, 911, "1234567", "10");
	PhoneNumber b(8, 911, "1234567", "10");//8 в начале
	ASSERT_TRUE((a < b) ==
		(std::make_tuple(a.CountryCode, a.CityCode, a.Number, a.AdditionalNumber.value_or(" ")) <
			std::make_tuple(b.CountryCode, b.CityCode, b.Number, b.AdditionalNumber.value_or(" "))));
}
TEST(PhoneNumberTests, less_false) {
	PhoneNumber a(7, 911, "1234567", "10");
	PhoneNumber b(8, 911, "1234567", "10");
	ASSERT_FALSE((b < a) ==
		(std::make_tuple(b.CountryCode, b.CityCode, b.Number, b.AdditionalNumber.value_or(" ")) <
			std::make_tuple(a.CountryCode, a.CityCode, a.Number, a.AdditionalNumber.value_or(" "))));//поменял местами
}
TEST(PersonTests, Print) {
	Person TestPerson("Abb", std::nullopt, "Ann");

	ASSERT_STREQ(TestPerson.print().c_str(), "Ann\tAbb\t-\t");
}
TEST(PhoneNumberTests, Print) {
	PhoneNumber TestNumber(99, 999, "1234567", "9");

	ASSERT_STREQ(TestNumber.print().c_str(), "+99(999)1234567 9");
}
/////////////////////////разделить для порядка

TEST_F(TestBook, GetPhoneNumber_notFound) {
	std::string lastname = "Egorov";

	auto answer = testbook->GetPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "not found");
}
TEST_F(TestBook, GetPhoneNumber_foundOne)
{
	std::string lastname = "Ivanchenko";

	auto answer = testbook->GetPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "");
}
TEST_F(TestBook, GetPhoneNumber_foundMore)
{
	std::string lastname = "Sobolev";

	auto answer = testbook->GetPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "found more than 1");
}
TEST_F(TestBook, GetPhoneNumber_empty)
{
	std::string lastname = "";

	auto answer = testbook->GetPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "not found");
}
TEST_F(TestBook, ChangePhoneNumber)
{
	std::string lastname = "Ivanchenko";

	ASSERT_EQ(std::get<0>(testbook->GetPhoneNumber(lastname)).empty(), true);

	PhoneNumber answer = std::get<1>(testbook->GetPhoneNumber(lastname));//
	EXPECT_EQ(answer.Number, "1234567");

	testbook->ChangePhoneNumber(Person{ "Ivanchenko", "Ivan", "Asphaltovich"}, PhoneNumber{7, 911, "1234567", "10"});

	answer = std::get<1>(testbook->GetPhoneNumber(lastname));
	EXPECT_EQ(answer.CountryCode, 7);
	EXPECT_EQ(answer.CityCode, 911);
	EXPECT_EQ(answer.Number, "1234567");
	EXPECT_EQ(answer.AdditionalNumber, "10");
}
TEST_F(TestBook, SortByPhone) {
	std::ifstream stm("SortByPhoneFile.txt");
	PhoneBook WithoutSort(stm);

	testbook->SortByPhone();
	EXPECT_EQ(testbook, WithoutSort); //объекты всё ж, а не строки
}
TEST_F(TestBook, SortByName) {
	std::ifstream stm("SortByNameFile.txt");
	PhoneBook WithoutSort(stm);

	testbook->SortByName();
	EXPECT_EQ(testbook, WithoutSort);
}
