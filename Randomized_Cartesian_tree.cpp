#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

struct Node {
    int key, cnt;
    Node *l, *r;
    Node() {}
    Node(int key) : key(key), cnt(1), l(NULL), r(NULL) {}
};

int cnt(Node* t) {
    return t ? t->cnt : 0;
}

void upd_cnt(Node* t) {
    if (t)
        t->cnt = 1 + cnt(t->l) + cnt(t->r);
}

void split(Node* t, int key, Node* &l, Node* &r) {
    if (!t)
        l = r = NULL;
    else if (key < t->key)
        split(t->l, key, l, t->l), r = t;
    else
        split(t->r, key, t->r, r), l = t;
    upd_cnt(t);
}

void insert(Node* &t, Node* it) {
    if (!t)
        t = it;
    else if (rand() % (cnt(t) + 1) == 0)
        split(t, it->key, it->l, it->r), t = it;
    else
        insert(it->key < t->key ? t->l : t->r, it);
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

void erase(Node* &t, int key) {
    if (t->key == key)
        merge(t, t->l, t->r);
    else
        erase(key < t->key ? t->l : t->r, key);
    upd_cnt(t);
}

bool search(Node* t, int x) {
    if (!t)
        return false;
    if (t->key == x)
        return true;
    if (x < t->key)
        return search(t->l, x);
    else
        return search(t->r, x);
}

Node* build(int *keys, int n) {
    Node* t = NULL;
    for (int i = 0; i < n; ++i) {
        Node* new_Node = new Node(keys[i]);
        insert(t, new_Node);
    }
    return t;
}

int main() {

     // Получаем текущее время до выполнения кода
    auto start = high_resolution_clock::now();

    // Зададим ключи элементов дерева
    int keys[] = {5, 3, 8, 2, 4, 7, 9};
    int n = sizeof(keys) / sizeof(keys[0]);

    // Строим рандомизированное дерево поиска
    Node* root = build(keys, n);

    // Выполним поиск нескольких ключей
    int searchKeys[] = {1, 3, 7}; // Ключи для поиска
    for (int key : searchKeys) {
        cout << "Key " << key << " is ";
        if (!search(root, key)) {
            cout << "not ";
        }
        cout << "found in the tree." << endl;
    }
    // Получаем текущее время после выполнения кода
    auto stop = high_resolution_clock::now();

    // Рассчитываем разницу между текущим временем и временем до выполнения кода
    auto duration = duration_cast<microseconds>(stop - start);

    // Выводим время выполнения в микросекундах
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}