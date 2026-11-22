# CS212 – Data Structures Course Project
## Project #77: Streetlight Repair Desk

**Registered Practical Section:** CRN: 7182  
**Team ID:** Team 77  

### Team Members (CRN: 7182)
1. **Mohamed Ahmed Abdelkhalek Gohary** — `200038102`
2. **Yusuf Ahmed Muhammad Sayed** — `200067726`
3. **Bilal Mahmoud Fahmy Mahmoud** — `200069114`
4. **Abdelrahman Salah Ibrahim Abdelwahab** — `200070046`
5. **Abdul-Azim Ibrahim Abdul-Azim Ibrahim Jabr** — `200073612`
6. **Hamza Abdelfattah Elazab Fayad** — `200083256`

---

## 1. Project Overview & Manual Data Structures
An integrated, menu-driven C++ console application implementing all four required data structures and sorting **100% manually** (following CS212 Labs 2–6 conventions, without `std::stack`, `std::queue`, `std::list`, `std::map`, or ready-made tree structures):

- **Stack (`stackPole[100]`, `topIdx` — Lab 2 Style):** Recent completed repairs history (**LIFO**) + Undo/Reopen last repair.
- **Queue (`queuePole[100]`, `frontIdx`, `rearIdx` — Lab 3 Style):** Broken streetlights awaiting technicians (**FIFO**).
- **Singly Linked List (`struct Node* head` — Lab 5 Style):** Dynamic streetlight inventory records (`id`, `street`, `status`).
- **Binary Search Tree (`struct BSTNode* root` — Lab 6 Style):** Fast Pole ID lookup (`insertBST`, `searchBST`, `deleteBST`, `inorder`, `preorder`, `postorder`).
- **Sorting (`sortListByStreet` — Lab 5 Style):** Bubble Sort on the Linked List alphabetically by Street Name.

---

## 2. Project Directory Layout
```text
CS212_Streetlight_Repair_Desk/
├── README.md
├── Source_Code/
│   └── main.cpp
├── Report/
│   └── CS212_Project_77_Streetlight_Repair_Desk_Report.docx
├── Screenshots/
│   ├── code_01_linked_list.png
│   ├── code_02_bst.png
│   ├── code_03a_queue.png
│   ├── code_03b_stack.png
│   ├── code_04_bubble_sort.png
│   ├── exec_01_menu_and_inventory.png
│   ├── exec_02_bst_lookup_traversals.png
│   ├── exec_03_integrated_queue_stack_undo.png
│   ├── exec_04_validation_edge_cases.png
│   └── exec_05_sorting_and_sync_delete.png
└── Tests/
    ├── test_cases_documentation.md
    └── sample_demo_input.txt
```

---

## 3. How to Compile and Run

### Using `g++` (Linux / macOS / Windows MinGW)
```bash
g++ -std=c++17 Source_Code/main.cpp -o streetlight_app
./streetlight_app
```
*(On Windows Command Prompt / PowerShell, run `streetlight_app.exe`).*

### Using an IDE (Code::Blocks / Dev-C++ / Visual Studio / CLion)
1. Open `Source_Code/main.cpp` in a C++ Console Application project.
2. Click **Build and Run**.

### Automated Demo Script
To reproduce the tested end-to-end workflow automatically:
```bash
./streetlight_app < Tests/sample_demo_input.txt
```
