#include <iostream>
#include <string>
using namespace std;

// =========================================================
// CS212 Project #77: Streetlight Repair Desk
// =========================================================

// ---------------- 1. REPAIR HISTORY STACK (LIFO) ----------------
// A completed repair is kept as a pole ID and technician name at the same array
// position. Option 8 adds to the top; Option 10 reopens the most recent repair,
// so repairs are undone in reverse order from the order in which they were made.
int stackPole[100];
string stackTech[100];
// topIdx is -1 when there are no repairs; otherwise it marks the newest entry.
int topIdx = -1;

// The checks guard the 100-entry arrays; stackSize() returns the number in use.
bool isStackEmpty() { return topIdx == -1; }
bool isStackFull()  { return topIdx == 99; }
int stackSize()     { return topIdx + 1; }

// Store the ID and technician together. The caller checks capacity before dispatch.
void push(int id, string tech) {
    if (isStackFull()) cout << "Stack Overflow!\n";
    else {
        topIdx++;
        stackPole[topIdx] = id;
        stackTech[topIdx] = tech;
    }
}

// Return the top pole ID, then move topIdx down so the next repair becomes newest.
int pop() {
    if (isStackEmpty()) {
        cout << "Stack Underflow!\n";
        return -1;
    }
    return stackPole[topIdx--];
}

// Let Undo inspect the newest ID before deciding whether it is safe to pop it.
int peekStack() {
    if (isStackEmpty()) return -1;
    return stackPole[topIdx];
}

// Show the latest completed repair first, followed by older entries down to index 0.
void displayStack() {
    if (isStackEmpty()) {
        cout << "Stack is empty (No recent repairs).\n";
        return;
    }
    cout << "--- Recent Repairs Stack (LIFO | Size: " << stackSize() << ") ---\n";
    for (int i = topIdx; i >= 0; i--)
        cout << "Pole #" << stackPole[i] << " | Tech: " << stackTech[i] << "\n";
}

// ---------------- 2. WAITING REPAIR QUEUE (CIRCULAR ARRAY, FIFO) ----------------
// Option 7 places a reported fault at the rear; Option 8 takes the oldest fault
// from the front. Modulo indexing wraps around the fixed array, so a dequeue
// frees a slot that a later enqueue can reuse even while other faults are waiting.
const int QUEUE_CAPACITY = 100;
int queuePole[QUEUE_CAPACITY];
string queueIssue[QUEUE_CAPACITY];
// frontIdx and rearIdx mark the active range; queueCount distinguishes empty from full.
int frontIdx = -1, rearIdx = -1;
int queueCount = 0;

// Use queueCount for empty/full checks and to report the number of waiting faults.
bool isQueueEmpty() { return queueCount == 0; }
bool isQueueFull()  { return queueCount == QUEUE_CAPACITY; }
int queueSize()     { return queueCount; }

// Search only the queueCount occupied slots, wrapping indices when the range crosses 99.
bool inQueue(int id) {
    for (int i = 0; i < queueCount; i++) {
        int index = (frontIdx + i) % QUEUE_CAPACITY;
        if (queuePole[index] == id) return true;
    }
    return false;
}

// Add the pole ID and its issue at rearIdx; if full, leave the queue unchanged.
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

// Return the front ID, then advance frontIdx; reset both indices when the last item leaves.
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

// Peek at the next repair so the menu can show which pole will be dispatched.
int frontQueue() {
    if (isQueueEmpty()) return -1;
    return queuePole[frontIdx];
}

// Delete every matching ID from the active range. Copy survivors toward the front
// in their existing order, then set rearIdx to the last retained entry.
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

// Visit exactly queueCount slots, wrapping at the end so display order stays FIFO.
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

// ---------------- 3. STREETLIGHT INVENTORY (SINGLY LINKED LIST) ----------------
// Each node owns one pole's ID, street, and status. BST nodes keep non-owning
// pointers to these records, allowing searches and repair actions to update the
// same record that inventory display and list traversal use.
struct Node {
    int id;
    string street;
    string status; // The pole condition, such as "Working" or "Broken".
    Node* next;
};

// head starts the chain of records; NULL means the inventory currently has no poles.
Node* head = NULL;

// Allocate and append a pole record. Returning its address lets insertBST store
// an index to this exact record instead of copying its street and status.
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

// Walk from head to find an ID; Option 3 uses this search to update the street field.
Node* searchList(int key) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->id == key) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Find the ID, reconnect the surrounding links, and delete the node; this handles the head too.
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

// Free list-owned records after the BST index is cleared at program exit.
void clearList() {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Count nodes by following next links from head to the end of the inventory.
int listSize() {
    int count = 0;
    for (Node* t = head; t != NULL; t = t->next) count++;
    return count;
}

// Visit each node once and print its ID, street, and current status.
void displayList() {
    if (head == NULL) {
        cout << "Inventory Linked List is empty.\n";
        return;
    }
    cout << "--- Streetlight Inventory (Count: " << listSize() << ") ---\n";
    for (Node* t = head; t != NULL; t = t->next)
        cout << "ID: " << t->id << " | Street: " << t->street << " | Status: " << t->status << "\n";
}

// Bubble Sort compares neighboring street names and moves an out-of-order node
// by changing next links. It does not swap record contents, so each BST pointer
// continues to refer to the same pole after the list is sorted.
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

// ---------------- 4. POLE ID INDEX (BINARY SEARCH TREE) ----------------
// Keys are unique pole IDs: smaller IDs go left and larger IDs go right. Each
// tree node also points to the matching list record, so a successful search can
// return the street and status without scanning the inventory a second time.
struct BSTNode {
    int data;
    Node* record;
    BSTNode *left, *right;
};

// root points to the first BST node; it remains NULL until the first pole is added.
BSTNode* root = NULL;

// Follow the key order to insert an ID with its matching record pointer; duplicates are ignored.
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

// Compare the key at each node and follow one branch; return the node and its record pointer.
BSTNode* searchBST(BSTNode* node, int key) {
    if (node == NULL || node->data == key) return node;
    if (key < node->data) return searchBST(node->left, key);
    return searchBST(node->right, key);
}

// The leftmost node in a right subtree is the inorder successor used by deletion.
BSTNode* findMin(BSTNode* node) {
    while (node != NULL && node->left != NULL) node = node->left;
    return node;
}

// Delete the matching node while keeping the BST ordered. For a node with two
// children, copy both the inorder successor's ID and its record pointer, then
// remove that successor from the right subtree.
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

// Inorder visits left, current, right and prints IDs in sorted order. Preorder
// visits current before its children; postorder visits the children first.
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

// BST nodes only reference list records, so free the index first and the records separately.
void clearBST(BSTNode* node) {
    if (node == NULL) return;
    clearBST(node->left);
    clearBST(node->right);
    delete node;
}

// Read an integer choice or ID. If the input is not numeric, clear the stream and
// discard that line before asking again. At EOF, return 0 so the menu can exit.
int readInt() {
    int x;
    while (!(cin >> x)) {
        if (cin.eof()) return 0;
        cout << "Invalid input! Enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return x;
}

// Read an entire text field so street names, fault descriptions, and names may contain spaces.
string readLine() {
    string s;
    getline(cin >> ws, s);
    return s;
}

// ---------------- MAIN MENU AND DATA FLOW ----------------
// Adding a pole creates one list record and indexes its ID in the BST. Reporting
// a fault changes that record's status and queues its ID; Repair Next removes the
// oldest ID, updates the same record, and saves the repair on the Stack. Undo
// reverses the latest repair and returns the pole to the waiting Queue.
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
                // Reject invalid or duplicate IDs before allocating anything. The
                // returned list-node address is stored with the ID in the BST.
                cout << "Pole ID: "; id = readInt();
                if (id <= 0 || searchBST(root, id) != NULL)
                    cout << "Invalid or Duplicate Pole ID!\n";
                else {
                    cout << "Street Name: "; street = readLine();
                    Node* newPole = insertList(id, street, "Working");
                    root = insertBST(root, id, newPole);
                    cout << "Pole #" << id << " added!\n";
                }
                break;

            case 2:
                // Remove the BST node before freeing the record it points to. Also
                // remove the pole from the Queue; older Stack history is handled by Undo.
                cout << "Pole ID to delete: "; id = readInt();
                if (searchBST(root, id) == NULL) cout << "Pole not found!\n";
                else {
                    root = deleteBST(root, id);
                    deleteList(id);
                    removeFromQueue(id);
                    cout << "Pole #" << id << " deleted!\n";
                }
                break;

            case 3:
                // The BST verifies that the pole exists; the list search locates
                // the record whose street name the operator wants to change.
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

            case 5: {
                // A successful BST search provides the complete pole record directly.
                cout << "Pole ID to search: "; id = readInt();
                BSTNode* foundPole = searchBST(root, id);
                if (foundPole != NULL) {
                    Node* p = foundPole->record;
                    cout << "Found -> ID: " << p->id << " | Street: " << p->street << " | Status: " << p->status << "\n";
                } else cout << "Pole not found in BST!\n";
                break;
            }

            case 6:
                if (root == NULL) cout << "BST is empty!\n";
                else {
                    cout << "Inorder (Sorted): "; inorder(root);
                    cout << "\nPreorder:         "; preorder(root);
                    cout << "\nPostorder:        "; postorder(root); cout << "\n";
                }
                break;

            case 7:
                // Use the BST pointer to update the pole's existing list record,
                // then queue its ID and issue for FIFO dispatch.
                cout << "Broken Pole ID: "; id = readInt();
                if (searchBST(root, id) == NULL) cout << "Pole not found!\n";
                else if (inQueue(id)) cout << "Already waiting in Queue!\n";
                else if (isQueueFull()) cout << "Queue Overflow!\n";
                else {
                    cout << "Fault Issue: "; issue = readLine();
                    searchBST(root, id)->record->status = "Broken";
                    enqueue(id, issue);
                    cout << "Added to Repair Queue!\n";
                }
                break;

            case 8: {
                // Do not dequeue unless the Stack can store the completed repair.
                // Then update the same record found through the BST and save its ID
                // and technician on the repair-history Stack.
                if (isQueueEmpty()) cout << "No broken lights waiting!\n";
                else if (isStackFull())
                    cout << "Repair history Stack is full! Undo a repair before continuing.\n";
                else {
                    cout << "Repairing Front Pole #" << frontQueue() << " | Tech Name: ";
                    tech = readLine();
                    id = dequeue();
                    BSTNode* repairedPole = searchBST(root, id);
                    if (repairedPole != NULL) repairedPole->record->status = "Working";
                    push(id, tech);
                    cout << "Pole #" << id << " repaired & pushed to Stack!\n";
                }
                break;
            }

            case 9:
                displayQueue();
                if (!isQueueEmpty()) cout << "Next (Front): Pole #" << frontQueue() << "\n";
                displayStack();
                if (!isStackEmpty()) cout << "Latest (Top): Pole #" << peekStack() << "\n";
                break;

            case 10: {
                // Inspect the newest repair first. Keep it on the Stack if the queue
                // is full; otherwise update the existing record and requeue its ID.
                // Deleted poles and IDs already waiting are handled separately.
                if (isStackEmpty()) cout << "Nothing to undo!\n";
                else {
                    id = peekStack();
                    BSTNode* poleNode = searchBST(root, id);
                    if (poleNode == NULL) {
                        pop();
                        cout << "Cannot reopen Pole #" << id << "; it was deleted from inventory.\n";
                    } else if (inQueue(id)) {
                        pop();
                        poleNode->record->status = "Broken";
                        cout << "Pole #" << id << " is already waiting in Queue; duplicate entry avoided.\n";
                    } else if (isQueueFull()) {
                        cout << "Queue is full! The repair remains in Stack; remove a waiting item before undoing.\n";
                    } else {
                        pop();
                        poleNode->record->status = "Broken";
                        enqueue(id, "Reopened Repair");
                        cout << "Undone! Pole #" << id << " back in Queue.\n";
                    }
                }
                break;
            }

            case 11:
                sortListByStreet();
                break;

            case 12:
                cout << "--- System Status & Structure Sizes ---\n";
                cout << "Inventory (Linked List) : " << listSize() << (head == NULL ? " (Empty)" : " poles") << "\n";
                cout << "BST Index               : " << (root == NULL ? "Empty" : "Active") << "\n";
                cout << "Waiting Queue (FIFO)    : " << queueSize() << "/" << QUEUE_CAPACITY
                     << " | Empty? " << (isQueueEmpty() ? "Yes" : "No")
                     << " | Full? " << (isQueueFull() ? "Yes" : "No") << "\n";
                cout << "Repairs Stack (LIFO)    : " << stackSize() << "/100"
                     << " | Empty? " << (isStackEmpty() ? "Yes" : "No")
                     << " | Full? " << (isStackFull() ? "Yes" : "No") << "\n";
                break;

            case 13:
                // Add sample records only if their IDs are absent. For each new
                // broken pole, create the list record, index its pointer, and queue
                // its issue so the sample starts with a usable dispatch workflow.
                if (searchBST(root, 50) == NULL) {
                    Node* pole50 = insertList(50, "Tahrir Street", "Working");
                    root = insertBST(root, 50, pole50);
                }
                if (searchBST(root, 30) == NULL) {
                    Node* pole30 = insertList(30, "Nile Avenue", "Broken");
                    root = insertBST(root, 30, pole30);
                    enqueue(30, "Dead Bulb");
                }
                if (searchBST(root, 70) == NULL) {
                    Node* pole70 = insertList(70, "Ramses Street", "Broken");
                    root = insertBST(root, 70, pole70);
                    enqueue(70, "Wire Fault");
                }
                if (searchBST(root, 20) == NULL) {
                    Node* pole20 = insertList(20, "Dokki Road", "Working");
                    root = insertBST(root, 20, pole20);
                }
                cout << "Sample data loaded (4 poles, 2 broken in Queue)!\n";
                break;

            case 0:
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (ch != 0);

    clearBST(root);
    root = NULL;
    clearList();
    return 0;
}
