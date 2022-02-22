#include <numeric>
#include "NewStudentsGroup.h"

void NewStudentsGroup() {
    Task1::FullName fn;
    fn.set_FirstName("Dmitrii");
    fn.set_LastName("Durov");
    fn.set_MiddleName("Igorevich");

    Task1::Student s;
    *s.mutable_StudentName() = fn;
    s.add_Marks(5);
    s.add_Marks(2);
    s.add_Marks(4);
    s.add_Marks(3);
    s.add_Marks(4);
    s.add_Marks(3);
    s.add_Marks(4);
    s.add_Marks(3);
    s.set_AverageRating(std::accumulate(s.Marks().begin(), s.Marks().end(), 0) / s.Marks().size());

	NSG::StudentsGroup sg;
	sg.add_student(s);
	sg.Save();

	NSG::StudentsGroup new_sg;
	new_sg.Open();
	std::cout << new_sg.GetAllInfo(fn) << "\n";
}