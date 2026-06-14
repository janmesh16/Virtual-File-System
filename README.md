DiskOS: Virtual File System Allocation Engine
An advanced, high-fidelity in-memory simulation of an operating system storage sub-architecture. Architected in C++, DiskOS serves as a functional implementation of the bridge between logical user abstractions (hierarchical directories) and bare-metal physical storage structures (contiguous disk sectors).

Drawing engineering inspirations from file systems such as Linux's ext4 and Windows' NTFS, this engine showcases the orchestration of multiple foundational Data Structures and Algorithms (DSA) working simultaneously to solve core OS problems: linear directory scaling bottlenecks, background task scheduling overheads, fragmented block prioritization, constant-time metadata retrieval, and optimal layout sector management.

🏛 Structural Architecture & System Topology
DiskOS decouples operations into a distinct dual-layer topology. This completely prevents synchronization corruption by separating how a user views a file path from how the hardware storage controller maps data sectors across raw block arrays.

Plaintext
========================================================================================
                                   LOGICAL VISUALIZATION LAYER
========================================================================================
                                           Root (/) 
                                        [Folder Node]
                                       /             \
                                      /               \
                              Documents/               Photos/
                            [Folder Node]           [Folder Node]
                            /           \                |
                           /             \               |
                     resume.pdf       report.docx   vacation.png
                     [File Specs]     [File Specs]   [File Specs]
                          │                │              │
==========================┼────────────────┼──────────────┼=============================
                          ▼                ▼              ▼
========================================================================================
                          PHYSICAL ALLOCATION & BLOCK CONTROL MATRIX
========================================================================================
    [X] = Sector Occupied (True)                      [.] = Sector Free / Contiguous (False)
    
    00-19:  [X][X][X][X][X][.][.][.][.][.][.][.][.][.][.][X][X][X][X][X]  <- (Sectors 0-19)
    20-39:  [.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.]  <- (Sectors 20-39)
    40-59:  [X][X][X][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.]  <- (Sectors 40-59)
    60-79:  [.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.]  <- (Sectors 60-79)
    80-99:  [.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.][.]  <- (Sectors 80-99)
========================================================================================
The Logical Layer (Hierarchical Tree): Manages directory tracking, nested subdirectories, naming parameters, and metadata attributes without needing to know where the bytes physically exist on the storage disk.

The Physical Layer (Allocation Matrix): A low-level bounded sequence mapping physical block indices (00 to 99). Handles sector indexing, continuity integrity, and physical fragmentation scores.

⚙️ Subsystem Breakdown (The 13 Core Runtime Options)
The system exposes an interactive administrative command menu comprising 13 specialized functional routines:

1. Create Folder
Data Structure: N-ary Tree Node Initialization.

Mechanism: Dynamically allocates a unique heap-bound Folder instance within RAM. It inserts the newly spawned node reference into the current working directory's child tracking pointer array and establishes a structural uplink (parent*) back to the originating directory node to support future upstream nested path traversals (..).

2. Create File (Auto Contiguous Block Allocation)
Data Structure: First-Fit Contiguous Sector Placement Strategy.

Mechanism: Translates abstract logical files into bounded hardware entities. The engine enforces an OS allocation limit: every 10KB of user file payload requires 1 physical block sector. It performs a linear scan across the 100-sector logical disk array to discover the first sequence of unallocated contiguous blocks sufficient to store the total size. If verified, it tags the sectors as occupied (true) and writes the boundary properties (startBlock and blockCount) straight to the file's primary descriptor token.

3. Display Current Directory
Data Structure: Multi-Vector Linear Iteration.

Mechanism: Queries the active directory folder context and loops through its collections of subfolder references and file pointers. If no nodes are registered, the output catches the state and prints clean notification signals ((none)).

4. Change Directory (cd)
Data Structure: Logarithmic Balanced Search Tree Index Mapping.

Mechanism: Rather than forcing a slow linear path-matching traversal from the root down to deep nested levels, DiskOS queries a specialized std::map index. This map stores global string paths mapped directly to folder memory points. This achieves high-speed directory changes in O(log N) time complexity. It also handles the parent navigation keyword (..) to drop down one level in constant time.

5. Add Background Task (FIFO Queue)
Data Structure: Standard FIFO Ring Buffer Insertion.

Mechanism: Simulates asynchronous operating system maintenance pipelines (such as system log syncs, cache purges, or routine indexing). Tasks are injected into the back boundary of a linear pipeline queue (std::queue), locking their chronological execution order.

6. Process Background Task
Data Structure: FIFO Element Dequeue & Processing.

Mechanism: Pulls the oldest pending maintenance item from the head boundary of the scheduling pipeline queue. It executes the task simulator and frees the space. If option 6 is executed when no tasks exist, it guards against crash conditions by throwing an explicit state alert (Formatting queue is empty).

7. Report Disk Block Health
Data Structure: Max-Heap Ingestion Engine.

Mechanism: Collects telemetry metrics detailing storage block corruption levels or fragmentation scores (on a strict scale from 0 to 100%). These entries are pushed into a high-performance priority sorting heap (std::priority_queue).

8. Run Smart Defragmenter
Data Structure: Max-Heap Extract-Max Routine (O(log K) Sorting).

Mechanism: Modern OS defragmenters must optimize hardware lifetimes by prioritizing the messiest sectors first. DiskOS pulls the absolute top element from its priority heap matrix. Because of internal binary max-heap rules, the top node is mathematically guaranteed to hold the highest fragmentation percentage, focusing processing power exactly where it is most desperately needed.

9. Instant File Lookup by ID
Data Structure: Constant Time O(1) Hash Table Lookup.

Mechanism: Bypasses the directory structure completely. It runs a hash function against the provided string unique key within a master global tracking container (std::unordered_map). If the file ID is valid, it retrieves the block location boundaries instantly, matching how modern Master File Tables (MFT) retrieve information without iterating through folders.

10. Fastest Search by Name
Data Structure: Tree-Based Breadth-First Search (BFS) / Level-Order Traversal.

Mechanism: When searching for a file by name without knowing its path, the engine initiates a queue-driven BFS search. Starting from the absolute root directory (/), it scans every file in the current tier before pushing child subfolders into an internal search queue to advance to the next level down. This tracks the shortest structural directory path to the requested file name.

11. Delete File (Triggers Deletion Balancer)
Data Structure: Dynamic Contiguous Memory Reclaiming and Deregistration.

Mechanism: Locates the file inside the local directory vector array, references its hardware boundaries (startBlock to startBlock + blockCount), and resets those physical disk sectors back to false (free). The Deletion Balancer algorithm ensures that blocks are wiped cleanly in sequence, instantly creating maximum unified contiguous storage availability for new incoming writes. It then wipes the file from the global Hash Table and deletes the raw pointer to prevent memory leaks.

12. View Physical Disk Layout Matrix
Data Structure: State Vector Bitmask Visualization.

Mechanism: Renders a comprehensive 20 x 5 textual visual grid representing the 100 physical disk sectors. Occupied sectors print as [X], while free contiguous sectors print as [.], providing an immediate visual snapshot of drive utilization and fragmentation layout.

13. Exit (Graceful Systemic Shutdown)
Data Structure: Post-Order Recursive Memory Disassembly.

Mechanism: Shuts down the operating system engine safely. To guarantee absolute compliance with robust C++ resource standards, it calls an explicit recursive tracking routine (clearTree) that clears memory from the bottom leaf elements up to the root, completely purging all allocated objects from RAM before ending the execution context.
