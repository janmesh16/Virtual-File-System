# 2.1 Project Title
**DiskOS: Virtual File System Engine**

---

# 2.2 Problem Statement
Modern operating systems require highly efficient, low-latency mechanisms to manage hierarchical file structures, schedule background maintenance tasks, and track historical user actions. The challenge is to build a robust, in-memory simulation of a file system that effectively bridges logical directory abstractions with systematic process scheduling without suffering from memory leaks or excessive performance overhead.

---

# 2.3 Objectives
* **Simulate Hierarchical Storage:** Create a dynamic directory and file tracking system using a custom N-ary tree.
* **Orchestrate Background Processes:** Implement a non-blocking task scheduler to handle system maintenance.
* **Enable Action Reversal:** Provide a robust "Undo" mechanism to revert recent system changes.
* **Ensure Memory Safety:** Develop a secure teardown sequence to prevent RAM leaks upon system exit.

---

# 2.4 System Overview / Architecture
DiskOS utilizes a modular, dual-layer architecture:
1. **Logical Abstraction Layer:** Manages the user-facing hierarchical tree, allowing the creation of nested folders and files, and facilitating navigation via standard commands (like `cd`).
2. **Process Management Layer:** Operates parallel to the file tree, managing asynchronous background tasks via a linear pipeline and tracking global state changes to allow instant action reversals.

---

# 2.5 Data Structures and Algorithms Used
* **N-ary Tree:** Used to build the core directory structure where each folder (node) can hold multiple subfolders and files.
* **Standard Queue (`std::queue`):** A First-In, First-Out (FIFO) structure used to queue and process background OS tasks in strict chronological order.
* **Standard Stack (`std::stack`):** A Last-In, First-Out (LIFO) structure used to maintain a history of actions, enabling the "Undo Last Action" feature.
* **Hash Map / Tree Map (`std::map`):** Used for indexing directory paths to achieve logarithmic traversal speeds.

---

# 2.6 Implementation Approach
The project is implemented in modern C++ and adheres to object-oriented programming (OOP) principles. The system is split into multiple files (e.g., `main.cpp` for the driver menu and `FileSystem.cpp` for engine logic) to separate declarations from implementations. The user interacts through a continuous `while` loop interface, which maps standard I/O inputs to specific data structure operations (e.g., pushing to a queue, allocating a tree node, or popping from a stack).

---

# 2.7 Time and Space Complexity Analysis

| Operation / Feature | Data Structure | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Create Folder / File** | N-ary Tree Node | O(1) | O(1) per node |
| **Change Directory (cd)** | Balanced Search Tree (`map`) | O(log N) | O(N) |
| **Add Background Task** | FIFO Queue (`std::queue`) | O(1) | O(T) tasks |
| **Process Task** | FIFO Queue (`std::queue`) | O(1) | O(1) |
| **Undo Last Action** | LIFO Stack (`std::stack`) | O(1) | O(A) actions |
| **Display Directory** | Dynamic Array (`std::vector`) | O(C) children | O(1) |

---

# 2.8 Execution Steps
To compile and execute the project locally, follow these terminal commands:

1. **Clone the repository:**
   `git clone https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git`
2. **Navigate to the directory:**
   `cd YOUR_REPO_NAME`
3. **Compile the source files:**
   `clang++ main.cpp FileSystem.cpp -o vfs_app`
   *(Note: Use `g++` if compiling on a Linux/Windows GCC environment).*
4. **Run the executable:**
   `./vfs_app`

---

# 2.9 Sample Inputs and Outputs

**Input Sequence:**
```text
Enter choice: 1
Enter folder name: Documents
Enter choice: 5
Enter task name: Clear_Cache
Enter choice: 6

Folder 'Documents' created successfully.
Background task 'Clear_Cache' added to queue.
Processing background task: Clear_Cache ... Done!

2.10 Screenshots

Main Menu Interface: ![<img width="1470" height="956" alt="Screenshot 2026-06-14 at 4 49 02 PM" src="https://github.com/user-attachments/assets/531297ad-1018-4628-b842-c7df3a81d5b3" />
)

Directory Display Output: ![Directory Screenshot](<img width="1470" height="956" alt="Screenshot 2026-06-14 at 4 50 44 PM" src="https://github.com/user-attachments/assets/23e0116d-404b-49c5-abb3-8999e7d0bf20" />
)

2.11 Results and Observations
Performance: The integration of std::map drastically reduced the time required to change directories compared to linear path-matching algorithms.

Reliability: The FIFO queue successfully prevented task execution overlap, mimicking a true OS background scheduler.

State Management: The stack-based undo system reliably reversed directory creations without corrupting the overall N-ary tree structure.

2.12 Conclusion
The DiskOS Virtual File System successfully demonstrates the practical application of fundamental Data Structures and Algorithms in system-level software. By combining Trees, Queues, and Stacks, the engine provides an efficient, leak-free, and highly responsive simulation of modern operating system behaviors.
