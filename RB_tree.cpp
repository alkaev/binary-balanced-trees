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

enum Color {RED, BLACK};

struct NodeRB {
    int data;
    bool color;
    NodeRB *l, *r, *parent;

    NodeRB(int data): data(data), color(RED), l(nullptr), r(nullptr), parent(nullptr) {}
};


// Левый поворот
void rotateLeft(NodeRB *&root, NodeRB *&pt) {
    NodeRB *pt_right = pt->r;
    pt->r = pt_right->l;

    if (pt->r != nullptr)
        pt->r->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_right;
    else if (pt == pt->parent->l)
        pt->parent->l = pt_right;
    else
        pt->parent->r = pt_right;

    pt_right->l = pt;
    pt->parent = pt_right;
}

// Правый поворот
void rotateRight(NodeRB *&root, NodeRB *&pt) {
    NodeRB *pt_left = pt->l;
    pt->l = pt_left->r;

    if (pt->l != nullptr)
        pt->l->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_left;
    else if (pt == pt->parent->l)
        pt->parent->l = pt_left;
    else
        pt->parent->r = pt_left;

    pt_left->r = pt;
    pt->parent = pt_left;
}

// Фиксирование нарушений после вставки
void fixViolation(NodeRB *&root, NodeRB *&pt) {
    NodeRB *parent_pt = nullptr;
    NodeRB *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        // Случай A: Родитель pt находится слева от дедушки pt
        if (parent_pt == grand_parent_pt->l) {
            NodeRB *uncle_pt = grand_parent_pt->r;

            // Случай 1: Дядя красный
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                // Случай 2: pt справа от своего родителя - нужен левый поворот
                if (pt == parent_pt->r) {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                // Случай 3: pt слева от своего родителя - нужен правый поворот
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
        // Случай B: Родитель pt находится справа от дедушки pt
        else {
            NodeRB *uncle_pt = grand_parent_pt->l;

            // Случай 1: Дядя красный
            if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                // Случай 2: pt слева от своего родителя - нужен правый поворот
                if (pt == parent_pt->l) {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                // Случай 3: pt справа от своего родителя - нужен левый поворот
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
}

// Вставка нового узла
void insert_red_black(NodeRB* & root, const int &data) {
    NodeRB *pt = new NodeRB(data);

    // Базовая вставка в бинарное дерево поиска
    if (root == nullptr) {
        root = pt;
        root->color = BLACK;
        return;
    }

    NodeRB *temp = root;
    NodeRB *parent = nullptr;

    while (temp != nullptr) {
        parent = temp;
        if (pt->data < temp->data)
            temp = temp->l;
        else
            temp = temp->r;
    }

    pt->parent = parent;

    if (pt->data < parent->data)
        parent->l = pt;
    else
        parent->r = pt;

    // Фикси    м нарушения красно-черных свойств
    fixViolation(root, pt);
}

// Поиск элемента в дереве
NodeRB* search_red_black(NodeRB* root, int key) {
    // Если дерево пустое или ключ найден в корне, возвращаем корень
    if (root == nullptr || root->data == key)
        return root;

    // Если ключ меньше значения корня, рекурсивно ищем в левом поддереве
    if (key < root->data)
        return search_red_black(root->l, key);

    // Иначе ищем в правом поддереве
    return search_red_black(root->r, key);
}

NodeRB* build_red_black(vector<int> data, int n) {
    NodeRB* root = NULL;
    for (int i = 0; i < n; ++i) {
        insert_red_black(root, data[i]);
    }
    return root;
}

int main() {

    cout << "data type / count of data / count of search \n";

    vector<int> data, search;

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

    // Create Red-black tree
    NodeRB* root = build_red_black(data, data.size());

    for (size_t i = 0; i < search.size(); i++) 
        NodeRB* result = search_red_black(root, search[i]);
    
    // Получаем текущее время после выполнения кода
    auto stop = high_resolution_clock::now();

    // Рассчитываем разницу между текущим временем и временем до выполнения кода
    auto duration = duration_cast<microseconds>(stop - start);

    // Выводим время выполнения в микросекундах
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}
