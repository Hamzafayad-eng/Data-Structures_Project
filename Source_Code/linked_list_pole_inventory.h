#ifndef LINKED_LIST_POLE_INVENTORY_H
#define LINKED_LIST_POLE_INVENTORY_H

#include <string>

struct Node {
    int id;
    std::string street;
    std::string status;
    Node* next;
};

extern Node* head;

Node* insertList(int id, std::string street, std::string status);
Node* searchList(int key);
void deleteList(int key);
void clearList();
int listSize();
void displayList();
void sortListByStreet();

#endif
