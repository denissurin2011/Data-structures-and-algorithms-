#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
template <class ValueType>
class Set {
 public:
    struct Node {
        Node *parent, *left, *right;
        const ValueType key;
        int height;
        Node() : parent(NULL), left(NULL), right(NULL), key(0), height(1) {}
        Node(ValueType val, Node* par)
                : parent(par)
                , left(NULL)
                , right(NULL)
                , key(val)
                , height(1) {}
    };
    struct iterator {
     public:
        iterator() : tree(NULL), check_end(0) {}
        iterator(Node *other, bool l1) : tree(other), check_end(l1) {}
        iterator(const iterator& val) {
            tree = val.tree;
            check_end = val.check_end;
        }

        iterator& operator ++() {
            if (tree->right != NULL) {
                tree = tree->right;
                while (tree->left != NULL) {
                    tree = tree->left;
                }
            } else {
                Node *par = tree->parent;
                Node *par1 = tree;
                while (par != NULL && tree == par->right) {
                    tree = par;
                    par = par->parent;
                }
                if (tree->parent == NULL) {
                    if (par1->key < tree->key) {
                        return *this;
                    } else {
                        check_end = 1;
                        tree = par1;
                    }
                } else {
                    tree = tree->parent;
                }
            }
            return *this;
        }

        iterator& operator --() {
            if (check_end == true) {
                check_end = 0;
                return *this;
            }
            if (tree->left != NULL) {
                tree = tree->left;
                while (tree->right != NULL) {
                    tree = tree->right;
                }
            } else {
                Node *par = tree->parent;
                while (par != NULL && tree == par->left) {
                    tree = par;
                    par = par->parent;
                }
                tree = tree->parent;
            }
            return *this;
        }

        iterator operator ++(int) {
            iterator cur(*this);
            ++(*this);
            return cur;
        }

        iterator operator --(int) {
            iterator cur(*this);
            --(*this);
            return cur;
        }

        const ValueType& operator *() const {
            return tree->key;
        }
        const ValueType* operator ->() const {
            return &(tree->key);
        }
        bool operator ==(const iterator &other) const {
            return ((tree == NULL && other.tree == NULL))
                   || ((tree == other.tree) && (check_end == other.check_end));
        }

        bool operator !=(const iterator &other) const {
            return !(*this == other);
        }

     private:
        Node *tree;
        bool check_end;
    };

    Set() : root(nullptr), num(0), min(nullptr), max(nullptr) {}

    template <typename it>
    Set(it first, it last) : Set() {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    Set(std::initializer_list<ValueType> val) : Set() {
        for (const auto& i : val) {
            insert(i);
        }
    }

    Set(const Set &val) : Set(val.begin(), val.end()) {}

    Set &operator=(const Set &val) {
        if (&val == this) return *this;
        Set clone(val);
        swap(clone);
        return *this;
    }
    void swap(Set &clone) {
        std::swap(this->root, clone.root);
        std::swap(this->num, clone.num);
        std::swap(this->min, clone.min);
        std::swap(this->max, clone.max);
    }
    ~Set() {
        while (begin() != end()) {
            erase(*begin());
        }
    }

    size_t size() const {
        return num;
    }

    bool empty() const {
        return (num == 0);
    }

    void insert(const ValueType& val) {
        iterator i = find(val);
        if (i == end()) {
            root = insert_1(root, val);
            num++;
            min = find_const_min(root);
            max = find_max(root);
        }
        return;
    }

    void erase(const ValueType& val) {
        iterator i = find(val);
        if (i != end()) {
            root = erase_1(root, val);
            num--;
            min = find_const_min(root);
            max = find_max(root);
        }
        return;
    }

    iterator begin() const {
        return iterator(min, 0);
    }

    iterator end() const {
        return iterator(max, 1);
    }

    iterator find(const ValueType &val) const {
        Node* cur = find_el(root, val);
        if (cur == NULL) {
            return end();
        } else {
            return iterator(cur, 0);
        }
    }

    iterator lower_bound(const ValueType &val) const {
        Node* cur = find_lower_el(root, val);
        if (cur == NULL) {
            return end();
        } else if (cur->key < val) {
            return end();
        } else {
            return iterator(cur, 0);
        }
    }

 private:
    Node *root;
    size_t num = 0;
    Node *min, *max;
    Node* insert_1(Node *tree, const ValueType& val) {
        if (tree == NULL) {
            return new Node(val, nullptr);
        } else {
            if (val < tree->key) {
                tree->left = insert_1(tree->left, val);
                tree->left->parent = tree;
            } else {
                tree->right = insert_1(tree->right, val);
                tree->right->parent = tree;
            }
        }
        return balance(tree);
    }
    Node* balance(Node *tree) {
        correct_height(tree);
        if (get_bal(tree) == 2) {
            if (get_bal(tree->right) < 0 )
                tree->right = small_right_rotate(tree->right);
            return small_left_rotate(tree);
        }
        if (get_bal(tree) == -2) {
            if (get_bal(tree->left) > 0 )
                tree->left = small_left_rotate(tree->left);
            return small_right_rotate(tree);
        }
        return tree;
    }

    Node* small_left_rotate(Node *tree) {
        Node* new_tree = tree->right;
        Node* cur_parent = tree->parent;
        if (cur_parent == nullptr) {
            root = new_tree;
        } else {
            if (cur_parent->left == tree) {
                cur_parent->left = new_tree;
            } else {
                cur_parent->right = new_tree;
            }
        }
        new_tree->parent = cur_parent;
        if (new_tree->left != nullptr) {
            new_tree->left->parent = tree;
        }
        tree->parent = new_tree;
        tree->right = new_tree->left;
        new_tree->left = tree;
        correct_height(new_tree);
        correct_height(tree);
        return new_tree;
    }
    Node* small_right_rotate(Node *tree) {
        Node* new_tree = tree->left;
        Node* cur_parent = tree->parent;
        if (cur_parent == nullptr) {
            root = new_tree;
        } else {
            if (cur_parent->right == tree) {
                cur_parent->right = new_tree;
            } else {
                cur_parent->left = new_tree;
            }
        }
        new_tree->parent = cur_parent;
        if (new_tree->right != nullptr) {
            new_tree->right->parent = tree;
        }
        tree->parent = new_tree;
        tree->left = new_tree->right;
        new_tree->right = tree;
        correct_height(new_tree);
        correct_height(tree);
        return new_tree;
    }

    int get_bal(Node *tree) {
        int h_r = 0, h_l = 0;
        if (tree->left !=  NULL) {
            h_l = tree->left->height;
        }
        if (tree->right != NULL) {
            h_r = tree->right->height;
        }
        return h_r - h_l;
    }

    void correct_height(Node *tree) {
        int h_r = 0, h_l = 0;
        if (tree->left !=  NULL) {
            h_l = tree->left->height;
        }
        if (tree->right != NULL) {
            h_r = tree->right->height;
        }
        tree->height = (h_r > h_l ? h_r : h_l) + 1;
    }

    Node* erase_1(Node *tree, const ValueType& val) {
        if (val < tree->key) {
            tree->left = erase_1(tree->left, val);
            tree = balance(tree);
            if (tree->left != nullptr) tree->left->parent = tree;
        } else {
            if (tree->key < val) {
                tree->right = erase_1(tree->right, val);
                tree = balance(tree);
                if (tree->left != nullptr) tree->left->parent = tree;
            } else {
                Node* left = tree->left;
                Node* right = tree->right;
                Node* parent = tree->parent;
                if (right == nullptr) {
                    if (left != nullptr) left->parent = parent;
                    if (parent == nullptr) {
                        root = left;
                    } else {
                        if (parent->left == tree) {
                            parent->left = left;
                        } else {
                            parent->right = left;
                        }
                    }
                    delete tree;
                    return left;
                }
                Node* min_el = find_min(right);
                min_el->right = del_min(right);
                min_el->left = left;
                if (min_el->right != nullptr) {
                    min_el->right->parent = min_el;
                }
                if (min_el->left != nullptr) {
                    min_el->left->parent = min_el;
                }
                min_el->parent = parent;
                if (parent == nullptr) {
                    root = min_el;
                } else {
                    if (parent->left == tree) {
                        parent->left = min_el;
                    } else {
                        parent->right = min_el;
                    }
                }
                delete tree;
                return balance(min_el);
            }
        }
        return tree;
    }

    Node* find_min(Node *tree) {
        if (tree->left == NULL) {
            return tree;
        }
        return find_min(tree->left);
    }

    Node* del_min(Node *tree) {
        if (tree->left == NULL) {
            return tree->right;
        }
        tree->left = del_min(tree->left);
        if (tree->left != NULL) tree->left->parent = tree;
        tree = balance(tree);
        return tree;
    }

    void destroy(Node* tree) {
        if (tree == NULL) {
            return;
        }
        destroy(tree->right);
        destroy(tree->left);
        delete tree;
    }

    Node* copy(Node *tree1) {
        if (tree1 == NULL) {
            return NULL;
        }
        Node* tree = new Node(tree1->key, tree1->parent);
        tree->right = copy(tree1->right);
        tree->left = copy(tree1->left);
        if (tree->left != NULL) {
            tree->left->parent = tree;
        }
        if (tree->right != NULL) {
            tree->right->parent = tree;
        }
        return tree;
    }

    Node* find_const_min(Node* tree) const {
        while (tree != NULL && tree->left != NULL) {
            tree = tree->left;
        }
        return tree;
    }

    Node* find_max(Node *tree) const {
        while (tree != NULL && tree->right != NULL) {
            tree = tree->right;
        }
        return tree;
    }

    Node* find_el(Node *tree, const ValueType& val) const {
        if (tree == NULL || (!(val < tree->key) && !(tree->key < val))) {
            return tree;
        } else {
            if (val < tree->key) {
                return find_el(tree->left, val);
            } else {
                return find_el(tree->right, val);
            }
        }
    }
    Node* find_lower_el(Node *tree, const ValueType& val) const {
        Node *ans = tree;
        while (tree != NULL) {
            if (tree->key < val) {
                tree = tree->right;
            } else {
                ans = tree;
                tree = tree->left;
            }
        }
        return ans;
    }
};