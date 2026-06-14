#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>

struct Folder; 

// Simulated Physical Disk Block
struct DiskBlock {
    int blockId;
    int fragmentationLevel; 

    bool operator<(const DiskBlock& other) const {
        return fragmentationLevel < other.fragmentationLevel;
    }
};

// Upgraded File Structure mapped to physical disk blocks
struct File {
    std::string fileId;
    std::string name;
    int size;
    int startBlock; // Real-world allocation (Start of Contiguous Space)
    int blockCount; // Number of sequential blocks occupied
};

// Folder Structure (N-ary Tree Node)
struct Folder {
    std::string name;
    std::vector<Folder*> subfolders;
    std::vector<File*> files;
    Folder* parent; 
};

// Write History Action
struct WriteAction {
    std::string actionType; 
    std::string targetName;
    std::string extraData; // Used to store IDs for structural rollbacks
};

class VirtualFileSystem {
private:
    Folder* root; 
    Folder* currentDirectory;

    std::map<std::string, Folder*> folderIndex;
    std::unordered_map<std::string, File*> fileTable; 
    std::stack<WriteAction> writeLog;
    std::queue<std::string> formattingQueue;
    std::priority_queue<DiskBlock> fragmentationSorter;

    // Physical Disk Layout (Simulating 100 Blocks of Storage)
    // false = Free space, true = Occupied by a file
    bool diskSectors[100]; 

    // Helper function for memory management
    void clearTree(Folder* current);

public:
    VirtualFileSystem();
    ~VirtualFileSystem();

    void createFolder(const std::string& folderName);
    void createFile(const std::string& fileName, const std::string& fileId, int size);
    void displayCurrentDirectory();
    void undoLastAction();
    void changeDirectory(const std::string& folderName);
    void addFormattingTask(const std::string& taskName);
    void processNextTask();
    void reportDiskBlockStatus(int blockId, int fragmentationLevel);
    void runDefragmentationStep(); 
    void lookupFileById(const std::string& fileId);
    void fastestSearchByName(const std::string& fileName);

    // NEW Feature 8: Deletion Balancer & Core File Removal
    void deleteFileByName(const std::string& fileName);
    void displayDiskMap(); // Visualizes the physical disk allocation
};

#endif