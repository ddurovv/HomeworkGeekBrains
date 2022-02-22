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

    Task1::StudentsGroup sg;
    *sg.add_StudentsOfGroup() = s;

    std::ofstream out("student.bin", std::ios_base::binary);
    sg.SerializeToOstream(&out);
    out.close();

    Task1::StudentsGroup new_sg;
    std::ifstream in("student.bin", std::ios_base::binary);
    if (new_sg.ParseFromIstream(&in))
    {
        std::cout << new_sg.StudentsOfGroup(0).StudentName().LastName() << "\n";
        std::cout << new_sg.StudentsOfGroup(0).AverageRaiting() << "\n";
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