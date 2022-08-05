#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>

typedef std::tuple<std::string, std::string, int, int, int> SortTuple2;

struct Student ///Structure Student
{
    std::string name;
    std::string surname;
    int day{};
    int month{};
    int year{};
    std::vector<std::string> uniChoices;
    double score{};
};

struct AllStudent ///Structure AllStudents
{
    std::vector<Student> students;
};

struct Univ ///Structure for one university
{
    std::string name;
    int nPlaces{};
    std::vector<SortTuple2> acceptedStudents;
};

struct AllUnis ///Structure for many universities
{
    std::map<std::string, Univ> allUnisMap;
};

typedef std::tuple<double, int, int, int, std::string, std::string> SortTuple;
typedef std::map<SortTuple, Student> TupleStudentMap;

void appendUnis(int n, AllUnis &all) ///Function processes available universities -> Structure
{
    for (int i = 0; i < n; i++)
    {
        Univ aUni;
        std::string dummy;
        std::cin >> aUni.name >> aUni.nPlaces;
        all.allUnisMap[aUni.name] = aUni;
    }
}

std::vector<std::string> readUnis(int iterateN) ///Function processes universities to which applicants want to apply
{
    std::string dummy;
    std::vector<std::string> uni;

    for (int i = 0; i < iterateN - 1; i++)
    {
        std::cin >> dummy;
        uni.push_back(dummy);
    }
    std::cin >> dummy;
    uni.push_back(dummy);
    return uni;
}

void processStudent(AllStudent &allStudents) ///Function Processes 1 student
{
    Student student;
    int j;
    std::cin >> student.name >> student.surname >> student.day >> student.month >> student.year >>j;
    student.score = 1.0 / j;

    //Number of Universities -> Vector
    int iterateN;
    std::cin >> iterateN;
    if (iterateN == 0) //Case with no universities desired
    {
        return;
    }
    student.uniChoices = readUnis(iterateN);
    allStudents.students.push_back(student);
}

void readFile(AllUnis &all, AllStudent &studs) ///Reads Input (Originally was a file)
{
    std::string dummy;
    int nOfUnis;
    std::cin >> nOfUnis;
    appendUnis(nOfUnis, all);
    int nOfStudents;
    std::cin >> nOfStudents;
    for (int i = 0; i < nOfStudents; i++)
    {
        processStudent(studs);
    }
}

SortTuple tieClass(Student &s) ///Tuple for 1st sort
{
    return std::tie(s.score, s.year, s.month, s.day, s.surname, s.name);
}

void createTupleStudentMap(TupleStudentMap &mapTupStu, AllStudent &allStudents) ///Creates a map key = class && value = tuple(SortTuple)
{
    for (auto &student: allStudents.students)
    {
        SortTuple tupleStu;
        tupleStu = tieClass(student);
        mapTupStu[tupleStu] = student;
    }
}

SortTuple2 tieClass2(Student &s) ///Tuple for 2nd sort
{
    return std::tie(s.surname, s.name, s.year, s.month, s.day);
}

void putStudents(TupleStudentMap &mapTupStu, AllUnis &allUnis) ///Distirbutes students to universities
{
    for (auto &i: mapTupStu)
    {
        for (const std::string &j: i.second.uniChoices)
        {
            if (allUnis.allUnisMap[j].nPlaces != 0)
            {
                SortTuple2 st = tieClass2(i.second);
                allUnis.allUnisMap[j].acceptedStudents.push_back(st);
                allUnis.allUnisMap[j].nPlaces -= 1;
                break;
            }
        }
    }
}

bool sortAlgos(SortTuple2 &rhs, SortTuple2 &lhs) ///Sorting algorithm
{
    return rhs < lhs;
}

void sortEachUni(AllUnis &allUnis) ///Sort each vector within university
{
    for (auto &k: allUnis.allUnisMap)
    {
        std::sort(k.second.acceptedStudents.begin(), k.second.acceptedStudents.end(), sortAlgos);
    }
}

void printStudents(AllUnis &allUnis) ///Output result
{
    for (const auto &k: allUnis.allUnisMap)
    {
        std::cout << k.first;
        for (const auto &i: k.second.acceptedStudents)
        {
            std::cout << "\t";
            std::cout << std::get<1>(i) << ' ' << std::get<0>(i);
        }
        std::cout << std::endl;
    }
}

int main()
{
    ///Initialize Structures
    AllUnis allUnis;
    AllStudent allStudents;
    ///ReadFile
    readFile(allUnis, allStudents);
    ///Sort students using tuple and map
    TupleStudentMap mapTupStu;
    createTupleStudentMap(mapTupStu, allStudents);
    ///Put students into universities
    putStudents(mapTupStu, allUnis);
    sortEachUni(allUnis);
    ///Cout
    printStudents(allUnis);
    return 0;
}