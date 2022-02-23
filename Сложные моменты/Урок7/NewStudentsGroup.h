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
        virtual void Open() = 0; // бинарная десериализация в файл
        virtual void Save() = 0; // бинарная сериализация в файл
        virtual ~iRepository() {}; // мне действительно лень писать правило 5-ти
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
                    return std::tie(s.studentname().firstname(), s.studentname().lastname(), s.studentname().middlename()) ==
                        std::tie(name.firstname(), name.lastname(), name.middlename());
                });

            if (it == m_students.end())
            {
                return 0.0;
            }

            return it->averagerating();
        }

        std::string GetAllInfo(const Task1::FullName& name)
        {
            auto it = std::find_if(m_students.begin(), m_students.end(), [&](const Task1::Student& s)
                {
                    return std::tie(s.studentname().firstname(), s.studentname().lastname(), s.studentname().middlename()) ==
                        std::tie(name.firstname(), name.lastname(), name.middlename());
                });

            if (it == m_students.end())
            {
                return {};
            }

            std::string temp = "Full Name: " + it->studentname().firstname() + ' ' + it->studentname().lastname() + ' ' + it->studentname().middlename() + "\n";
            temp += "Marks: ";
            for (int i = 0; i < it->marks().size(); ++i)
            {
                temp += std::to_string(it->marks(i)) + "; ";
            }
            temp += '\n';
            temp += "Average Rating: " + std::to_string(it->averagerating()) + "\n\n";

            return temp;
        }

        std::string GetAllInfo()
        {
            std::string temp;

            std::for_each(m_students.begin(), m_students.end(), [&](const Task1::Student& s)
                {
                    temp += GetAllInfo(s.studentname());
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