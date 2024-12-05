#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool compareFiles(const string& file1, const string& file2) {
    ifstream f1(file1, ios::binary);
    ifstream f2(file2, ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        cerr << "Error opening files for comparison: " << file1 << " or " << file2 << endl;
        return false;
    }

    istreambuf_iterator<char> begin1(f1), end1;
    istreambuf_iterator<char> begin2(f2), end2;

    return equal(begin1, end1, begin2, end2);
}

int main() {
    string file1 = "result.txt";
    string file2 = "data.txt";

    if (compareFiles(file1, file2)) {
        cout << "Files are equal" << endl;
    } else {
        cout << "Files are not equal" << endl;
    }

    return 0;
}