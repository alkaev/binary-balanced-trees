#ifndef RANDOMIZED_CARTESIAN_TREE_H
#define RANDOMIZED_CARTESIAN_TREE_H

#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

struct NodeRC {
    int data, cnt;
    NodeRC *l, *r;
    NodeRC() {}
    NodeRC(int data);
};

int cnt(NodeRC* t);
void upd_cnt(NodeRC* t);
void split_rand_cart(NodeRC* t, int data, NodeRC* &l, NodeRC* &r);
void insert_rand_cart(NodeRC* &t, int it);
void merge_rand_cart(NodeRC* &t, NodeRC* l, NodeRC* r);
void erase_rand_cart(NodeRC* &t, int data);
void delete_rand_cart(NodeRC* root);
NodeRC* search_rand_cart(NodeRC* root, int data);
NodeRC* build_rand_cart(vector<int> data, int n);

#endif // RANDOMIZED_CARTESIAN_TREE_H