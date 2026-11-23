# CS212 Project #77 — Test Cases & Verification

## 1. Reproducing the Demo

From the project root, compile and run the supplied end-to-end session:

```bash
g++ -std=c++17 -Wall -Wextra Source_Code/main.cpp -o streetlight_app
./streetlight_app < Tests/sample_demo_input.txt
```

The sample session loads the poles, searches and updates a record, reports an additional fault, repairs the oldest queued pole, undoes that repair, sorts the inventory, deletes a waiting pole, and displays the system status.

## 2. Test Matrix

| Test ID | Category | Test steps / input | Expected result | Result |
| :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Normal CRUD | Load sample data (`13`), then display inventory (`4`). | Four poles appear in the Linked List and BST; poles `30` and `70` are waiting in the Queue. | PASS |
| **TC-02** | BST lookup and traversals | Search for `20` (`5`), update its street (`3`), then show traversals (`6`). | The BST finds the key; the Linked List provides the record details. Inorder is `20 30 50 70`; all three traversals print. | PASS |
| **TC-03** | Empty structures | At startup, run `4`, `6`, `8`, and `10`. | The inventory, BST, Queue, and Stack report their empty states without a crash. | PASS |
| **TC-04** | Single record / boundary | Add one pole, traverse and sort, then delete it. | The one-node BST and list work correctly; deleting the pole leaves both empty. | PASS |
| **TC-05** | Invalid and duplicate input | Enter `abc`, an invalid menu choice, a duplicate ID, a negative ID, and a missing search key. | The program recovers from non-numeric input and reports invalid, duplicate, or missing values. | PASS |
| **TC-06** | FIFO repair dispatch | With sample data loaded, report pole `20` as broken, then choose Repair Next. | The oldest queued pole (`30`) is repaired first, marked Working, and placed on the Stack. | PASS |
| **TC-07** | Undo repair | Undo the latest repair, then display Queue and Stack. | The repair is removed from the Stack, the pole becomes Broken, and it returns to the Queue. | PASS |
| **TC-08** | Synchronized deletion | Delete a pole that is waiting in the Queue, then show system status. | The pole is removed from the inventory, BST, and pending Queue entry. | PASS |
| **TC-09** | Bubble Sort | Sort the sample inventory by street. | The Linked List is displayed in alphabetical street order; each pole's ID, street, and status stay together. | PASS |
| **TC-10** | Full Stack and Queue | Fill the Stack with 100 repair records and request another repair; separately, fill the Queue with 100 waiting poles and try Undo. | A repair is not dequeued when the Stack is full. When the Queue is full, Undo leaves the repair on the Stack. | PASS |
| **TC-11** | Undo edge cases and EOF | Try Undo when the pole is already queued; try Undo after deleting a pole with repair history; end input while the menu is waiting. | No duplicate Queue entry is created; deleted-pole history is reported; EOF exits instead of looping forever. | PASS |

The full-capacity checks use scripted inputs to reach the 100-item limits; they are not part of the short demo file.

## 3. Complexity Notes

- Stack operations and the implemented Queue enqueue/dequeue operations use array indices and take **O(1)** time.
- Linked List search, deletion, and full-record retrieval take **O(n)** in the worst case because nodes are visited from `head`.
- BST search, insertion, and deletion take **O(log n)** when the tree is reasonably balanced and **O(n)** in the worst case when it becomes a chain. The current BST stores Pole IDs; after checking the key in the BST, the menu searches the Linked List to print the complete record, so the full display operation includes an **O(n)** scan.
- Bubble Sort on the Linked List takes **O(n²)** time.

## 4. Known Implementation Limits

The Queue is a linear array. Once `rearIdx` reaches the last array position, it cannot reuse earlier positions until the Queue becomes empty; a circular Queue would remove this limitation. The BST is not self-balancing, and pole data is held in memory only for the duration of the program.
