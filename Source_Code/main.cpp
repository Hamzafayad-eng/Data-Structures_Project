#include <iostream>
#include <string>
using namespace std;

// CS212 Project #77: Streetlight Repair Desk

// Repair history is LIFO: the pole ID and technician name use the same array
// index. Option 8 adds a repair here; Undo (Option 10) takes the newest one first.
int stackPole[100];
string stackTech[100];
// topIdx is -1 when empty; otherwise it points to the latest repair.
int topIdx = -1;

// These checks keep stack operations within the fixed 100-entry arrays.
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

// Pending faults use a circular FIFO queue. Option 7 adds at the rear and Option 8
// takes from the front. queueCount distinguishes empty from full; modulo indexing
// lets the rear reuse slots freed by earlier dequeues.
const int QUEUE_CAPACITY = 100;
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

// Deletion may leave a gap in the circular range. Copy the remaining IDs and issues
// forward in FIFO order, then update queueCount and rearIdx.
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

// The linked list owns each complete pole record. BST nodes point to these same
// records, so a lookup by ID can return the street and status without copying them.
struct Node {
    int id;
    string street;
    string status; // "Working" or "Broken".
    Node* next;
};

// head points to the first pole; NULL means the inventory is empty.
Node* head = NULL;

// Append a new record and return its address so the BST can point to this node.
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

// Option 3 uses this list search to find the record whose street will be updated.
Node* searchList(int key) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->id == key) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Unlink the matching node and free it. The first-node case needs head updated separately.
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

// Free the records after clearBST has removed the non-owning index nodes.
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

// Sort by changing next links instead of swapping record fields. The BST points
// to Node objects, so keeping those addresses fixed preserves every ID-to-record link.
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

// The BST orders unique pole IDs (smaller on the left, larger on the right). Each
// tree node also holds a non-owning pointer to the matching list record, so a
// successful ID search returns the complete pole data directly.
struct BSTNode {
    int data;
    Node* record;
    BSTNode *left, *right;
};

// root is NULL until the first pole is registered.
BSTNode* root = NULL;

// Insert the ID and its matching record pointer in key order; ignore duplicate IDs.
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

// Compare IDs and follow the matching branch; the returned node includes the record pointer.
BSTNode* searchBST(BSTNode* node, int key) {
    if (node == NULL || node->data == key) return node;
    if (key < node->data) return searchBST(node->left, key);
    return searchBST(node->right, key);
}

// The leftmost node in the right subtree is the inorder successor.
BSTNode* findMin(BSTNode* node) {
    while (node != NULL && node->left != NULL) node = node->left;
    return node;
}

// For a node with two children, replace its ID and record pointer with those of
// the inorder successor, then remove the successor from the right subtree.
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

// Inorder prints IDs in ascending order. Preorder visits a node before its children;
// postorder visits it after them.
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

// BST nodes do not own the list records; clear the index before freeing the list.
void clearBST(BSTNode* node) {
    if (node == NULL) return;
    clearBST(node->left);
    clearBST(node->right);
    delete node;
}

// If input is not an integer, discard that line and ask again. At EOF, return 0
// so the menu exits instead of trying to read forever.
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

// Read a full line so street names, fault descriptions, and names can contain spaces.
string readLine() {
    string s;
    getline(cin >> ws, s);
    return s;
}

// Menu flow: add a record to the list and BST; report a fault into the Queue;
// repair the oldest fault and save it on the Stack; Undo returns the latest repair.
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
                // Check the ID first. The new list node's address is then saved with
                // the key, which lets later BST searches reach the same pole record.
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
                // Remove the index before freeing its record, then remove any waiting
                // fault. Older repair history remains on the Stack until Undo handles it.
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
                // Verify the ID in the BST, then use the list search to update its street.
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
                // Mark the existing record Broken, then queue the ID and issue for
                // repair in the order in which faults were reported.
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
                // Check Stack capacity before removing the front item. After the
                // repair, update its record and save the ID and technician together.
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
                // Check the newest repair before popping it. If its pole was deleted,
                // or is already queued, handle that case; if the Queue is full, leave
                // the repair on the Stack. Otherwise mark it Broken and enqueue it.
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
                // Add only missing sample IDs. Poles 30 and 70 start Broken, so each
                // gets a list record, a BST pointer, and an initial Queue entry.
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
