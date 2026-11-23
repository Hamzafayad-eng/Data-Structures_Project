#include <iostream>
#include <string>
#include <utility>
using namespace std;

// =========================================================
// CS212 Project #77: Streetlight Repair Desk
// =========================================================

// ---------------- 1. REPAIR HISTORY STACK (LIFO) ----------------
// Each position stores a repaired pole ID and the technician who handled it.
// Option 8 pushes a completed repair; Option 10 uses the top entry to reopen
// the most recent repair, so the last repair handled is the first one undone.
int stackPole[100];
string stackTech[100];
// topIdx is -1 when empty and otherwise points to the newest repair.
int topIdx = -1;

// These helpers check empty/full states and calculate the number of stored repairs.
bool isStackEmpty() { return topIdx == -1; }
bool isStackFull()  { return topIdx == 99; }
int stackSize()     { return topIdx + 1; }

// Add a completed repair and its technician at the next free stack position.
void push(int id, string tech) {
    if (isStackFull()) cout << "Stack Overflow!\n";
    else {
        topIdx++;
        stackPole[topIdx] = id;
        stackTech[topIdx] = tech;
    }
}

// Remove and return the newest repaired pole from the top of the stack.
int pop() {
    if (isStackEmpty()) {
        cout << "Stack Underflow!\n";
        return -1;
    }
    return stackPole[topIdx--];
}

// Read the newest pole ID without removing its repair history.
int peekStack() {
    if (isStackEmpty()) return -1;
    return stackPole[topIdx];
}

// Display repair history from newest to oldest, matching LIFO order.
void displayStack() {
    if (isStackEmpty()) {
        cout << "Stack is empty (No recent repairs).\n";
        return;
    }
    cout << "--- Recent Repairs Stack (LIFO | Size: " << stackSize() << ") ---\n";
    for (int i = topIdx; i >= 0; i--)
        cout << "Pole #" << stackPole[i] << " | Tech: " << stackTech[i] << "\n";
}

// ---------------- 2. WAITING REPAIR QUEUE (LINEAR ARRAY, FIFO) ----------------
// Option 7 adds a validated fault at the rear; Option 8 repairs the front pole.
// The array is linear: rearIdx does not wrap, so freed front slots are reused
// only after the queue becomes empty.
int queuePole[100];
string queueIssue[100];
// frontIdx and rearIdx mark the waiting range; both return to -1 when emptied.
int frontIdx = -1, rearIdx = -1;

// These checks detect an empty range and whether the rear reached the last slot.
bool isQueueEmpty() { return (frontIdx == -1 || frontIdx > rearIdx); }
bool isQueueFull()  { return rearIdx == 99; }
int queueSize()     { return isQueueEmpty() ? 0 : (rearIdx - frontIdx + 1); }

// Prevent the same pole from being added to the waiting queue more than once.
bool inQueue(int id) {
    if (isQueueEmpty()) return false;
    for (int i = frontIdx; i <= rearIdx; i++)
        if (queuePole[i] == id) return true;
    return false;
}

// Add a broken pole and its fault description at the FIFO rear.
void enqueue(int id, string issue) {
    if (isQueueFull()) cout << "Queue Overflow!\n";
    else {
        if (frontIdx == -1) frontIdx = 0;
        rearIdx++;
        queuePole[rearIdx] = id;
        queueIssue[rearIdx] = issue;
    }
}

// Remove the oldest waiting pole from the FIFO front.
int dequeue() {
    if (isQueueEmpty()) {
        cout << "Queue Underflow!\n";
        return -1;
    }
    int id = queuePole[frontIdx++];
    if (frontIdx > rearIdx) frontIdx = rearIdx = -1;
    return id;
}

// Return the next pole to be repaired without removing it.
int frontQueue() {
    if (isQueueEmpty()) return -1;
    return queuePole[frontIdx];
}

// Remove a deleted pole from the waiting range while keeping other entries
// and their fault descriptions in the same FIFO order.
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

// Show waiting faults from front to rear so the dispatch order is visible.
void displayQueue() {
    if (isQueueEmpty()) {
        cout << "Queue is empty (No broken lights waiting).\n";
        return;
    }
    cout << "--- Broken Lights Queue (FIFO | Size: " << queueSize() << ") ---\n";
    for (int i = frontIdx; i <= rearIdx; i++)
        cout << "Pole #" << queuePole[i] << " | Issue: " << queueIssue[i] << "\n";
}

// ---------------- 3. STREETLIGHT INVENTORY (SINGLY LINKED LIST) ----------------
// Each node is one complete pole record. The list is the source of street and
// status details; the BST indexes IDs, while queue and repair actions update
// this record as a pole moves through the maintenance workflow.
struct Node {
    int id;
    string street;
    string status; // Current state shown in the inventory and search results.
    Node* next;
};

// head points to the first inventory record, or NULL when the inventory is empty.
Node* head = NULL;

// Create a pole record and append it to the end of the inventory list.
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

// Find and return a pole record by ID; this scans the list from head to tail.
// Option 5 uses it after the BST confirms the ID, to retrieve full record details.
Node* searchList(int key) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->id == key) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Unlink and delete the matching inventory record when a pole is removed.
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

// Release every dynamically allocated inventory node when the program exits.
void clearList() {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Count current inventory records by traversing the list.
int listSize() {
    int count = 0;
    for (Node* t = head; t != NULL; t = t->next) count++;
    return count;
}

// Traverse and print every pole record currently stored in the inventory.
void displayList() {
    if (head == NULL) {
        cout << "Inventory Linked List is empty.\n";
        return;
    }
    cout << "--- Streetlight Inventory (Count: " << listSize() << ") ---\n";
    for (Node* t = head; t != NULL; t = t->next)
        cout << "ID: " << t->id << " | Street: " << t->street << " | Status: " << t->status << "\n";
}

// Bubble Sort compares street names; all fields are swapped together so an ID
// remains paired with its street and status. This is the inventory sort option.
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

// ---------------- 4. POLE ID INDEX (BINARY SEARCH TREE) ----------------
// The BST stores unique pole IDs: smaller IDs go left and larger IDs go right.
// It supports ID checks and traversals; Option 5 then scans the linked list
// when it needs the full street and status record for a found ID.
struct BSTNode {
    int data;
    BSTNode *left, *right;
};

// root points to the top of the ID index, or NULL when no poles are registered.
BSTNode* root = NULL;

// Insert a new ID into its ordered position in the BST. Duplicate IDs are ignored.
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

// Search by ID, following left or right links according to BST ordering.
BSTNode* searchBST(BSTNode* node, int key) {
    if (node == NULL || node->data == key) return node;
    if (key < node->data) return searchBST(node->left, key);
    return searchBST(node->right, key);
}

// Find the smallest ID in a subtree; deletion uses it as the successor.
BSTNode* findMin(BSTNode* node) {
    while (node != NULL && node->left != NULL) node = node->left;
    return node;
}

// Delete an ID while preserving BST order, including the two-child case.
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

// Inorder prints IDs in ascending order; preorder and postorder show other
// standard ways to visit every node in the index.
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

// Free the BST after the menu ends, deleting child nodes before their parent.
void clearBST(BSTNode* node) {
    if (node == NULL) return;
    clearBST(node->left);
    clearBST(node->right);
    delete node;
}

// Read an integer menu choice or pole ID. Invalid text is discarded and the
// user is prompted again; EOF returns 0 so the menu exits instead of looping.
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

// Read a full text field, such as a street name, issue description, or technician
// name.
string readLine() {
    string s;
    getline(cin >> ws, s);
    return s;
}

// ---------------- MAIN MENU AND WORKFLOW ----------------
// The menu connects the structures: register/delete updates the list and BST;
// faults enter the Queue, repairs move from Queue to Stack, and Undo moves a
// repair back to the Queue after checking the pole and available capacity.
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
                // A new pole must have a positive ID not already in the BST. Its
                // full record is added to the inventory, and its ID is indexed.
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
                // Remove the pole from inventory and ID index, and clear any
                // pending fault for it from the waiting Queue.
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
                // First check the ID in the BST; then find the list record to
                // display its street and current status.
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
                // Validate the pole, mark its inventory record Broken, and add
                // the fault to the Queue so repairs are handled in arrival order.
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

            case 8: {
                // Repair the oldest queued pole, update its inventory status,
                // then push the completion and technician onto the history Stack.
                if (isQueueEmpty()) cout << "No broken lights waiting!\n";
                else if (isStackFull())
                    cout << "Repair history Stack is full! Undo a repair before continuing.\n";
                else {
                    cout << "Repairing Front Pole #" << frontQueue() << " | Tech Name: ";
                    tech = readLine();
                    id = dequeue();
                    Node* pole = searchList(id);
                    if (pole != NULL) pole->status = "Working";
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
                // Undo starts at the newest Stack entry. Checks happen before
                // re-enqueueing so deleted, duplicate, or full-Queue cases are safe.
                if (isStackEmpty()) cout << "Nothing to undo!\n";
                else {
                    id = peekStack();
                    Node* pole = searchList(id);
                    if (pole == NULL) {
                        pop();
                        cout << "Cannot reopen Pole #" << id << "; it was deleted from inventory.\n";
                    } else if (inQueue(id)) {
                        pop();
                        pole->status = "Broken";
                        cout << "Pole #" << id << " is already waiting in Queue; duplicate entry avoided.\n";
                    } else if (isQueueFull()) {
                        cout << "Queue is full! The repair remains in Stack; remove a waiting item before undoing.\n";
                    } else {
                        pop();
                        pole->status = "Broken";
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
                cout << "Waiting Queue (FIFO)    : " << queueSize() << "/100"
                     << " | Empty? " << (isQueueEmpty() ? "Yes" : "No")
                     << " | Full? " << (isQueueFull() ? "Yes" : "No") << "\n";
                cout << "Repairs Stack (LIFO)    : " << stackSize() << "/100"
                     << " | Empty? " << (isStackEmpty() ? "Yes" : "No")
                     << " | Full? " << (isStackFull() ? "Yes" : "No") << "\n";
                break;

            case 13:
                // Load demonstration records into the inventory and ID index;
                // the two broken poles are also placed in the waiting Queue.
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

    clearBST(root);
    root = NULL;
    clearList();
    return 0;
}
