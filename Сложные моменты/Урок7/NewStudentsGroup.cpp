#include <numeric>
#include "NewStudentsGroup.h"

void NewStudentsGroup() {
    Task1::FullName fn;
    fn.set_firstname("Dmitrii");
    fn.set_lastname("Durov");
    fn.set_middlename("Igorevich");

    Task1::Student s;
    *s.mutable_studentname() = fn;
    s.add_marks(5);
    s.add_marks(2);
    s.add_marks(4);
    s.add_marks(3);
    s.add_marks(4);
    s.add_marks(3);
    s.add_marks(4);
    s.add_marks(3);
    s.set_averagerating(std::accumulate(s.marks().begin(), s.marks().end(), 0) / s.marks().size());

	NSG::StudentsGroup sg;
	sg.add_student(s);
	sg.Save();

	NSG::StudentsGroup new_sg;
	new_sg.Open();
	std::cout << new_sg.GetAllInfo(fn) << "\n";
}