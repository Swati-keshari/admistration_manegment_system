#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <locale>
using namespace std;

const int LEN = 32;
const int MAXPEOPLE = 100;

enum person_type { tteacher, tadmin_staff, tnon_admin_staff, tstudent };

class person {
protected:
    string id;
    string name;
    static int n;
    static person* arrap[MAXPEOPLE];
    static set<string> used_ids;

public:
    virtual void getdata() {
        cin.ignore(10, '\n');
        cout << "Enter name: "; getline(cin, name);

        do {
            cout << "Enter unique ID: "; cin >> id;
            if (used_ids.find(id) != used_ids.end()) {
                cout << "ID already in use. Please enter a unique ID.\n";
            }
        } while (used_ids.find(id) != used_ids.end());

        used_ids.insert(id);
    }

    virtual void putdata() {
        cout << "\nID: " << id << "\nName: " << name;
    }

    virtual person_type get_type() = 0;

    virtual void read_from_file(ifstream& in) = 0;
    virtual void write_to_file(ofstream& out) = 0;

    static void add();
    static void display();
    static void write();
    static void read();
    static void delete_by_id();
    static void edit_by_id();
};

int person::n = 0;
person* person::arrap[MAXPEOPLE];
set<string> person::used_ids;

class teacher : public person {
private:
    string post;
    double salary;
    string department;

public:
    void getdata() override {
        person::getdata();
        cin.ignore();
        cout << "Enter Post (Professor/Assistant Professor/Lecturer): "; getline(cin, post);
        cout << "Enter Department: "; getline(cin, department);
        while (true) {
            cout << "Enter Salary: "; cin >> salary;
            if (cin.fail() || salary <= 0) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid input. Salary must be a positive number.\n";
            } else {
                break;
            }
        }
    }

    void putdata() override {
        person::putdata();
        cout << "\nPost: " << post;
        cout << "\nDepartment: " << department;
        cout << "\nSalary: " << fixed << setprecision(2) << salary;
    }

    person_type get_type() override {
        return tteacher;
    }

    void read_from_file(ifstream& in) override {
        getline(in, id);
        getline(in, name);
        getline(in, post);
        getline(in, department);
        in >> salary;
        in.ignore();
    }

    void write_to_file(ofstream& out) override {
        out << "Teacher\n";
        out << id << '\n';
        out << name << '\n';
        out << post << '\n';
        out << department << '\n';
        out << salary << '\n';
        out << "---------------------------\n";
    }
};

class admin_staff : public person {
private:
    string post;

public:
    void getdata() override {
        person::getdata();
        cin.ignore();
        cout << "Enter Post (Admin Assistant/Clerk/Manager): "; getline(cin, post);
    }

    void putdata() override {
        person::putdata();
        cout << "\nPost: " << post;
    }

    person_type get_type() override {
        return tadmin_staff;
    }

    void read_from_file(ifstream& in) override {
        getline(in, id);
        getline(in, name);
        getline(in, post);
        in.ignore();
    }

    void write_to_file(ofstream& out) override {
        out << "AdminStaff\n";
        out << id << '\n';
        out << name << '\n';
        out << post << '\n';
        out << "---------------------------\n";
    }
};

class non_admin_staff : public person {
private:
    string post;

public:
    void getdata() override {
        person::getdata();
        cin.ignore();
        cout << "Enter Post (Lab Assistant/Cleaner/Guard): "; getline(cin, post);
    }

    void putdata() override {
        person::putdata();
        cout << "\nPost: " << post;
    }

    person_type get_type() override {
        return tnon_admin_staff;
    }

    void read_from_file(ifstream& in) override {
        getline(in, id);
        getline(in, name);
        getline(in, post);
        in.ignore();
    }

    void write_to_file(ofstream& out) override {
        out << "NonAdminStaff\n";
        out << id << '\n';
        out << name << '\n';
        out << post << '\n';
        out << "---------------------------\n";
    }
};

class student : public person {
private:
    string course;
    int year;

public:
    void getdata() override {
        person::getdata();
        cin.ignore();
        cout << "Enter Course (B.Tech/M.Tech/PhD): "; getline(cin, course);
        while (true) {
            cout << "Enter Year of Study: "; cin >> year;
            if (cin.fail() || year <= 0) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid input. Year must be a positive integer.\n";
            } else {
                break;
            }
        }
    }

    void putdata() override {
        person::putdata();
        cout << "\nCourse: " << course;
        cout << "\nYear of Study: " << year;
    }

    person_type get_type() override {
        return tstudent;
    }

    void read_from_file(ifstream& in) override {
        getline(in, id);
        getline(in, name);
        getline(in, course);
        in >> year;
        in.ignore();
    }

    void write_to_file(ofstream& out) override {
        out << "Student\n";
        out << id << '\n';
        out << name << '\n';
        out << course << '\n';
        out << year << '\n';
        out << "---------------------------\n";
    }
};

void person::add() {
    char ch;
    cout << "'t' to add a teacher\n"
         << "'a' to add admin staff\n"
         << "'n' to add non-admin staff\n"
         << "'s' to add a student\n"
         << "Enter selection: ";
    cin >> ch;

    switch (ch) {
        case 't': arrap[n] = new teacher; break;
        case 'a': arrap[n] = new admin_staff; break;
        case 'n': arrap[n] = new non_admin_staff; break;
        case 's': arrap[n] = new student; break;
        default: cout << "\nUnknown type\n"; return;
    }
    arrap[n++]->getdata();
}

void person::display() {
    if (n == 0) {
        cout << "No records found.\n";
        return;
    }
    for (int j = 0; j < n; j++) {
        arrap[j]->putdata();
        cout << endl;
    }
}

void person::write() {
    ofstream out("people.txt");
    if (!out) {
        cout << "\nCan't open file\n";
        return;
    }

    for (int j = 0; j < n; j++) {
        arrap[j]->write_to_file(out);
    }

    out.close();
    cout << "Data successfully saved to 'people.txt'\n";
}

void person::read() {
    ifstream in("people.txt");
    if (!in) {
        cout << "\nCan't open file\n";
        return;
    }
    cout << "Reading people from file...\n";
    string type;

    while (getline(in, type)) {
//       type = trim(type);
        if (type == "Teacher") {
            arrap[n] = new teacher;
        } else if (type == "AdminStaff") {
            arrap[n] = new admin_staff;
        } else if (type == "NonAdminStaff") {
            arrap[n] = new non_admin_staff;
        } else if (type == "Student") {
            arrap[n] = new student;
        } else {
            cout << "Unknown person type: " << type << endl;
            continue;
        }
        arrap[n]->read_from_file(in);
        used_ids.insert(arrap[n]->id);
        n++;
    }

    in.close();
    cout << "Data successfully read from 'people.txt'\n";
}

void person::delete_by_id() {
    if (n == 0) {
        cout << "No records to delete.\n";
        return;
    }

    string id_to_delete;
    cout << "Enter the ID of the person to delete: "; cin >> id_to_delete;

    bool found = false;
    for (int j = 0; j < n; j++) {
        if (arrap[j]->id == id_to_delete) {
            found = true;
            used_ids.erase(id_to_delete);
            delete arrap[j];
            for (int k = j; k < n - 1; k++) {
                arrap[k] = arrap[k + 1];
            }
            n--;
            cout << "Record with ID " << id_to_delete << " deleted.\n";
            break;
        }
    }

    if (!found) {
        cout << "No record found with ID " << id_to_delete << ".\n";
    }
}

void person::edit_by_id() {
    if (n == 0) {
        cout << "No records to edit.\n";
        return;
    }

    string id_to_edit;
    cout << "Enter the ID of the person to edit: "; cin >> id_to_edit;

    bool found = false;
    for (int j = 0; j < n; j++) {
        if (arrap[j]->id == id_to_edit) {
            found = true;
            cout << "Editing record for ID " << id_to_edit << ":\n";
            arrap[j]->getdata();
            break;
        }
    }

    if (!found) {
        cout << "No record found with ID " << id_to_edit << ".\n";
    }
}

int main() {
    char ch;
    while (true) {
        cout << "'a' -- add data for a person"
             << "\n'd' -- display data for all people"
             << "\n'w' -- write all people to file"
             << "\n'r' -- read all people from file"
             << "\n'x' -- delete a person by ID"
             << "\n'e' -- edit a person by ID"
             << "\n'q' -- quit"
             << "\nEnter selection: ";
        cin >> ch;

        switch (ch) {
            case 'a': person::add(); break;
            case 'd': person::display(); break;
            case 'w': person::write(); break;
            case 'r': person::read(); break;
            case 'x': person::delete_by_id(); break;
            case 'e': person::edit_by_id(); break;
            case 'q': exit(0); break;
            default: cout << "Unknown command\n";
        }
    }

    return 0;
}
