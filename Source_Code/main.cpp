#include <iostream>
#include <string>
using namespace std;

// =========================================================
// CS212 Project #77: Streetlight Repair Desk
// Simple Lab 2 - Lab 6 Style (2 Structs Only: Node & BSTNode)
// =========================================================

// ---------------- 1. STACK (Recent Repairs - Lab 2) ----------------
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

// ---------------- 2. QUEUE (Broken Lights - Lab 3) ----------------
int queuePole[100];
string queueIssue[100];
int frontIdx = -1, rearIdx = -1;

bool isQueueEmpty() { return (frontIdx == -1 || frontIdx > rearIdx); }
bool isQueueFull()  { return rearIdx == 99; }
int queueSize()     { return isQueueEmpty() ? 0 : (rearIdx - frontIdx + 1); }

bool inQueue(int id) {
    if (isQueueEmpty()) return false;
    for (int i = frontIdx; i <= rearIdx; i++)
        if (queuePole[i] == id) return true;
    return false;
}

void enqueue(int id, string issue) {
    if (isQueueFull()) cout << "Queue Overflow!\n";
    else {
        if (frontIdx == -1) frontIdx = 0;
        rearIdx++;
        queuePole[rearIdx] = id;
        queueIssue[rearIdx] = issue;
    }
}

int dequeue() {
    if (isQueueEmpty()) {
        cout << "Queue Underflow!\n";
        return -1;
    }
    int id = queuePole[frontIdx++];
    if (frontIdx > rearIdx) frontIdx = rearIdx = -1;
    return id;
}

int frontQueue() {
    if (isQueueEmpty()) return -1;
    return queuePole[frontIdx];
}

void removeFromQueue(int id) {
    if (isQueueEmpty()) return;
    int j = frontIdx;
    for (int i = frontIdx; i <= rearIdx; i++) {
        if (queuePole[i] != id) {
            queuePole[j] = queuePole[i];
            queueIssue[j] = queueIssue[i];
            j++;
        }
    }
    rearIdx = j - 1;
    if (frontIdx > rearIdx) frontIdx = rearIdx = -1;
}

void displayQueue() {
    if (isQueueEmpty()) {
        cout << "Queue is empty (No broken lights waiting).\n";
        return;
    }
    cout << "--- Broken Lights Queue (FIFO | Size: " << queueSize() << ") ---\n";
    for (int i = frontIdx; i <= rearIdx; i++)
        cout << "Pole #" << queuePole[i] << " | Issue: " << queueIssue[i] << "\n";
}

// ---------------- 3. LINKED LIST (Inventory - Lab 5) ----------------
struct Node {
    int id;
    string street;
    string status; // "Working" or "Broken"
    Node* next;
};

Node* head = NULL;

void insertList(int id, string street, string status) {
    Node* newNode = new Node;
    newNode->id = id;
    newNode->street = street;
    newNode->status = status;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newNode;
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

// Bubble Sort on Linked List (Lab 5 Page 10)
void sortListByStreet() {
    if (head == NULL) {
        cout << "Inventory is empty!\n";
        return;
    }
    for (Node* cur = head; cur != NULL; cur = cur->next) {
        for (Node* idx = cur->next; idx != NULL; idx = idx->next) {
            if (cur->street > idx->street) {
                swap(cur->id, idx->id);
                swap(cur->street, idx->street);
                swap(cur->status, idx->status);
            }
        }
    }
    cout << "Sorted by Street Name!\n";
    displayList();
}

// ---------------- 4. BINARY SEARCH TREE (Pole Lookup - Lab 6) ----------------
struct BSTNode {
    int data;
    BSTNode *left, *right;
};

BSTNode* root = NULL;

BSTNode* insertBST(BSTNode* node, int key) {
    if (node == NULL) {
        BSTNode* n = new BSTNode;
        n->data = key;
        n->left = n->right = NULL;
        return n;
    }
    if (key < node->data) node->left = insertBST(node->left, key);
    else if (key > node->data) node->right = insertBST(node->right, key);
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

BSTNode* deleteBST(BSTNode* node, int key) {
    if (node == NULL) return NULL;
    if (key < node->data) node->left = deleteBST(node->left, key);
    else if (key > node->data) node->right = deleteBST(node->right, key);
    else {
        if (node->left == NULL)  { BSTNode* t = node->right; delete node; return t; }
        if (node->right == NULL) { BSTNode* t = node->left;  delete node; return t; }
        BSTNode* t = findMin(node->right);
        node->data = t->data;
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

// Safe integer input so letters never freeze cin
int readInt() {
    int x;
    while (!(cin >> x)) {
        cout << "Invalid input! Enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return x;
}

// Read text supporting spaces (e.g. "Tahrir Street", "Dead Bulb")
string readLine() {
    string s;
    getline(cin >> ws, s);
    return s;
}

// ---------------- MAIN MENU (Lab Switch-Case Style) ----------------
int main() {
    int ch, id;
    string street, issue, tech;

    do {
        cout << "\n=== STREETLIGHT REPAIR DESK (PROJECT 77) ===\n";
        cout << "1. Add Pole          2. Delete Pole       3. Update Street\n";
        cout << "4. Show Inventory    5. Search Pole (BST) 6. BST Traversals\n";
        cout << "7. Report Broken     8. Repair Next       9. Show Queue & Stack\n";
        cout << "10. Undo Last Repair 11. Sort by Street   12. System Status\n";
        cout << "13. Load Sample Data 0. Exit\nChoice: ";
        ch = readInt();

        switch (ch) {
            case 1:
                cout << "Pole ID: "; id = readInt();
                if (id <= 0 || searchBST(root, id) != NULL)
                    cout << "Invalid or Duplicate Pole ID!\n";
                else {
                    cout << "Street Name: "; street = readLine();
                    insertList(id, street, "Working");
                    root = insertBST(root, id);
                    cout << "Pole #" << id << " added!\n";
                }
                break;

            case 2:
                cout << "Pole ID to delete: "; id = readInt();
                if (searchBST(root, id) == NULL) cout << "Pole not found!\n";
                else {
                    deleteList(id);
                    root = deleteBST(root, id);
                    removeFromQueue(id);
                    cout << "Pole #" << id << " deleted!\n";
                }
                break;

            case 3:
                cout << "Pole ID to update: "; id = readInt();
                if (searchBST(root, id) == NULL) cout << "Pole not found!\n";
                else {
                    cout << "New Street Name: "; searchList(id)->street = readLine();
                    cout << "Updated!\n";
                }
                break;

            case 4:
                displayList();
                break;

            case 5:
                cout << "Pole ID to search: "; id = readInt();
                if (searchBST(root, id) != NULL) {
                    Node* p = searchList(id);
                    cout << "Found -> ID: " << p->id << " | Street: " << p->street << " | Status: " << p->status << "\n";
                } else cout << "Pole not found in BST!\n";
                break;

            case 6:
                if (root == NULL) cout << "BST is empty!\n";
                else {
                    cout << "Inorder (Sorted): "; inorder(root);
                    cout << "\nPreorder:         "; preorder(root);
                    cout << "\nPostorder:        "; postorder(root); cout << "\n";
                }
                break;

            case 7:
                cout << "Broken Pole ID: "; id = readInt();
                if (searchBST(root, id) == NULL) cout << "Pole not found!\n";
                else if (inQueue(id)) cout << "Already waiting in Queue!\n";
                else if (isQueueFull()) cout << "Queue Overflow!\n";
                else {
                    cout << "Fault Issue: "; issue = readLine();
                    searchList(id)->status = "Broken";
                    enqueue(id, issue);
                    cout << "Added to Repair Queue!\n";
                }
                break;

            case 8:
                if (isQueueEmpty()) cout << "No broken lights waiting!\n";
                else {
                    cout << "Repairing Front Pole #" << frontQueue() << " | Tech Name: ";
                    tech = readLine();
                    id = dequeue();
                    if (searchList(id) != NULL) searchList(id)->status = "Working";
                    push(id, tech);
                    cout << "Pole #" << id << " repaired & pushed to Stack!\n";
                }
                break;

            case 9:
                displayQueue();
                if (!isQueueEmpty()) cout << "Next (Front): Pole #" << frontQueue() << "\n";
                displayStack();
                if (!isStackEmpty()) cout << "Latest (Top): Pole #" << peekStack() << "\n";
                break;

            case 10:
                if (isStackEmpty()) cout << "Nothing to undo!\n";
                else {
                    id = pop();
                    if (searchList(id) != NULL) {
                        searchList(id)->status = "Broken";
                        enqueue(id, "Reopened Repair");
                        cout << "Undone! Pole #" << id << " back in Queue.\n";
                    }
                }
                break;

            case 11:
                sortListByStreet();
                break;

            case 12:
                cout << "--- System Status & Structure Sizes ---\n";
                cout << "Inventory (Linked List) : " << listSize() << (head == NULL ? " (Empty)" : " poles") << "\n";
                cout << "BST Index               : " << (root == NULL ? "Empty" : "Active") << "\n";
                cout << "Waiting Queue (FIFO)    : " << queueSize() << "/100"
                     << " | Empty? " << (isQueueEmpty() ? "Yes" : "No")
                     << " | Full? " << (isQueueFull() ? "Yes" : "No") << "\n";
                cout << "Repairs Stack (LIFO)    : " << stackSize() << "/100"
                     << " | Empty? " << (isStackEmpty() ? "Yes" : "No")
                     << " | Full? " << (isStackFull() ? "Yes" : "No") << "\n";
                break;

            case 13:
                if (searchBST(root, 50) == NULL) { insertList(50, "Tahrir Street", "Working"); root = insertBST(root, 50); }
                if (searchBST(root, 30) == NULL) { insertList(30, "Nile Avenue", "Broken");    root = insertBST(root, 30); enqueue(30, "Dead Bulb"); }
                if (searchBST(root, 70) == NULL) { insertList(70, "Ramses Street", "Broken");  root = insertBST(root, 70); enqueue(70, "Wire Fault"); }
                if (searchBST(root, 20) == NULL) { insertList(20, "Dokki Road", "Working");    root = insertBST(root, 20); }
                cout << "Sample data loaded (4 poles, 2 broken in Queue)!\n";
                break;

            case 0:
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (ch != 0);

    return 0;
}
