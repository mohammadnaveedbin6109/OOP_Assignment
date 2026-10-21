#include <iostream>
using namespace std;

struct StudentRecord {
    string name;
    double gpa;

    void print_info() {
        cout << name << " : " << gpa << endl;
    }
};

int main() {
    StudentRecord s = {"Ali", 3.6};
    StudentRecord *ptr = &s;

    ptr->gpa = 3.9;
    ptr->print_info();
    return 0;
}