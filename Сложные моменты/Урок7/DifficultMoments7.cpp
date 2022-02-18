#include <iostream>
#include "Task2.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <algorithm>
#include <numeric>


namespace Study {
	class FullName {
	public:
		FullName(std::string First, std::string Middle, std::string Last) {
			m_FullName = std::make_tuple(First, Middle, Last);
		}
		FullName(std::string First, std::string Last) {
			m_FullName = std::make_tuple(First, std::nullopt, Last);
		}
		std::tuple<std::string, std::optional< std::string >, std::string> getName() const {
			return m_FullName;
		}
	private:
		std::tuple<std::string, std::optional< std::string >, std::string> m_FullName;
	};

	class Student {
	public:
		Student(FullName& FNS, std::vector<size_t>& MS) : NameStudent(FNS), Marks(MS) {
			AverageRating = std::accumulate(MS.begin(), MS.end(), 0) / MS.size();
		}
		float getAverageRating() const {
			return AverageRating;
		}
		std::vector<size_t> getMarks() const {
			return Marks;
		}
		std::tuple<std::string, std::optional< std::string >, std::string> getNameStudent() const {
			return NameStudent.getName();
		}
	private:
		std::vector<size_t> Marks;
		float AverageRating;
		FullName NameStudent;
	};

	class StudentsGroup {
	public:
		StudentsGroup(std::vector<Student> s) : m_Students(s) {	}
		void NewStudent(Student& new) {
			m_Students.push_back(new);
		}
	private:
		std::vector<Student> m_Students;
	};
}

int main()
{
    std::cout << "Hello World!\n";
}