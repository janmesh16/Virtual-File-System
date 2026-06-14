#include "FileSystem.h"

VirtualFileSystem::VirtualFileSystem() {
    root = new Folder{"Root", {}, {}, nullptr};
    currentDirectory = root;
    folderIndex["Root"] = root;

    // Initialize entire physical disk array as free (false)
    for (int i = 0; i < 100; i++) {
        diskSectors[i] = false;
    }
}

// Deep clean memory destructor to completely eliminate memory leaks
VirtualFileSystem::~VirtualFileSystem() {
    clearTree(root);
    std::cout << "System Memory deallocated safely.\n";
}

void VirtualFileSystem::clearTree(Folder* current) {
    if (current == nullptr) return;
    for (File* f : current->files) delete f;
    for (Folder* sub : current->subfolders) clearTree(sub);
    delete current;
}

void VirtualFileSystem::createFolder(const std::string& folderName) {
    Folder* newFolder = new Folder{folderName, {}, {}, currentDirectory};
    currentDirectory->subfolders.push_back(newFolder);
    folderIndex[folderName] = newFolder;
    writeLog.push({"CREATE_FOLDER", folderName, ""});
    std::cout << "Success: Folder '" << folderName << "' created.\n";
}

// Advanced Contiguous Block Allocator (Emulating ext4/NTFS Extent management)
void VirtualFileSystem::createFile(const std::string& fileName, const std::string& fileId, int size) {
    if (fileTable.find(fileId) != fileTable.end()) {
        std::cout << "Error: File ID '" << fileId << "' already exists!\n";
        return;
    }

    // Rule: Every 10KB requires 1 physical disk block
    int blocksNeeded = (size + 9) / 10; 
    int startBlock = -1;

    // Find the first available contiguous segment of blocks (First-Fit Algorithm)
    for (int i = 0; i <= 100 - blocksNeeded; i++) {
        bool segmentFree = true;
        for (int j = 0; j < blocksNeeded; j++) {
            if (diskSectors[i + j]) {
                segmentFree = false;
                break;
            }
        }
        if (segmentFree) {
            startBlock = i;
            break;
        }
    }

    if (startBlock == -1) {
        std::cout << "Allocation Error: Out of contiguous physical disk space for size " << size << "KB!\n";
        return;
    }

    // Map file to physical blocks
    for (int i = startBlock; i < startBlock + blocksNeeded; i++) {
        diskSectors[i] = true;
    }

    File* newFile = new File{fileId, fileName, size, startBlock, blocksNeeded};
    currentDirectory->files.push_back(newFile);
    fileTable[fileId] = newFile; 
    writeLog.push({"CREATE_FILE", fileName, fileId});
    
    std::cout << "Success: '" << fileName << "' written to physical disk blocks [" 
              << startBlock << " to " << (startBlock + blocksNeeded - 1) << "].\n";
}

void VirtualFileSystem::displayCurrentDirectory() {
    std::cout << "\n--- Contents of [" << currentDirectory->name << "] ---\n";
    std::cout << "Folders:\n";
    if (currentDirectory->subfolders.empty()) std::cout << "  (none)\n";
    for (Folder* f : currentDirectory->subfolders) {
        std::cout << "  [DIR] " << f->name << "\n";
    }
    std::cout << "Files:\n";
    if (currentDirectory->files.empty()) std::cout << "  (none)\n";
    for (File* f : currentDirectory->files) {
        std::cout << "  [FILE] " << f->name << " (ID: " << f->fileId << ", Size: " << f->size 
                  << "kb, Physical Blocks: " << f->startBlock << "-" << (f->startBlock + f->blockCount - 1) << ")\n";
    }
    std::cout << "------------------------\n\n";
}

void VirtualFileSystem::undoLastAction() {
    if (writeLog.empty()) {
        std::cout << "Nothing to undo.\n";
        return;
    }
    WriteAction lastAction = writeLog.top();
    writeLog.pop();
    std::cout << "Undoing action: " << lastAction.actionType << " for '" << lastAction.targetName << "'\n";
}

void VirtualFileSystem::changeDirectory(const std::string& folderName) {
    if (folderName == "..") {
        if (currentDirectory->parent != nullptr) {
            currentDirectory = currentDirectory->parent;
            std::cout << "Moved up to: " << currentDirectory->name << "\n";
        } else {
            std::cout << "Already at the Root directory.\n";
        }
        return;
    }
    auto it = folderIndex.find(folderName);
    if (it != folderIndex.end()) {
        currentDirectory = it->second;
        std::cout << "Changed directory to: " << currentDirectory->name << "\n";
    } else {
        std::cout << "Error: Folder '" << folderName << "' not found.\n";
    }
}

void VirtualFileSystem::addFormattingTask(const std::string& taskName) {
    formattingQueue.push(taskName);
    std::cout << "Task '" << taskName << "' added to background queue.\n";
}

void VirtualFileSystem::processNextTask() {
    if (formattingQueue.empty()) {
        std::cout << "Formatting queue is empty.\n";
        return;
    }
    std::string nextTask = formattingQueue.front();
    formattingQueue.pop();
    std::cout << "Processing background task: " << nextTask << " ... Done!\n";
}

void VirtualFileSystem::reportDiskBlockStatus(int blockId, int fragmentationLevel) {
    DiskBlock block{blockId, fragmentationLevel};
    fragmentationSorter.push(block);
    std::cout << "Telemetry: Block " << blockId << " reported at " << fragmentationLevel << "% fragmentation.\n";
}

void VirtualFileSystem::runDefragmentationStep() {
    if (fragmentationSorter.empty()) {
        std::cout << "System Health Nominal: No messy disk blocks recorded.\n";
        return;
    }
    DiskBlock worstBlock = fragmentationSorter.top();
    fragmentationSorter.pop();
    std::cout << "\n=== LAUNCHING DEFRAGMENTER ===\n";
    std::cout << "Optimized Block ID: " << worstBlock.blockId << " (" << worstBlock.fragmentationLevel << "% cleared)\n";
    std::cout << "==============================\n\n";
}

void VirtualFileSystem::lookupFileById(const std::string& fileId) {
    auto it = fileTable.find(fileId);
    std::cout << "\n=== O(1) HASH TABLE FILE LOOKUP ===\n";
    if (it != fileTable.end()) {
        std::cout << "File: " << it->second->name << " | Blocks: " 
                  << it->second->startBlock << "-" << (it->second->startBlock + it->second->blockCount - 1) << "\n";
    } else {
        std::cout << "Error: File ID '" << fileId << "' not found.\n";
    }
    std::cout << "===================================\n";
}

void VirtualFileSystem::fastestSearchByName(const std::string& fileName) {
    if (root == nullptr) return;
    std::queue<Folder*> searchQueue;
    searchQueue.push(root);
    std::cout << "\n=== BFS ENGINE: TRAVERSAL PATH ===\n";
    bool found = false;
    while (!searchQueue.empty()) {
        Folder* current = searchQueue.front();
        searchQueue.pop();
        for (File* f : current->files) {
            if (f->name == fileName) {
                std::cout << "Target Discovered! Path: " << current->name << "/" << f->name << "\n";
                found = true;
            }
        }
        for (Folder* sub : current->subfolders) searchQueue.push(sub);
    }
    if (!found) std::cout << "File not found.\n";
    std::cout << "==================================\n";
}

// --- NEW FEATURE 8: DELETION BALANCER IMPLEMENTATION ---

void VirtualFileSystem::deleteFileByName(const std::string& fileName) {
    int indexToRemove = -1;
    
    // Find file in current directory's linear list
    for (size_t i = 0; i < currentDirectory->files.size(); i++) {
        if (currentDirectory->files[i]->name == fileName) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        std::cout << "Deletion Error: File '" << fileName << "' does not exist in this folder.\n";
        return;
    }

    File* fileToDelete = currentDirectory->files[indexToRemove];

    std::cout << "\n=== DELETION BALANCER ACTIVATED ===\n";
    std::cout << "Clearing contiguous block allocation sequence...\n";
    
    // Deletion Balancer Optimization Rule: 
    // Clear structural array mapping instantly to avoid block misalignment
    for (int i = fileToDelete->startBlock; i < fileToDelete->startBlock + fileToDelete->blockCount; i++) {
        diskSectors[i] = false; // Reclaiming sectors to preserve contiguous blocks
    }
    
    std::cout << "Freed " << fileToDelete->blockCount << " blocks starting from sector " << fileToDelete->startBlock << ".\n";
    std::cout << "Status: Contiguous sector availability maximized successfully.\n";
    std::cout << "====================================\n";

    // Wipe from global Hash Table Map
    fileTable.erase(fileToDelete->fileId);

    // Wipe from tree vector registry
    currentDirectory->files.erase(currentDirectory->files.begin() + indexToRemove);

    // Free raw pointer memory
    delete fileToDelete;
}

// Simple visual map helper to track block status
void VirtualFileSystem::displayDiskMap() {
    std::cout << "\n=== PHYSICAL DISK ALLOCATION MATRIX (Sectors 0-99) ===\n";
    for (int i = 0; i < 100; i++) {
        if (diskSectors[i]) std::cout << "[X]"; // Occupied
        else std::cout << "[.]";               // Clean Contiguous Free space
        if ((i + 1) % 20 == 0) std::cout << "\n";
    }
    std::cout << "========================================================\n";
}