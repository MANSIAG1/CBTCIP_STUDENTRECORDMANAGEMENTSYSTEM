#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

class Student {
public:
    int rollNumber;
    string name;
    string course;
    int year;

    void input() {
        cout << "Enter Roll Number: ";
        cin >> rollNumber;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Course: ";
        getline(cin, course);
        cout << "Enter Year: ";
        cin >> year;
    }

    void display() const {
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Course: " << course << endl;
        cout << "Year: " << year << endl;
    }
};

void addRecord() {
    Student student;
    student.input();

    ofstream outFile("students.dat", ios::binary | ios::app);
    if (outFile.is_open()) {
        // Writing size of string before the actual string data
        int nameSize = student.name.size();
        int courseSize = student.course.size();

        outFile.write(reinterpret_cast<const char*>(&student.rollNumber), sizeof(student.rollNumber));
        outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        outFile.write(student.name.c_str(), nameSize);
        outFile.write(reinterpret_cast<const char*>(&courseSize), sizeof(courseSize));
        outFile.write(student.course.c_str(), courseSize);
        outFile.write(reinterpret_cast<const char*>(&student.year), sizeof(student.year));

        outFile.close();
        cout << "Record added successfully!" << endl;
    } else {
        cout << "Error opening file for writing!" << endl;
    }
    cout << "\n\n\n\n";
}

void displayRecords() {
    Student student;
    ifstream inFile("students.dat", ios::binary);

    if (!inFile.is_open()) {
        cout << "No records found or error opening file!" << endl;
        cout << "\n\n\n\n";
        return;
    }

    cout << "Displaying all records:\n";
    while (true) {
        int nameSize, courseSize;
        inFile.read(reinterpret_cast<char*>(&student.rollNumber), sizeof(student.rollNumber));
        if (inFile.eof()) break;

        inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        student.name.resize(nameSize);
        inFile.read(&student.name[0], nameSize);

        inFile.read(reinterpret_cast<char*>(&courseSize), sizeof(courseSize));
        student.course.resize(courseSize);
        inFile.read(&student.course[0], courseSize);

        inFile.read(reinterpret_cast<char*>(&student.year), sizeof(student.year));

        student.display();
        cout << "-----------------------" << endl;
    }
    inFile.close();
    cout << "End of records.\n";
    cout << "\n\n\n\n";
}

void searchRecord(int rollNumber) {
    Student student;
    ifstream inFile("students.dat", ios::binary);
    bool found = false;

    if (!inFile.is_open()) {
        cout << "Error opening file for reading!" << endl;
        cout << "\n\n\n\n";
        return;
    }

    while (true) {
        int nameSize, courseSize;
        inFile.read(reinterpret_cast<char*>(&student.rollNumber), sizeof(student.rollNumber));
        if (inFile.eof()) break;

        inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        student.name.resize(nameSize);
        inFile.read(&student.name[0], nameSize);

        inFile.read(reinterpret_cast<char*>(&courseSize), sizeof(courseSize));
        student.course.resize(courseSize);
        inFile.read(&student.course[0], courseSize);

        inFile.read(reinterpret_cast<char*>(&student.year), sizeof(student.year));

        if (student.rollNumber == rollNumber) {
            student.display();
            found = true;
            cout << "\n\n\n\n";
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "Record not found!" << endl;
        cout << "\n\n\n\n";
    }
}

void deleteRecord(int rollNumber) {
    Student student;
    ifstream inFile("students.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;

    if (!inFile.is_open()) {
        cout << "Error opening file for reading!" << endl;
        cout << "\n\n\n\n";
        return;
    }

    while (true) {
        int nameSize, courseSize;
        inFile.read(reinterpret_cast<char*>(&student.rollNumber), sizeof(student.rollNumber));
        if (inFile.eof()) break;

        inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        student.name.resize(nameSize);
        inFile.read(&student.name[0], nameSize);

        inFile.read(reinterpret_cast<char*>(&courseSize), sizeof(courseSize));
        student.course.resize(courseSize);
        inFile.read(&student.course[0], courseSize);

        inFile.read(reinterpret_cast<char*>(&student.year), sizeof(student.year));

        if (student.rollNumber == rollNumber) {
            found = true;
            cout << "Record found and deleted.\n";
        } else {
            outFile.write(reinterpret_cast<const char*>(&student.rollNumber), sizeof(student.rollNumber));

            outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
            outFile.write(student.name.c_str(), nameSize);

            outFile.write(reinterpret_cast<const char*>(&courseSize), sizeof(courseSize));
            outFile.write(student.course.c_str(), courseSize);

            outFile.write(reinterpret_cast<const char*>(&student.year), sizeof(student.year));
        }
    }
    inFile.close();
    outFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        cout << "Record deleted successfully!" << endl;
    } else {
        cout << "Record not found!" << endl;
    }
    cout << "\n\n\n\n";
}

int main() {
    int choice, rollNumber;

    while (true) {
        cout << "**************************************************************************************\n";
        cout << "\n\t\t\t\t\t\tStudent Record Management System\t\t\t\t\t\t\n";
        cout << "1. Add Record\n";
        cout << "2. Display Records\n";
        cout << "3. Search Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Exit\n\n\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 5." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                addRecord();
                break;
            case 2:
                displayRecords();
                break;
            case 3:
                cout << "Enter Roll Number to search: ";
                cin >> rollNumber;
                searchRecord(rollNumber);
                break;
            case 4:
                cout << "Enter Roll Number to delete: ";
                cin >> rollNumber;
                deleteRecord(rollNumber);
                break;
            case 5:
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
