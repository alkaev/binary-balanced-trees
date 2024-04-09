#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Node {
	int key, prior;
	Node * l, * r;
	Node() { }
	Node (int key, int prior) : key(key), prior(prior), l(NULL), r(NULL) { }
};

void split (Node* t, int key, Node* & l, Node* & r) {
	if (!t)
		l = r = NULL;
	else if (key < t->key)
		split (t->l, key, l, t->l),  r = t;
	else
		split (t->r, key, t->r, r),  l = t;
}


void insert (Node* & t, Node* it) {
	if (!t)
		t = it;
	else if (it->prior > t->prior)
		split (t, it->key, it->l, it->r),  t = it;
	else
		insert (it->key < t->key ? t->l : t->r, it);
}

void merge (Node* & t, Node* l, Node* r) {
	if (!l || !r)
		t = l ? l : r;
	else if (l->prior > r->prior)
		merge (l->r, l->r, r),  t = l;
	else
		merge (r->l, l, r->l),  t = r;
}

void erase (Node* & t, int key) {
	if (t->key == key)
		merge (t, t->l, t->r);
	else
		erase (key < t->key ? t->l : t->r, key);
}

Node* unite (Node* l, Node* r) {
	if (!l || !r)  return l ? l : r;
	if (l->prior < r->prior)  swap (l, r);
	Node* lt, *rt;
	split (r, l->key, lt, rt);
	l->l = unite (l->l, lt);
	l->r = unite (l->r, rt);
	return l;
}

bool search(Node* t, int x) {
    if (!t) return false;
    if (t->key == x) return true;
    if (x < t->key) return search(t->l, x);
    else return search(t->r, x);
}

Node* build(int* keys, int* priors, int n) {
    Node* t = NULL;
    for (int i = 0; i < n; ++i) {
        Node* new_item = new Node(keys[i], priors[i]);
        insert(t, new_item);
    }
    return t;
}

int main() {
    // Зададим ключи и приоритеты для элементов дерева
    int keys[] = {5, 3, 8, 2, 4, 7, 9};
    int priors[] = {20, 30, 25, 5, 15, 10, 50}; // Примерные приоритеты, в реальности они могут быть случайными
    int n = sizeof(keys) / sizeof(keys[0]);

	// Получаем текущее время до выполнения кода
    auto start = high_resolution_clock::now();


    // Строим дерево
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        insert(root, new Node(keys[i], priors[i]));
    }

    // Выполним поиск нескольких ключей
    int searchKeys[] = {4, 7, 10}; // Ключи для поиска
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

