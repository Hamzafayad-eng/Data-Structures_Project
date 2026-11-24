#ifndef QUEUE_FAULTS_WAITING_H
#define QUEUE_FAULTS_WAITING_H

#include <string>

constexpr int QUEUE_CAPACITY = 100;

extern int queuePole[QUEUE_CAPACITY];
extern std::string queueIssue[QUEUE_CAPACITY];
extern int frontIdx;
extern int rearIdx;
extern int queueCount;

bool isQueueEmpty();
bool isQueueFull();
int queueSize();
bool inQueue(int id);
void enqueue(int id, std::string issue);
int dequeue();
int frontQueue();
void removeFromQueue(int id);
void displayQueue();

#endif
