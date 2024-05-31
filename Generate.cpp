#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;

// Функция для генерации случайных данных
void generateRandomData(ofstream& file, int count) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000);
    for (int i = 0; i < count; ++i) {
        file << dis(gen) << endl;
    }
}

// Функция для генерации упорядоченных данных по возрастанию
void generateAscendingData(ofstream& file, int count) {
    for (int i = 0; i < count; ++i) {
        file << i << endl;
    }
}

// Функция для генерации упорядоченных данных по убыванию
void generateDescendingData(ofstream& file, int count) {
    for (int i = count - 1; i >= 0; --i) {
        file << i << endl;
    }
}

// Функция для генерации данных с повторениями
void generateDataWithDuplicates(ofstream& file, int count) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, max(count/300, 100));
    for (int i = 0; i < count; ++i) {
        file << dis(gen) << endl;
    }
}

// Функция для генерации данных в файл
void generateDataFile(const string& filename, int count, const string& structure) {
    ofstream file(filename);

    if (!file) {
        cerr << "Failed to create file." << endl;
        return;
    }

    if (structure == "random") {
        generateRandomData(file, count);
    } else if (structure == "ascending") {
        generateAscendingData(file, count);
    } else if (structure == "descending") {
        generateDescendingData(file, count);
    } else if (structure == "duplicates") {
        generateDataWithDuplicates(file, count);
    } else {
        cerr << "Invalid structure specified." << endl;
    }

    file.close();
}
