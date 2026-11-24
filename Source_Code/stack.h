#ifndef STACK_H
#define STACK_H

#include <string>

extern int stackPole[100];
extern std::string stackTech[100];
extern int topIdx;

bool isStackEmpty();
bool isStackFull();
int stackSize();
void push(int id, std::string tech);
int pop();
int peekStack();
void displayStack();

#endif
