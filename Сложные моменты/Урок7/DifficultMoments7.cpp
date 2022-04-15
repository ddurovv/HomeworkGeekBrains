#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <fstream>
#include "Task1.pb.h"
#include "NewStudentsGroup.h"

int main()
{
    {
    Task1::FullName fn;
    fn.set_firstname ("Dmitrii");
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

    Task1::StudentsGroup sg;
    *sg.add_studentsofgroup() = s;

    std::ofstream out("student.bin", std::ios_base::binary);
    sg.SerializeToOstream(&out);
    out.close();

    Task1::StudentsGroup new_sg;
    std::ifstream in("student.bin", std::ios_base::binary);
    if (new_sg.ParseFromIstream(&in))
    {
        std::cout << new_sg.studentsofgroup(0).studentname().lastname() << "\n";
        std::cout << new_sg.studentsofgroup(0).averagerating() << "\n";
    }
    else
    {
        std::cout << "Error!" << "\n";
    }
    in.close();
    }

    {
        NewStudentsGroup();
    }
}