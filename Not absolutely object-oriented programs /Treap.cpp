#include<cstdio>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstddef>
#include<fstream>
#include<set>
#include <ctime>

using namespace std;

ifstream fin("movetofront.in");
ofstream fout("movetofront.out");

struct Node {
    int key;
    int priority;
    Node *left;
    Node *right;
    int inmplicit_key;
    Node(int key1) {
        inmplicit_key = 0;
        key = key1;
        priority = rand();
        left = nullptr;
        right = nullptr;
    }
};

int count(Node *t) {
    if (t != nullptr) {
        return t->inmplicit_key;
    }
    return 0;
}

void update(Node *t) {
    if (t != nullptr) {
        t->inmplicit_key = count(t->left) + count(t->right) + 1;
    }
}

void split(Node *t, int key, Node *&left, Node *&right, int add = 0) {
    if (t == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    int cur = add + count(t->left);
    if (key <= cur) {
        split(t->left, key, left, t->left, add);
        right = t;
    } else {
        split(t->right, key, t->right, right, cur+1);
        left = t;
    }
    update(t);
}

void merge(Node *&t, Node *left, Node *right) {
    if (left == nullptr || right == nullptr) {
        t = right == nullptr ? left : right;
        update(t);
        return;
    }
    if (left -> priority > right -> priority) {
        merge(left -> right, left -> right, right);
        t = left;
        update(t);
        return;
    } else {
        merge(right->left, left, right->left);
        t = right;
        update(t);
        return;
    }
}

void print(Node *t) {
    if (t == nullptr) {
        return;
    } else {
        if ((t->left == nullptr) && (t->right == nullptr)) {
            fout << t->key << " ";
            return;
        } else {
            if (t->left != nullptr) {
                print(t->left);
                fout<< t->key << " ";
            }
            if (t->right != nullptr) {
                if (t->left == nullptr) {
                    fout << t->key << " ";
                }
                print(t->right);
            }
        }
    }
}

int main() {
    bool l1 = true;
    int n, m;
    fin >> n >> m;
    Node *t = new Node(1);
    for (int i = 2; i <= n; i++) {
        Node *n = new Node(i);
        merge(t, t, n);
    }
    for (int i = 0; i < m; i++) {
        int l, r;
        fin >> l >> r;
        Node *left;
        Node *right;
        Node *tree1;
        Node *tree2;
        Node *right1;
        Node *left1;
        Node *t1, *rez;
        split(t, r, left, right);
        split(left, l-1, left1, right1);
        merge(tree1, left1, right);
        merge(tree2, right1, tree1);
        t = tree2;
    }
    print(t);
    fin.close();
    fout.close();
    return 0;
}
