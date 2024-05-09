#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

struct Node {
    int data, prior;
    Node * l, * r;
    Node() { }
    Node (int data, int prior) : data(data), prior(prior), l(NULL), r(NULL) { }
};

void split (Node* t, int data, Node* & l, Node* & r) {
    if (!t)
        l = r = NULL;
    else if (data < t->data)
        split (t->l, data, l, t->l),  r = t;
    else
        split (t->r, data, t->r, r),  l = t;
}

void insert (Node* & t, Node* it) {
    if (!t)
        t = it;
    else if (it->prior > t->prior)
        split (t, it->data, it->l, it->r),  t = it;
    else
        insert (it->data < t->data ? t->l : t->r, it);
}

void merge (Node* & t, Node* l, Node* r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge (l->r, l->r, r),  t = l;
    else
        merge (r->l, l, r->l),  t = r;
}

void erase (Node* & t, int data) {
    if (t->data == data)
        merge (t, t->l, t->r);
    else
        erase (data < t->data ? t->l : t->r, data);
}

Node* unite (Node* l, Node* r) {
    if (!l || !r)  return l ? l : r;
    if (l->prior < r->prior)  swap (l, r);
    Node* lt, *rt;
    split (r, l->data, lt, rt);
    l->l = unite (l->l, lt);
    l->r = unite (l->r, rt);
    return l;
}

Node* search(Node* root, int data) {
    if (root == nullptr || root->data == data)
        return root;
    if (data < root->data)
        return search(root->l, data);
    return search(root->r, data);
}

Node* build(int* data, int* priors, int n) {
    Node* t = NULL;
    for (int i = 0; i < n; ++i) {
        Node* new_item = new Node(data[i], priors[i]);
        insert(t, new_item);
    }
    return t;
}


int main() {
    const string data_file = "test_data.txt";
    const string priors_file = "test_priors.txt";
    const string search_test_file = "search_test_data.txt";

    ifstream inputFile(data_file);
    ifstream inputFile2(search_test_file);
    ifstream inputFile3(priors_file);

    if (!inputFile.is_open() || !inputFile2.is_open() || !inputFile3.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    Node* root = nullptr;
    int data;
    int prior;

    vector<int> searchKeys;

    while (inputFile2 >> data) {
        searchKeys.push_back(data);
    }

    auto start = high_resolution_clock::now();

    while (inputFile >> data && inputFile3 >> prior) {
        insert(root, new Node(data, prior));
    }

    inputFile.close();
    inputFile2.close();
    inputFile3.close();

    for (size_t i = 0; i < searchKeys.size(); i++) {
        Node* result = search(root, searchKeys[i]);
        cout << "key " << searchKeys[i] << " is ";
        if (result == nullptr)
            cout << "not ";
        cout << "found in the tree" << endl;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    

    return 0;
}
