#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

enum Color {RED, BLACK};

struct Node {
    int data;
    bool color;
    Node *l, *r, *parent;

    Node(int data): data(data), color(RED), l(nullptr), r(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    Node *root;

protected:
    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);

public:
    RBTree() { root = nullptr; }
    void insert(const int &n);
    void inorder();
    void levelOrder();
    void inorderHelper(Node *root);
    void printLevelOrder(Node *root);
    Node* search(int key);
    Node* searchHelper(Node* root, int key);
};

// Левый поворот
void RBTree::rotateLeft(Node *&root, Node *&pt) {
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
void RBTree::rotateRight(Node *&root, Node *&pt) {
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
void RBTree::fixViolation(Node *&root, Node *&pt) {
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
void RBTree::insert(const int &data) {
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

    // Фиксируем нарушения красно-черных свойств
    fixViolation(root, pt);
}

// Поиск элемента в дереве
Node* RBTree::searchHelper(Node* root, int key) {
    // Если дерево пустое или ключ найден в корне, возвращаем корень
    if (root == nullptr || root->data == key)
        return root;

    // Если ключ меньше значения корня, рекурсивно ищем в левом поддереве
    if (key < root->data)
        return searchHelper(root->l, key);

    // Иначе ищем в правом поддереве
    return searchHelper(root->r, key);
}

// Для поиска элемента в дереве
Node* RBTree::search(int key) {
    return searchHelper(root, key);
}

int main() {

    // Получаем текущее время до выполнения кода
    auto start = high_resolution_clock::now();
    RBTree tree;
    int keys[] = {5, 3, 8, 2, 4, 7, 9};
    int N = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0 ; i < N ; i ++){
        tree.insert(keys[i]);
    }

    // Поиск элемента
    int searchKeys[] = {1, 3, 7};
    int n = sizeof(searchKeys) / sizeof(searchKeys[0]);
    for (int i = 0 ; i < n; i++){
        Node* result = tree.search(searchKeys[i]);
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
