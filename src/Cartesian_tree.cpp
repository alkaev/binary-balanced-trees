#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <ctime>

using namespace std;
using namespace std::chrono;


random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(1, 1000000000);

struct NodeCart {
    int data;
    int prior;
    NodeCart* l;
    NodeCart* r;

    NodeCart(int _data) : data(_data), l(nullptr), r(nullptr) {
        // Generate a random priority between 1 and 1,000,000,000
        prior = dis(gen);
    }
};

void split_cart(NodeCart* t, int key, NodeCart* & l, NodeCart* & r) {
	if (!t)
		l = r = NULL;
	else if (key < t->data)
		split_cart(t->l, key, l, t->l),  r = t;
	else
		split_cart(t->r, key, t->r, r),  l = t;
}

void insert_cart (NodeCart* t, int value) {
    NodeCart* it = new NodeCart(value);
	if (t == nullptr)
		t = it;
	else if (it->prior > t->prior)
		split_cart (t, it->data, it->l, it->r),  t = it;
	else
		insert_cart(it->data < t->data ? t->l : t->r, it->data);
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
    if (root == nullptr)
        return root;
    if (root->data == data)
        return root;
    if (data < root->data)
        return search_cart(root->l, data);
    return search_cart(root->r, data);
}

NodeCart* build_cart(vector<int> data, int size) {
    NodeCart* root = nullptr;
    for (int i = 0; i < size; ++i) {
        insert_cart(root, data[i]);
    }
    return root;
}              
