#include <iostream>
#include <vector>
#include "Cartesian_tree.h" // Include header files for trees
#include "Randomized_Cartesian_tree.h"
#include "RB_tree.h"
#include "Generate.h"


using namespace std;

int main() {

    vector<int> data, priors, search;

    string data_file = "data.txt";
    string search_file = "search.txt";
    string prior_file = "prior.txt";

    // выбор дерева поиска
    int choice_tree;
    cout << "Select the type of tree to build and test:\n";
    cout << "1. Cartesian tree\n";
    cout << "2. Randomized Cartesian tree\n";
    cout << "3. Red-black tree\n";
    cin >> choice_tree;

    if (choice_tree < 1 || choice_tree > 3) {
        cout << "Invalid choice. Exiting program.\n";
        return 0;
    }

    // выбор структуры данных дерева
    int choice_test;
    cout << "Select the test data structure\n";
    cout << "1. random\n";
    cout << "2. ascending\n";
    cout << "3. descending\n";
    cout << "4. with duplicates\n";
    cin >> choice_test;

    if (choice_test < 1 || choice_test > 4) {
        cout << "Invalid choice. Exiting program.\n";
        return 0;
    }


    // выбор количества данных
    int choice_count;
    cout << "Enter the number of data (up to 500000)\n";
    cin >> choice_count;

    if (choice_count < 1 || choice_count > 500000) {
        cout << "Invalid choice. Exiting program.\n";
        return 0;
    }

    // структура приоритетов
    if (choice_tree == 1) {

        int choice_prior;

        cout << "Select the test prior structure\n";
        cout << "1. random\n";
        cout << "2. ascending\n";
        cout << "3. descending\n";
        cout << "4. with duplicates\n";
        cin >> choice_prior;

        if (choice_prior < 1 || choice_prior > 4) {
            cout << "Invalid choice. Exiting program.\n";
        return 0;
        }
    // генерация файла приоритетов
    if (choice_prior == 1){
        generateDataFile(prior_file, choice_count, "random");      
    } else if (choice_prior == 2) {
        generateDataFile(prior_file, choice_count, "ascending"); 
    } else if (choice_prior == 3) {
        generateDataFile(prior_file, choice_count, "descending"); 
    } else 
        generateDataFile(prior_file, choice_count, "duplicates");  

    ifstream priorFile(prior_file);
    int prior;

    if (!priorFile.is_open()){
        cerr << "Error opening file: " << prior_file << endl;
        return 0;
    }
    
    while (priorFile >> prior)
        priors.push_back(prior);  
    
    priorFile.close();
    }

    // данные поиска
    int count_search;
    cout << "Enter the number of data to search (up to 500000)\n";
    cin >> count_search;

    if (count_search < 1 || count_search > 500000) {
        cout << "Invalid choice. Exiting program.\n";
        return 0;
    }

    // генерация файла с данными
    if (choice_test == 1){
        generateDataFile(data_file, choice_count, "random");      
    } else if (choice_test == 2) {
        generateDataFile(data_file, choice_count, "ascending"); 
    } else if (choice_test == 3) {
        generateDataFile(data_file, choice_count, "descending"); 
    } else 
        generateDataFile(data_file, choice_count, "duplicates");
    
    // генерация файла поиска
    generateDataFile(search_file, count_search, "random");


    // Открытие файла для чтения
    ifstream dataFile(data_file); 
    ifstream searchFile(search_file);

    if(!dataFile.is_open()) {
        cerr << "Error opening file: " << data_file << endl;
        return 0;
    }

    int value;
    while (dataFile >> value) 
        data.push_back(value);
    
    if(!searchFile.is_open()){
        cerr << "Error opening file: " << search_file << endl;
        return 0;
    }

    int elem;
    while (searchFile >> elem)
        search.push_back(elem);
    
    dataFile.close();
    searchFile.close();

    // В зависимости от выбора пользователя тестируется дерево
    if (choice_tree == 1) {
            // Create Cartesian  tree 
            NodeCart* root = build_cart(data, priors, data.size());
            for (size_t i = 0; i < search.size(); i++) {
                NodeCart* result = search_cart(root, search[i]);
                cout << "key " << search[i] << " is ";
                if (result == nullptr)
                    cout << "not ";
                cout << "found in the tree" << endl;
            
            }
        }
        else if (choice_tree == 2) {
            // Create Randomized Cartesian tree
            NodeRC* root = build_rand_cart(data, data.size());
            for (size_t i = 0; i < search.size(); i++) {
                NodeRC* result = search_rand_cart(root, search[i]);
                cout << "key " << search[i] << " is ";
                if (result == nullptr)
                    cout << "not ";
                cout << "found in the tree" << endl;
            }
        }
        else {
            // Create Red-black tree
            NodeRB* root = build_red_black(data, data.size());
            for (size_t i = 0; i < search.size(); i++) {
                NodeRB* result = search_red_black(root, search[i]);
                cout << "key " << search[i] << " is ";
                if (result == nullptr)
                    cout << "not ";
                cout << "found in the tree" << endl;
            }
        }

    return 0;
}
