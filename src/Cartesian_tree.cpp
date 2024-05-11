#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>

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

