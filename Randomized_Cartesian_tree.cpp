#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

struct Node {
    int data, cnt;
    Node *l, *r;
    Node() {}
    Node(int data) : data(data), cnt(1), l(NULL), r(NULL) {}
};

int cnt(Node* t) {
    return t ? t->cnt : 0;
}

void upd_cnt(Node* t) {
    if (t)
        t->cnt = 1 + cnt(t->l) + cnt(t->r);
}

void split(Node* t, int data, Node* &l, Node* &r) {
    if (!t)
        l = r = NULL;
    else if (data < t->data)
        split(t->l, data, l, t->l), r = t;
    else
        split(t->r, data, t->r, r), l = t;
    upd_cnt(t);
}

void insert(Node* &t, Node* it) {
    if (!t)
        t = it;
    else if (rand() % (cnt(t) + 1) == 0)
        split(t, it->data, it->l, it->r), t = it;
    else
        insert(it->data < t->data ? t->l : t->r, it);
    upd_cnt(t);
}

void merge(Node* &t, Node* l, Node* r) {
    if (!l || !r)
        t = l ? l : r;
    else if (rand() % (cnt(l) + cnt(r)) < cnt(l))
        merge(l->r, l->r, r), t = l;
    else
        merge(r->l, l, r->l), t = r;
    upd_cnt(t);
}

void erase(Node* &t, int data) {
    if (t->data == data)
        merge(t, t->l, t->r);
    else
        erase(data < t->data ? t->l : t->r, data);
    upd_cnt(t);
}

Node* search(Node* root, int data) {
    // Если дерево пустое или ключ найден в корне, возвращаем корень
    if (root == nullptr || root->data == data)
        return root;

    // Если ключ меньше значения корня, рекурсивно ищем в левом поддереве
    if (data < root->data)
        return search(root->l, data);

    // Иначе ищем в правом поддереве
    return search(root->r, data);
}

Node* build(vector<int> data, int n) {
    Node* t = NULL;
    for (int i = 0; i < n; ++i) {
        Node* new_Node = new Node(data[i]);
        insert(t, new_Node);
    }
    return t;
}

int main() {
    const string data_file = "test_data.txt";
    const string search_test_file = "search_test_data.txt";
    //const int numElements = 100000;

    // Генерация тестовых данных в файл
    //generateRandomData(data_file, numElements);

    ifstream inputFile(data_file); // Открытие файла для чтения
    ifstream inputFile2(search_test_file);

    if (!inputFile.is_open() && !inputFile2.is_open()) {
        cerr << "Error opening file: " << data_file << endl;
        return 1;
    }

    
    int data;

    vector<int> searchKeys;
    vector<int> dataRoot;

    while (inputFile >> data) {
        searchKeys.push_back(data);
    }
    // Получаем текущее время до выполнения кода
    auto start = high_resolution_clock::now();

    // Чтение данных из файла и вставка их в дерево
    while (inputFile >> data) {
        dataRoot.push_back(data);
    }
    Node* root = build( dataRoot, dataRoot.size());

    inputFile.close(); // Закрытие файла после чтения данных
    inputFile2.close();
    // Поиск элементов в дереве
    
    for (int i = 0 ; i < 100000; i++){
        Node* result = search(root, searchKeys[i]);
        cout << "key " << searchKeys[i] << " is ";
        if (result == nullptr)
            cout << "not ";
        cout << "found in the tree" << endl;
    }

     // Получаем текущее время после выполнения кода
    auto stop = high_resolution_clock::now();

	// Рассчитываем разницу между текущим временем и временем до выполнения кода
    auto duration = duration_cast<microseconds>(stop - start);

    // Выводим время выполнения в микросекундах
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;



    return 0;
}
