# CS212 Project #77 — Test Cases & Verification Evidence

## 1. Reproducing the End-to-End Test Session
From the `CS212_Streetlight_Repair_Desk` root folder:
```bash
g++ -std=c++17 Source_Code/main.cpp -o streetlight_app
./streetlight_app < Tests/sample_demo_input.txt
```

## 2. Verified Test Matrix (`Source_Code/main.cpp`)

| Test ID | Category | Menu Option & Input | Expected Output | Status |
| :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Normal CRUD | `13` (Load Sample Data), `4` (Show Inventory) | Inserts 4 poles (`50, 30, 70, 20`) into Linked List & BST and enqueues broken poles `30` and `70`. | PASS |
| **TC-02** | BST Lookup & 3 Traversals | `5` -> `20`, `3` -> `20` -> `Zamalek Road`, `6` | Finds ID `20` via BST, updates street name, and prints Inorder (`20 30 50 70`), Preorder (`50 30 20 70`), and Postorder (`20 30 70 50`). | PASS |
| **TC-03** | Empty Structures | Run `4`, `6`, `8`, `10` immediately on startup | Prints safe empty-structure messages without crashing or dereferencing `NULL`. | PASS |
| **TC-04** | Boundary & Single Item | Add single Pole `50` -> `6` -> `11` -> `2` (`50`) | Root-only BST prints `50`; deleting `50` leaves `head == NULL` and `root == NULL`. | PASS |
| **TC-05** | Invalid & Duplicate Data | Input `abc`, Choice `99`, Duplicate ID `50`, ID `-5`, Search `999` | `readInt()` clears `cin` fail state on `abc`; blocks out-of-range `99`, duplicate `50`, negative `-5`, and missing `999`. | PASS |
| **TC-06** | Integration: Repair Dispatch | `7` -> `20` -> `Broken Sensor`, then `8` -> `Eng Ahmed` | Dequeues oldest waiting Pole `30` first (FIFO), marks status `"Working"`, and pushes `#30` onto LIFO Stack top. | PASS |
| **TC-07** | Integration: Undo Repair | `10` (Undo Last Repair), then `9` | Pops Pole `30` from Stack top (LIFO), sets status back to `"Broken"`, and re-enqueues `#30` at Queue rear. | PASS |
| **TC-08** | Integration: Sync Delete | `2` -> Delete Pole `30`, then `12` (System Status) | Removes Pole `30` simultaneously from Linked List, BST, and pending Queue; updates counts in System Status. | PASS |
| **TC-09** | Sorting Algorithm | `11` (Sort by Street) | Bubble Sort orders Linked List alphabetically (`Nile Avenue`, `Ramses Street`, `Tahrir Street`, `Zamalek Road`). | PASS |
