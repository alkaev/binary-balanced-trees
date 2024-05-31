#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>

using namespace std;
using namespace std::chrono;

enum Color {RED, BLACK};

struct NodeRB {
    int data;
    bool color;
    NodeRB *l, *r, *parent;

    NodeRB(int data);
};

void rotateLeft(NodeRB *&root, NodeRB *&pt);
void rotateRight(NodeRB *&root, NodeRB *&pt);
void fixViolation(NodeRB *&root, NodeRB *&pt);
void insert_red_black(NodeRB* &root, const int &data);
NodeRB* search_red_black(NodeRB* root, int key);
NodeRB* build_red_black(vector<int> data, int n);

#endif // RB_TREE_H