#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Student {
    char name[30];
    int group;
    double grade;
};

int countStudentsInFile(const char* filename);
bool readStudentsFromFile(const char* filename, Student* students, int count);
void writeStudentsToFile(const char* filename, Student* students, int count);
void printStudents(Student* students, int count);
void printGroupAverages(Student* students, int count);
void searchStudent(Student* students, int count, const char* surname);
void sortByName(Student* students, int count);
void sortByGroupThenName(Student* students, int count);
void swapStudents(Student& a, Student& b);

int main() {
    const char* filename = "students.txt";
    int studentCount = countStudentsInFile(filename);

    if (studentCount == 0) {
        cout << "File is empty or not found. Program will exit\n";
        return 1;
    }

    Student* students = new Student[studentCount];

    if (!readStudentsFromFile(filename, students, studentCount)) {
        cout << "Error reading file\n";
        delete[] students;
        return 1;
    }

    int choice;
    do {
        cout << "Menu:\n";
        cout << "1. Sort by name and write to file\n";
        cout << "2. Search student by surname\n";
        cout << "3. Sort by group and name\n";
        cout << "4. Display average grade per group\n";
        cout << "5. Display all students\n";
        cout << "0. Exit\n";
        cout << "Select an action: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            sortByName(students, studentCount);
            writeStudentsToFile(filename, students, studentCount);
            cout << "Data sorted and written to file.\n";
            break;
        case 2: {
            char surname[30];
            cout << "Enter student's surname: ";
            cin.getline(surname, 30);
            searchStudent(students, studentCount, surname);
            break;
        }
        case 3:
            sortByGroupThenName(students, studentCount);
            cout << "Sorted list of students:\n";
            printStudents(students, studentCount);
            break;
        case 4:
            printGroupAverages(students, studentCount);
            break;
        case 5:
            printStudents(students, studentCount);
            break;
        case 0:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    delete[] students;
    return 0;
}

int countStudentsInFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) return 0;

    int count = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) count++;
    }

    file.close();
    return count;
}

bool readStudentsFromFile(const char* filename, Student* students, int count) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    for (int i = 0; i < count; i++) {
        string line;
        if (!getline(file, line)) break;

        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);

        if (pos1 != string::npos && pos2 != string::npos) {
            string name = line.substr(0, pos1);
            strncpy(students[i].name, name.c_str(), sizeof(students[i].name) - 1);
            students[i].name[sizeof(students[i].name) - 1] = '\0';

            students[i].group = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            students[i].grade = stod(line.substr(pos2 + 1));
        }
    }

    file.close();
    return true;
}

void writeStudentsToFile(const char* filename, Student* students, int count) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file for writing\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        file << students[i].name << ";" << students[i].group << ";" << students[i].grade << '\n';
    }

    file.close();
}

void printStudents(Student* students, int count) {
    cout << "Name        Group     Average Grade\n";
    cout << "                              \n";
    for (int i = 0; i < count; i++) {
        cout << students[i].name << " " << students[i].group << "   " << students[i].grade << '\n';
    }
}

void printGroupAverages(Student* students, int count) {
    sortByGroupThenName(students, count);

    int currentGroup = students[0].group;
    double sum = students[0].grade;
    int groupCount = 1;

    cout << "Average grade per group:\n";
    cout << "Group     Average Grade\n";

    for (int i = 1; i < count; i++) {
        if (students[i].group == currentGroup) {
            sum += students[i].grade;
            groupCount++;
        }
        else {
            cout << currentGroup << "     " << (sum / groupCount) << '\n';
            currentGroup = students[i].group;
            sum = students[i].grade;
            groupCount = 1;
        }
    }

    cout << currentGroup << "      " << (sum / groupCount) << '\n';
}

void searchStudent(Student* students, int count, const char* surname) {
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (strstr(students[i].name, surname) == students[i].name) {
            if (!found) {
                cout << "Found students:\nName       Group       Average Grade\n";
                found = true;
            }
            cout << students[i].name << "    " << students[i].group << "     " << students[i].grade << '\n';
        }
    }

    if (!found) {
        cout << "Student with surname '" << surname << "' not found.\n";
    }
}

void swapStudents(Student& a, Student& b) {
    Student temp = a;
    a = b;
    b = temp;
}

void sortByName(Student* students, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                swapStudents(students[j], students[j + 1]);
            }
        }
    }
}

void sortByGroupThenName(Student* students, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].group > students[j + 1].group ||
                (students[j].group == students[j + 1].group && strcmp(students[j].name, students[j + 1].name) > 0)) {
                swapStudents(students[j], students[j + 1]);
            }
        }
    }
}
