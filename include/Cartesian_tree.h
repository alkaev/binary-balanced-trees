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


void insert_cart(NodeCart* & t, int value);
void merge_cart(NodeCart* & t, NodeCart* l, NodeCart* r);
void erase_cart(NodeCart* & t, int data);
// void delete_cart(NodeCart* root);
NodeCart* search_cart(NodeCart* root, int data);
NodeCart* build_cart(vector<int> data, int n);

#endif // CARTESIAN_TREE_H