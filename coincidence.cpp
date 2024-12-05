#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

vector<int> readDataFromFile(const string& filename) {
    ifstream file(filename);
    vector<int> data;
    int value;

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    while (file >> value) {
        data.push_back(value);
    }

    file.close();
    return data;
}

void writeDataToFile(const string& filename, const vector<int>& data) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (int value : data) {
        file << value << '\n';
    }

    file.close();
}

vector<int> findCommonNumbers(const vector<int>& data1, const vector<int>& data2) {
    unordered_set<int> set1(data1.begin(), data1.end());
    vector<int> common;

    for (int value : data2) {
        if (set1.find(value) != set1.end()) {
            common.push_back(value);
        }
    }

    return common;
}

int main() {
    string file1 = "data.txt";
    string file2 = "search.txt";
    string resultFile = "common_numbers.txt";

    vector<int> data1 = readDataFromFile(file1);
    vector<int> data2 = readDataFromFile(file2);

    vector<int> commonNumbers = findCommonNumbers(data1, data2);

    writeDataToFile(resultFile, commonNumbers);

    cout << "Common numbers written to " << resultFile << endl;

    return 0;
}