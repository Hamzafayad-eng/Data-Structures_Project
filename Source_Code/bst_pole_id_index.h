#ifndef BST_POLE_ID_INDEX_H
#define BST_POLE_ID_INDEX_H

#include "linked_list_pole_inventory.h"

struct BSTNode {
    int data;
    Node* record;
    BSTNode *left, *right;
};

extern BSTNode* root;

BSTNode* insertBST(BSTNode* node, int key, Node* record);
BSTNode* searchBST(BSTNode* node, int key);
BSTNode* findMin(BSTNode* node);
BSTNode* deleteBST(BSTNode* node, int key);
void inorder(BSTNode* node);
void preorder(BSTNode* node);
void postorder(BSTNode* node);
void clearBST(BSTNode* node);

#endif
