# CS212 Project #77 — Test Cases & Verification

## 1. Reproducing the Demo

From the project root, compile and run the supplied end-to-end session:

```bash
g++ -std=c++17 -Wall -Wextra Source_Code/main.cpp Source_Code/stack.cpp Source_Code/queue.cpp Source_Code/linked_list.cpp Source_Code/bst.cpp -o streetlight_app
./streetlight_app < Tests/sample_demo_input.txt
```

The sample session loads the poles, searches and updates a record, reports an additional fault, repairs the oldest queued pole, undoes that repair, sorts the inventory, deletes a waiting pole, and displays system status.

## 2. Test Matrix

| Test ID | Category | Test steps / input | Expected result | Result |
| :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Normal CRUD | Load sample data (`13`), then display inventory (`4`). | Four poles appear in the Linked List and BST; poles `30` and `70` are waiting in the Queue. | PASS |
| **TC-02** | BST lookup and traversals | Search for `20` (`5`), update its street (`3`), then show traversals (`6`). | BST lookup returns the tree node and its direct pointer to the full pole record. Inorder is `20 30 50 70`; all three traversals print. | PASS |
| **TC-03** | Empty structures | At startup, run `4`, `6`, `8`, and `10`. | The inventory, BST, Queue, and Stack report their empty states without a crash. | PASS |
| **TC-04** | Single record / boundary | Add one pole, traverse and sort, then delete it. | The one-node BST and list work correctly; deleting the pole leaves both empty. | PASS |
| **TC-05** | Invalid and duplicate input | Enter `abc`, an invalid menu choice, a duplicate ID, a negative ID, and a missing search key. | The program recovers from non-numeric input and reports invalid, duplicate, or missing values. | PASS |
| **TC-06** | FIFO repair dispatch | With sample data loaded, report pole `20` as broken, then choose Repair Next. | The oldest queued pole (`30`) is repaired first, its record is marked Working, and its ID is placed on the Stack. | PASS |
| **TC-07** | Undo repair | Undo the latest repair, then display Queue and Stack. | The repair is removed from the Stack, the same inventory record becomes Broken, and the ID returns to the Queue. | PASS |
| **TC-08** | Synchronized deletion | Delete a pole that is waiting in the Queue, then show system status. | Its BST index and list record are removed, along with its pending Queue entry. | PASS |
| **TC-09** | Bubble Sort and pointer integrity | Sort the sample inventory, then search for each pole by ID. | The list is alphabetical and each BST record pointer still returns the matching ID, street, and status. | PASS |
| **TC-10** | Full Stack and Queue | Fill the Stack with 100 repair records and request another repair; separately fill the Queue and try Undo. | A repair is not dequeued when the Stack is full. When the Queue is full, Undo leaves the repair on the Stack. | PASS |
| **TC-11** | Undo edge cases and EOF | Undo a pole already in Queue; undo after deleting a pole with repair history; end input at the menu. | No duplicate Queue entry is added; deleted-pole history is reported; EOF exits safely. | PASS |
| **TC-12** | Circular Queue wraparound | Fill the Queue, dequeue its front, enqueue another pole, then delete a pole from the wrapped range. | Freed slots are reused, deletion preserves FIFO order, and the Queue can accept another valid pole without becoming empty. | PASS |

The capacity and wraparound checks use scripted inputs; they are longer than the short demo file.

## 3. Complexity Notes

- Stack operations use direct array indices and take **O(1)** time.
- Circular Queue enqueue, dequeue, front, empty/full checks, and size use indices/count and take **O(1)** time. `inQueue` and synchronized removal scan the active queue and take **O(q)** time, where `q` is the number of waiting faults.
- Linked List append, search, deletion, and traversal take **O(n)** in the worst case because records are reached by following links from `head`.
- BST operations take **O(h)**, where `h` is tree height: **O(log n)** when balanced and **O(n)** in the worst case when the tree becomes a chain. A successful BST search returns a pointer to the associated list record, so Option 5 does not perform an additional linked-list scan.
- Bubble Sort re-links adjacent list nodes and takes **O(n²)** time. Re-linking preserves node addresses, so record pointers stored in the BST remain valid after sorting.

## 4. Known Implementation Limits

The circular Queue has a fixed capacity of 100 entries. The BST is not self-balancing, so sorted input can create a chain and slow its operations. All records remain in memory only while the program runs. The implementation is split by role: `main.cpp` contains the menu, `stack.cpp` and `queue.cpp` implement the work lists, `linked_list.cpp` manages pole records, and `bst.cpp` manages the BST. Each structure has its own header for shared declarations: `stack.h`, `queue.h`, `linked_list.h`, and `bst.h`.
