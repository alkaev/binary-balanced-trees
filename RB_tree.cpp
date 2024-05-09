#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>

using namespace std;
using namespace std::chrono;

enum Color {RED, BLACK};

struct Node {
    int data;
    bool color;
    Node *l, *r, *parent;

    Node(int data): data(data), color(RED), l(nullptr), r(nullptr), parent(nullptr) {}
};


// Левый поворот
void rotateLeft(Node *&root, Node *&pt) {
    Node *pt_right = pt->r;
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
void rotateRight(Node *&root, Node *&pt) {
    Node *pt_left = pt->l;
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
void fixViolation(Node *&root, Node *&pt) {
    Node *parent_pt = nullptr;
    Node *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        // Случай A: Родитель pt находится слева от дедушки pt
        if (parent_pt == grand_parent_pt->l) {
            Node *uncle_pt = grand_parent_pt->r;

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
            Node *uncle_pt = grand_parent_pt->l;

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
void insert(Node* & root, const int &data) {
    Node *pt = new Node(data);

    // Базовая вставка в бинарное дерево поиска
    if (root == nullptr) {
        root = pt;
        root->color = BLACK;
        return;
    }

    Node *temp = root;
    Node *parent = nullptr;

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
Node* search(Node* root, int key) {
    // Если дерево пустое или ключ найден в корне, возвращаем корень
    if (root == nullptr || root->data == key)
        return root;

    // Если ключ меньше значения корня, рекурсивно ищем в левом поддереве
    if (key < root->data)
        return search(root->l, key);

    // Иначе ищем в правом поддереве
    return search(root->r, key);
}

/*
void generateRandomData(const string& filename, int numElements) {
    ofstream file(filename);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000);

    if (file.is_open()) {
        for (int i = 0; i < numElements; ++i) {
            file << dis(gen) << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file";
    }
}
*/


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

    Node* root = nullptr;
    int data;

    vector<int> searchKeys;

    while (inputFile >> data) {
        searchKeys.push_back(data);
    }
    // Получаем текущее время до выполнения кода
    auto start = high_resolution_clock::now();

    // Чтение данных из файла и вставка их в дерево
    while (inputFile >> data) {
        insert(root, data);
    }

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
