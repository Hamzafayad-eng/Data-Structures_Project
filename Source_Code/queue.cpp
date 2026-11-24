#include "queue.h"
#include <iostream>

using namespace std;

// Faults leave in report order. Modulo wraps the indices; queueCount tracks used slots.
int queuePole[QUEUE_CAPACITY];
string queueIssue[QUEUE_CAPACITY];

int frontIdx = -1, rearIdx = -1;
int queueCount = 0;

bool isQueueEmpty() { return queueCount == 0; }
bool isQueueFull()  { return queueCount == QUEUE_CAPACITY; }
int queueSize()     { return queueCount; }

bool inQueue(int id) {
    for (int i = 0; i < queueCount; i++) {
        int index = (frontIdx + i) % QUEUE_CAPACITY;
        if (queuePole[index] == id) return true;
    }
    return false;
}

void enqueue(int id, string issue) {
    if (isQueueFull()) cout << "Queue Overflow!\n";
    else {
        if (isQueueEmpty()) frontIdx = rearIdx = 0;
        else rearIdx = (rearIdx + 1) % QUEUE_CAPACITY;
        queuePole[rearIdx] = id;
        queueIssue[rearIdx] = issue;
        queueCount++;
    }
}

int dequeue() {
    if (isQueueEmpty()) {
        cout << "Queue Underflow!\n";
        return -1;
    }
    int id = queuePole[frontIdx];
    queueCount--;
    if (isQueueEmpty()) frontIdx = rearIdx = -1;
    else frontIdx = (frontIdx + 1) % QUEUE_CAPACITY;
    return id;
}

int frontQueue() {
    if (isQueueEmpty()) return -1;
    return queuePole[frontIdx];
}

// Keep the remaining faults in FIFO order after removing this ID.
void removeFromQueue(int id) {
    int kept = 0;
    int oldCount = queueCount;
    for (int i = 0; i < oldCount; i++) {
        int readIndex = (frontIdx + i) % QUEUE_CAPACITY;
        if (queuePole[readIndex] != id) {
            int writeIndex = (frontIdx + kept) % QUEUE_CAPACITY;
            queuePole[writeIndex] = queuePole[readIndex];
            queueIssue[writeIndex] = queueIssue[readIndex];
            kept++;
        }
    }
    queueCount = kept;
    if (isQueueEmpty()) frontIdx = rearIdx = -1;
    else rearIdx = (frontIdx + queueCount - 1) % QUEUE_CAPACITY;
}

void displayQueue() {
    if (isQueueEmpty()) {
        cout << "Queue is empty (No broken lights waiting).\n";
        return;
    }
    cout << "--- Broken Lights Queue (FIFO | Size: " << queueSize() << ") ---\n";
    for (int i = 0; i < queueCount; i++) {
        int index = (frontIdx + i) % QUEUE_CAPACITY;
        cout << "Pole #" << queuePole[index] << " | Issue: " << queueIssue[index] << "\n";
    }
}
