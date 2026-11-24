#include "bst.h"
#include <iostream>

using namespace std;

BSTNode* root = NULL;

BSTNode* insertBST(BSTNode* node, int key, Node* record) {
    if (node == NULL) {
        BSTNode* n = new BSTNode;
        n->data = key;
        n->record = record;
        n->left = n->right = NULL;
        return n;
    }
    if (key < node->data) node->left = insertBST(node->left, key, record);
    else if (key > node->data) node->right = insertBST(node->right, key, record);
    else cout << "Duplicate ID ignored!\n";
    return node;
}

BSTNode* searchBST(BSTNode* node, int key) {
    if (node == NULL || node->data == key) return node;
    if (key < node->data) return searchBST(node->left, key);
    return searchBST(node->right, key);
}

BSTNode* findMin(BSTNode* node) {
    while (node != NULL && node->left != NULL) node = node->left;
    return node;
}

// If a node has two children, use the smallest ID in its right subtree to replace it.
BSTNode* deleteBST(BSTNode* node, int key) {
    if (node == NULL) return NULL;
    if (key < node->data) node->left = deleteBST(node->left, key);
    else if (key > node->data) node->right = deleteBST(node->right, key);
    else {
        if (node->left == NULL)  { BSTNode* t = node->right; delete node; return t; }
        if (node->right == NULL) { BSTNode* t = node->left;  delete node; return t; }
        BSTNode* t = findMin(node->right);
        node->data = t->data;
        node->record = t->record;
        node->right = deleteBST(node->right, t->data);
    }
    return node;
}

void inorder(BSTNode* node) {
    if (node == NULL) return;
    inorder(node->left);
    cout << node->data << " ";
    inorder(node->right);
}

void preorder(BSTNode* node) {
    if (node == NULL) return;
    cout << node->data << " ";
    preorder(node->left);
    preorder(node->right);
}

void postorder(BSTNode* node) {
    if (node == NULL) return;
    postorder(node->left);
    postorder(node->right);
    cout << node->data << " ";
}

// The tree only points to list records, so free the tree first.
void clearBST(BSTNode* node) {
    if (node == NULL) return;
    clearBST(node->left);
    clearBST(node->right);
    delete node;
}
