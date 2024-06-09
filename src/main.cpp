#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <memory>
#include "RB_tree.h"
#include "Randomized_Cartesian_tree.h"
#include "Generate.h"
#include "Cartesian_tree.h"

using namespace std;
using namespace chrono;

void writeVectorToFile(const vector<int>& vec, const string& filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error opening file:" << filename << endl;
        return;
    }

    for (const int& element : vec) {
        outFile << element << endl;
    }

    outFile.close();
}

template<typename Node, typename BuildFunc, typename SearchFunc>
void run_test(Node*& root, BuildFunc build, SearchFunc search, vector<int>& data, const vector<int>& search_data, int repeat, vector<int>& common) {
    for (int i = 0; i < repeat; ++i) {

        auto start = high_resolution_clock::now();

        root = build(data, data.size()); // Создаем новое дерево

        for (int value : search_data) {
            if(search(root, value))
                common.push_back(value);
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        cout << duration.count() << '\n';
    }

    // if (root != nullptr) {
    //         delete_tree(root); // Удаляем предыдущее дерево, если оно существует
    //         root = nullptr;
    //     }
    cout << '\n';
}

int main() {
    int arr[1] = {10000};
    int choice_test = 1; // выбор данных
    int choice_tree = 1; // 1 для красно-черного дерева, 2 для рандомизированного декартова дерева, 3 для декартового

    for (int elem1 : arr) {
        int count_search = elem1;

        vector<int> data, search;
        string data_file = "data.txt";
        string search_file = "search.txt";

        if (choice_test == 1) {
            generateDataFile(data_file, elem1, "random");      
        } else if (choice_test == 2) {
            generateDataFile(data_file, elem1, "ascending"); 
        } else if (choice_test == 3) {
            generateDataFile(data_file, elem1, "descending"); 
        } else {
            generateDataFile(data_file, elem1, "duplicates");
        }

        generateDataFile(search_file, count_search, "random");

        ifstream dataFile(data_file); 
        ifstream searchFile(search_file);

        if (!dataFile.is_open()) {
            cerr << "Error opening file: " << data_file << endl;
            return 0;
        }

        int value;
        while (dataFile >> value) 
            data.push_back(value);
        
        if (!searchFile.is_open()) {
            cerr << "Error opening file: " << search_file << endl;
            return 0;
        }

        int elem;
        while (searchFile >> elem)
            search.push_back(elem);
        
        dataFile.close();
        searchFile.close();
        
        int repeat = 1  ; // количество повторений
        vector<int> common;

        if (choice_tree == 1) {
            NodeRB* root = nullptr;
            run_test(root, build_red_black, search_red_black, data, search, repeat, common);
        } else if (choice_tree == 2) {
            NodeRC* root = nullptr;
            run_test(root, build_rand_cart, search_rand_cart, data, search, repeat, common);
        } else if (choice_tree == 3) {
            NodeCart* root = nullptr;
            run_test(root, build_cart, search_cart, data, search, repeat, common);
        }
        cout << common.size();

        writeVectorToFile(common, "result.txt");
    }

    return 0;
}