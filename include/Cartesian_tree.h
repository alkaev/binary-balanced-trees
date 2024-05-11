#ifndef CARTESIAN_TREE_H
#define CARTESIAN_TREE_H

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
    NodeCart();
    NodeCart(int data, int prior);
};

void search_cart(NodeCart* t, int data, NodeCart* & l, NodeCart* & r);
void insert_cart(NodeCart* & t, NodeCart* it);
void merge_cart(NodeCart* & t, NodeCart* l, NodeCart* r);
void erase_cart(NodeCart* & t, int data);
NodeCart* unite_cart(NodeCart* l, NodeCart* r);
NodeCart* search_cart(NodeCart* root, int data);
NodeCart* build_cart(vector<int> data, vector<int> priors, int n);

#endif // CARTESIAN_TREE_H

