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

struct NodeCart {
    int data, prior;
    NodeCart * l, * r;
    NodeCart() { }
    NodeCart (int data, int prior) : data(data), prior(prior), l(NULL), r(NULL) { }
};

void search_cart (NodeCart* t, int data, NodeCart* & l, NodeCart* & r) {
    if (!t)
        l = r = NULL;
    else if (data < t->data)
        search_cart (t->l, data, l, t->l),  r = t;
    else
        search_cart (t->r, data, t->r, r),  l = t;
}

void insert_cart (NodeCart* & t, NodeCart* it) {
    if (!t)
        t = it;
    else if (it->prior > t->prior)
        search_cart (t, it->data, it->l, it->r),  t = it;
    else
        insert_cart (it->data < t->data ? t->l : t->r, it);
}

void merge_cart (NodeCart* & t, NodeCart* l, NodeCart* r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge_cart (l->r, l->r, r),  t = l;
    else
        merge_cart (r->l, l, r->l),  t = r;
}

void erase_cart (NodeCart* & t, int data) {
    if (t->data == data)
        merge_cart (t, t->l, t->r);
    else
        erase_cart (data < t->data ? t->l : t->r, data);
}

NodeCart* unite_cart (NodeCart* l, NodeCart* r) {
    if (!l || !r)  return l ? l : r;
    if (l->prior < r->prior)  swap (l, r);
    NodeCart* lt, *rt;
    search_cart (r, l->data, lt, rt);
    l->l = unite_cart (l->l, lt);
    l->r = unite_cart (l->r, rt);
    return l;
}

NodeCart* search_cart(NodeCart* root, int data) {
    if (root == nullptr || root->data == data)
        return root;
    if (data < root->data)
        return search_cart(root->l, data);
    return search_cart(root->r, data);
}

NodeCart* build_cart(vector<int> data, vector<int> priors, int n) {
    NodeCart* t = NULL;
    for (int i = 0; i < n; ++i) {
        NodeCart* new_item = new NodeCart(data[i], priors[i]);
        insert_cart(t, new_item);
    }
    return t;
}

int main() {


    // выбор структуры данных дерева
    int choice_test = 1;

    // выбор количества данных
    int choice_count = 1000;

    // структура приоритетов
    int choice_prior = 2;

    // данные поиска
    int count_search = choice_count;

    long long result = 0;

    int count_ = 10000;

    for (int j = 0; j < count_; j++){

    vector<int> data, priors, search;

    string data_file = "data.txt";
    string search_file = "search.txt";
    string prior_file = "prior.txt";

  
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

    NodeCart* root = build_cart(data, priors, data.size());
            for (size_t i = 0; i < search.size(); i++) 
                NodeCart* result = search_cart(root, search[i]);

    // Получаем текущее время после выполнения кода
    auto stop = high_resolution_clock::now();

    // Рассчитываем разницу между текущим временем и временем до выполнения кода
    auto duration = duration_cast<microseconds>(stop - start);

    // Выводим время выполнения в микросекундах
    result += duration.count();
    }

    cout << result / count_;

    return 0;
}
                