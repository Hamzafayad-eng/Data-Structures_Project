#include "repair_desk.h"
#include <iostream>

using namespace std;

// head points to the first node, or NULL if the list is empty.
Node* head = NULL;

// Append a pole record and return its address for the BST.
Node* insertList(int id, string street, string status) {
    Node* newNode = new Node;
    newNode->id = id;
    newNode->street = street;
    newNode->status = status;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        return newNode;
    }
    Node* temp = head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newNode;
    return newNode;
}

Node* searchList(int key) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->id == key) return temp;
        temp = temp->next;
    }
    return NULL;
}

void deleteList(int key) {
    Node *temp = head, *prev = NULL;
    if (temp != NULL && temp->id == key) {
        head = temp->next;
        delete temp;
        return;
    }
    while (temp != NULL && temp->id != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    delete temp;
}

void clearList() {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int listSize() {
    int count = 0;
    for (Node* t = head; t != NULL; t = t->next) count++;
    return count;
}

void displayList() {
    if (head == NULL) {
        cout << "Inventory Linked List is empty.\n";
        return;
    }
    cout << "--- Streetlight Inventory (Count: " << listSize() << ") ---\n";
    for (Node* t = head; t != NULL; t = t->next)
        cout << "ID: " << t->id << " | Street: " << t->street << " | Status: " << t->status << "\n";
}

// Re-link nodes instead of swapping their data, so BST pointers stay valid.
void sortListByStreet() {
    if (head == NULL) {
        cout << "Inventory is empty!\n";
        return;
    }
    bool swapped;
    do {
        swapped = false;
        Node* previous = NULL;
        Node* current = head;
        while (current != NULL && current->next != NULL) {
            Node* next = current->next;
            if (current->street > next->street) {
                current->next = next->next;
                next->next = current;
                if (previous == NULL) head = next;
                else previous->next = next;
                previous = next;
                swapped = true;
            } else {
                previous = current;
                current = current->next;
            }
        }
    } while (swapped);
    cout << "Sorted by Street Name!\n";
    displayList();
}
