#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Student {
    string roll_no;
    string name;
    string Class;
    string course;
    string mobile_no;
    string admission_year;
};

vector<Student> students;
unordered_map<string, string> users; // stores username and hashed password

string hashPassword(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

void saveUsers() {
    ofstream file("users.txt");
    for (const auto& user : users) {
        file << user.first << endl;
        file << user.second << endl;
    }
    file.close();
}

void loadUsers() {
    ifstream file("users.txt");
    if (file.is_open()) {
        string username, hashedPassword;
        while (getline(file, username)) {
            getline(file, hashedPassword);
            users[username] = hashedPassword;
        }
        file.close();
    }
}

void saveData() {
    ofstream file("student_data.txt");
    file << students.size() << endl;
    for (const auto& student : students) {
        file << student.roll_no << endl;
        file << student.name << endl;
        file << student.Class << endl;
        file << student.course << endl;
        file << student.mobile_no << endl;
        file << student.admission_year << endl;
    }
    file.close();
}

void loadData() {
    ifstream file("student_data.txt");
    if (file.is_open()) {
        int total;
        file >> total;
        file.ignore();
        students.clear(); // Clear existing data
        for (int i = 0; i < total; ++i) {
            Student student;
            getline(file, student.roll_no);
            getline(file, student.name);
            getline(file, student.Class);
            getline(file, student.course);
            getline(file, student.mobile_no);
            getline(file, student.admission_year);
            students.push_back(student);
        }
        file.close();
    }
}

// API Functions
bool addStudent(const Student& student) {
    students.push_back(student);
    saveData();
    return true;
}

bool getStudent(const string& roll_no, Student& student) {
    for (const auto& s : students) {
        if (s.roll_no == roll_no) {
            student = s;
            return true;
        }
    }
    return false;
}

bool updateStudent(const string& roll_no, const Student& updatedStudent) {
    for (auto& s : students) {
        if (s.roll_no == roll_no) {
            s = updatedStudent;
            saveData();
            return true;
        }
    }
    return false;
}

bool deleteStudent(const string& roll_no) {
    auto it = remove_if(students.begin(), students.end(), [&](const Student& student) {
        return student.roll_no == roll_no;
    });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveData();
        return true;
    }
    return false;
}

bool clearAllStudents() {
    students.clear();
    saveData();
    return true;
}

bool registerNewUser(const string& username, const string& password) {
    if (users.find(username) != users.end()) {
        return false; // Username already exists
    }
    users[username] = hashPassword(password);
    saveUsers();
    return true;
}

bool validateUserLogin(const string& username, const string& password) {
    if (users.find(username) != users.end() && users[username] == hashPassword(password)) {
        return true;
    }
    return false;
}

int main() {
    loadUsers();

    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    int choice;
    cin >> choice;
    cin.ignore(); // Ignore the newline character left by cin
    bool authenticated = false;
    if (choice == 1) {
        string username, password;
        cout << "Enter a new username: ";
        getline(cin, username);
        cout << "Enter a new password: ";
        getline(cin, password);
        if (registerNewUser(username, password)) {
            authenticated = validateUserLogin(username, password); // Login after registration
        } else {
            cout << "Username already exists. Try again." << endl;
            return 0;
        }
    } else if (choice == 2) {
        string username, password;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
        authenticated = validateUserLogin(username, password);
    } else {
        cout << "Invalid choice. Exiting." << endl;
        return 0;
    }

    if (!authenticated) {
        cout << "Authentication failed. Exiting." << endl;
        return 0;
    }

    loadData();
    while (true) {
        int value;
        cout << "\n1. Enter data" << endl;
        cout << "2. Show data" << endl;
        cout << "3. Search data" << endl;
        cout << "4. Update data" << endl;
        cout << "5. Delete data" << endl;
        cout << "6. Clear all data" << endl;
        cout << "7. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> value;
        cin.ignore(); // Ignore the newline character left by cin
        switch (value) {
            case 1: {
                int num_students;
                cout << "How many students do you want to enter?" << endl;
                cin >> num_students;
                cin.ignore();
                for (int i = 0; i < num_students; ++i) {
                    Student student;
                    cout << "\nEnter the Data of student " << students.size() + 1 << endl << endl;
                    cout << "Enter Roll NO: ";
                    getline(cin, student.roll_no);
                    cout << "Enter Name: ";
                    getline(cin, student.name);
                    cout << "Enter Class: ";
                    getline(cin, student.Class);
                    cout << "Enter Course: ";
                    getline(cin, student.course);
                    cout << "Enter Mobile NO: ";
                    getline(cin, student.mobile_no);
                    cout << "Enter Admission Year: ";
                    getline(cin, student.admission_year);
                    addStudent(student);
                }
                break;
            }
            case 2:
                if (students.empty()) {
                    cout << "No data is entered." << endl;
                } else {
                    for (size_t i = 0; i < students.size(); ++i) {
                        const auto& student = students[i];
                        cout << "\nData of Student " << i + 1 << endl;
                        cout << "Roll NO: " << student.roll_no << endl;
                        cout << "Name: " << student.name << endl;
                        cout << "Class: " << student.Class << endl;
                        cout << "Course: " << student.course << endl;
                        cout << "Mobile NO: " << student.mobile_no << endl;
                        cout << "Admission Year: " << student.admission_year << endl;
                    }
                }
                break;
            case 3: {
                string roll_no;
                cout << "Enter the roll no of student: ";
                cin >> roll_no;
                cin.ignore();
                Student student;
                if (getStudent(roll_no, student)) {
                    cout << "Roll NO: " << student.roll_no << endl;
                    cout << "Name: " << student.name << endl;
                    cout << "Class: " << student.Class << endl;
                    cout << "Course: " << student.course << endl;
                    cout << "Mobile NO: " << student.mobile_no << endl;
                    cout << "Admission Year: " << student.admission_year << endl;
                } else {
                    cout << "Student with Roll NO " << roll_no << " not found." << endl;
                }
                break;
            }
            case 4: {
                string roll_no;
                cout << "Enter the roll no of student which you want to update: ";
                cin >> roll_no;
                cin.ignore();
                Student updatedStudent;
                if (getStudent(roll_no, updatedStudent)) {
                    cout << "\nEnter new data" << endl;
                    cout << "Enter Roll NO: ";
                    getline(cin, updatedStudent.roll_no);
                    cout << "Enter Name: ";
                    getline(cin, updatedStudent.name);
                    cout << "Enter Class: ";
                    getline(cin, updatedStudent.Class);
                    cout << "Enter Course: ";
                    getline(cin, updatedStudent.course);
                    cout << "Enter Mobile NO: ";
                    getline(cin, updatedStudent.mobile_no);
                    cout << "Enter Admission Year: ";
                    getline(cin, updatedStudent.admission_year);
                    if (updateStudent(roll_no, updatedStudent)) {
                        cout << "Student updated successfully!" << endl;
                    } else {
                        cout << "Failed to update student." << endl;
                    }
                } else {
                    cout << "Student with Roll NO " << roll_no << " not found." << endl;
                }
                break;
            }
            case 5: {
                string roll_no;
                cout << "Enter the roll no of student to delete: ";
                cin >> roll_no;
                cin.ignore();
                if (deleteStudent(roll_no)) {
                    cout << "Record deleted!" << endl;
                } else {
                    cout << "Student with Roll NO " << roll_no << " not found." << endl;
                }
                break;
            }
            case 6:
                if (clearAllStudents()) {
                    cout << "All records are deleted!" << endl;
                } else {
                    cout << "Failed to clear all records." << endl;
                }
                break;
            case 7:
                saveData();
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid input!" << endl;
        }
    }
}
