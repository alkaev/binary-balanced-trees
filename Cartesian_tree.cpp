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
    int data;
    int prior;
    NodeCart* l;
    NodeCart* r;

    NodeCart(int _data) : data(_data), l(nullptr), r(nullptr) {
        // Generate a random priority between 1 and 1,000,000,000
        prior = rand() % 1000000000 + 1;
    }
};


void insert_cart(NodeCart* &t, int data) {
    if (!t) {
        t = new NodeCart(data);
    } else if (t->prior < rand() % 1000000000 + 1) { // Compare priority with a random number
        NodeCart* newNode = new NodeCart(data);
        newNode->l = t;
        t = newNode;
    } else {
        if (data < t->data) {
            insert_cart(t->l, data);
        } else {
            insert_cart(t->r, data);
        }
    }
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


NodeCart* search_cart(NodeCart* root, int data) {
    if (root == nullptr || root->data == data)
        return root;
    if (data < root->data)
        return search_cart(root->l, data);
    return search_cart(root->r, data);
}

NodeCart* build_cart(vector<int> data, int size) {
    NodeCart* root = nullptr;
    for (size_t i = 0; i < size; ++i) {
        insert_cart(root, data[i]);
    }
    return root;
}              