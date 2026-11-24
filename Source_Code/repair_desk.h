#ifndef REPAIR_DESK_H
#define REPAIR_DESK_H

#include <string>

constexpr int QUEUE_CAPACITY = 100;

// One inventory node stores the pole's full record.
struct Node {
    int id;
    std::string street;
    std::string status;
    Node* next;
};

// The BST indexes a pole ID and points to its inventory record.
struct BSTNode {
    int data;
    Node* record;
    BSTNode *left, *right;
};

extern Node* head;
extern BSTNode* root;

bool isStackEmpty();
bool isStackFull();
int stackSize();
void push(int id, std::string tech);
int pop();
int peekStack();
void displayStack();

bool isQueueEmpty();
bool isQueueFull();
int queueSize();
bool inQueue(int id);
void enqueue(int id, std::string issue);
int dequeue();
int frontQueue();
void removeFromQueue(int id);
void displayQueue();

Node* insertList(int id, std::string street, std::string status);
Node* searchList(int key);
void deleteList(int key);
void clearList();
int listSize();
void displayList();
void sortListByStreet();

BSTNode* insertBST(BSTNode* node, int key, Node* record);
BSTNode* searchBST(BSTNode* node, int key);
BSTNode* findMin(BSTNode* node);
BSTNode* deleteBST(BSTNode* node, int key);
void inorder(BSTNode* node);
void preorder(BSTNode* node);
void postorder(BSTNode* node);
void clearBST(BSTNode* node);

#endif
