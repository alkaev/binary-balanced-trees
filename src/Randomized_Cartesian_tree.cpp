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

random_device rrd;
mt19937 rng(rrd());
uniform_int_distribution<> distr(1, 1000000000);

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

void insert_rand_cart(NodeRC*& t, int value) {
    NodeRC* it = new NodeRC(value);
    if (!t) {
        t = it;
    } else if (distr(rng) % (cnt(t) + 1) == 0) {
        split_rand_cart(t, it->data, it->l, it->r);
        t = it;
    } else {
        insert_rand_cart(it->data < t->data ? t->l : t->r, value);
    }
    upd_cnt(t);
}

void merge_rand_cart(NodeRC* &t, NodeRC* l, NodeRC* r) {
    if (!l || !r)
        t = l ? l : r;
    else if (distr(rng) % (cnt(l) + cnt(r)) < cnt(l))
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
        insert_rand_cart(t, data[i]);
    }
    return t;
}

void delete_rand_cart(NodeRC* root) {
    if (root) {
        delete_rand_cart(root->l); 
        delete_rand_cart(root->r); 
        delete root; 
    }
}


