#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Task1.pb.h"

namespace NSG {
    class iRepository
    {
    public:
        virtual void Open() = 0; // �������� �������������� � ����
        virtual void Save() = 0; // �������� ������������ � ����
        virtual ~iRepository() {}; // ��� ������������� ���� ������ ������� 5-��
    };

    class Methods
    {
    public:
        virtual double GetAverageScore(const Task1::FullName& name) = 0;
        virtual std::string GetAllInfo(const Task1::FullName& name) = 0;
        virtual std::string GetAllInfo() = 0;
        virtual ~Methods() {};
    };

    class StudentsGroup : public iRepository, public Methods
    {
    private:
        std::vector<Task1::Student> m_students;
    public:
        void add_student(const Task1::Student& s)
        {
            m_students.push_back(s);
        }

        double GetAverageScore(const Task1::FullName& name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(), [&](const Task1::Student& s)
                {
                    return std::tie(s.StudentName().FirstName(), s.StudentName().LastName(), s.StudentName().MiddleName()) ==
                        std::tie(name.FirstName(), name.LastName(), name.MiddleName());
                });

            if (it == m_students.end())
            {
                return 0.0;
            }

            return it->AverageRaiting();
        }

        std::string GetAllInfo(const Task1::FullName& name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(), [&](const Task1::Student& s)
                {
                    return std::tie(s.StudentName().FirstName(), s.StudentName().LastName(), s.StudentName().MiddleName()) ==
                        std::tie(name.FirstName(), name.LastName(), name.MiddleName());
                });

            if (it == m_students.end())
            {
                return {};
            }

            std::string temp = "Full Name: " + it->StudentName().FirstName() + ' ' + it->StudentName().LastName() + ' ' + it->StudentName().MiddleName() + "\n";
            temp += "Marks: ";
            for (int i = 0; i < it->Marks().size(); ++i)
            {
                temp += std::to_string(it->Marks(i)) + "; ";
            }
            temp += '\n';
            temp += "Average Raiting: " + std::to_string(it->AverageRaiting()) + "\n\n";

            return temp;
        }

        std::string GetAllInfo()
        {
            std::string temp;

            std::for_each(m_students.begin(), m_students.end(), [&](const Task1::Student& s)
                {
                    temp += GetAllInfo(s.StudentName());
                });

            return temp;
        }

        void Save()
        {
            std::ofstream out("data.bin", std::ios_base::binary);
            auto size = m_students.size();
            out.write(reinterpret_cast<char*>(&size), sizeof(size));
            std::for_each(m_students.begin(), m_students.end(), [&](const Task1::Student& s)
                {
                    s.SerializeToOstream(&out);
                });
            out.close();
        }

        void Open()
        {
            std::ifstream in("data.bin", std::ios_base::binary);
            size_t size = 0;
            in.read(reinterpret_cast<char*>(&size), sizeof(size));
            while (size--)
            {
                Task1::Student s;
                s.ParseFromIstream(&in);
                m_students.push_back(std::move(s));
            }
            in.close();
        }

        virtual ~StudentsGroup() {};
    };
}

void NewStudentsGroup();