#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <ctime>
#include "Generate.h"

using namespace std;
using namespace std::chrono;

struct NodeRC {
    int data, cnt;
    NodeRC *l, *r;
    NodeRC() {}
    NodeRC(int data) : data(data), cnt(1), l(NULL), r(NULL) {}
};

int cnt(NodeRC* t) {
    return t ? t->cnt : 0;
}

void upd_cnt(NodeRC* t) {
    if (t)
        t->cnt = 1 + cnt(t->l) + cnt(t->r);
}

void split_rand_cart(NodeRC* t, int data, NodeRC* &l, NodeRC* &r) {
    if (!t)
        l = r = NULL;
    else if (data < t->data)
        split_rand_cart(t->l, data, l, t->l), r = t;
    else
        split_rand_cart(t->r, data, t->r, r), l = t;
    upd_cnt(t);
}

void insert_rand_cart(NodeRC* &t, NodeRC* it) {
    if (!t)
        t = it;
    else if (rand() % (cnt(t) + 1) == 0)
        split_rand_cart(t, it->data, it->l, it->r), t = it;
    else
        insert_rand_cart(it->data < t->data ? t->l : t->r, it);
    upd_cnt(t);
}

void merge_rand_cart(NodeRC* &t, NodeRC* l, NodeRC* r) {
    if (!l || !r)
        t = l ? l : r;
    else if (rand() % (cnt(l) + cnt(r)) < cnt(l))
        merge_rand_cart(l->r, l->r, r), t = l;
    else
        merge_rand_cart(r->l, l, r->l), t = r;
    upd_cnt(t);
}

void erase_rand_cart(NodeRC* &t, int data) {
    if (t->data == data)
        merge_rand_cart(t, t->l, t->r);
    else
        erase_rand_cart(data < t->data ? t->l : t->r, data);
    upd_cnt(t);
}

NodeRC* search_rand_cart(NodeRC* root, int data) {
    // Если дерево пустое или ключ найден в корне, возвращаем корень
    if (root == nullptr || root->data == data)
        return root;

    // Если ключ меньше значения корня, рекурсивно ищем в левом поддереве
    if (data < root->data)
        return search_rand_cart(root->l, data);

    // Иначе ищем в правом поддереве
    return search_rand_cart(root->r, data);
}

NodeRC* build_rand_cart(vector<int> data, int n) {
    NodeRC* t = NULL;
    for (int i = 0; i < n; ++i) {
        NodeRC* new_NodeRC = new NodeRC(data[i]);
        insert_rand_cart(t, new_NodeRC);
    }
    return t;
}


int main() {

    cout << "data type / count of data / count of search\n";

   vector<int> data,  search;

    string data_file = "data.txt";
    string search_file = "search.txt";


    // выбор структуры данных дерева
    int choice_test;
    cin >> choice_test;

    // выбор количества данных
    int choice_count;
    cin >> choice_count;

    // данные поиска
    int count_search;
    cin >> count_search;

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

    // Получаем текущее время до выполнения кода
    auto start = high_resolution_clock::now();

    NodeRC* root = build_rand_cart(data, data.size());

            for (size_t i = 0; i < search.size(); i++) {
                NodeRC* result = search_rand_cart(root, search[i]);
            }
    // Получаем текущее время после выполнения кода
    auto stop = high_resolution_clock::now();

    // Рассчитываем разницу между текущим временем и временем до выполнения кода
    auto duration = duration_cast<microseconds>(stop - start);

    // Выводим время выполнения в микросекундах
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}

