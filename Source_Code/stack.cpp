#include "stack.h"
#include <iostream>

using namespace std;

// The ID and technician share an index. topIdx marks the latest repair (-1 means empty).
int stackPole[100];
string stackTech[100];
int topIdx = -1;

bool isStackEmpty() { return topIdx == -1; }
bool isStackFull()  { return topIdx == 99; }
int stackSize()     { return topIdx + 1; }

void push(int id, string tech) {
    if (isStackFull()) cout << "Stack Overflow!\n";
    else {
        topIdx++;
        stackPole[topIdx] = id;
        stackTech[topIdx] = tech;
    }
}

int pop() {
    if (isStackEmpty()) {
        cout << "Stack Underflow!\n";
        return -1;
    }
    return stackPole[topIdx--];
}

int peekStack() {
    if (isStackEmpty()) return -1;
    return stackPole[topIdx];
}

void displayStack() {
    if (isStackEmpty()) {
        cout << "Stack is empty (No recent repairs).\n";
        return;
    }
    cout << "--- Recent Repairs Stack (LIFO | Size: " << stackSize() << ") ---\n";
    for (int i = topIdx; i >= 0; i--)
        cout << "Pole #" << stackPole[i] << " | Tech: " << stackTech[i] << "\n";
}
