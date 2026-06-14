#include <iostream>
#include "FileSystem.h"

int main() {
    VirtualFileSystem vfs;
    int choice;

    std::cout << "=== DiskOS Virtual File System Started ===\n";

    while (true) {
        std::cout << "\n1. Create Folder\n";
        std::cout << "2. Create File (Auto Contiguous Block Allocation)\n";
        std::cout << "3. Display Current Directory\n";
        std::cout << "4. Change Directory (cd)\n";
        std::cout << "5. Add Background Task (FIFO Queue)\n";
        std::cout << "6. Process Background Task\n";
        std::cout << "7. Report Disk Block Health\n";
        std::cout << "8. Run Smart Defragmenter\n";
        std::cout << "9. Instant File Lookup by ID (Hash O(1))\n";
        std::cout << "10. Fastest Search by Name (Tree BFS)\n";
        std::cout << "11. Delete File (Triggers Deletion Balancer)\n";
        std::cout << "12. View Physical Disk Layout Matrix\n";
        std::cout << "13. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string name;
            std::cout << "Enter folder name: ";
            std::cin >> name;
            vfs.createFolder(name);
        } 
        else if (choice == 2) {
            std::string name, id;
            int size;
            std::cout << "Enter file name: ";
            std::cin >> name;
            std::cout << "Enter unique File ID: ";
            std::cin >> id;
            std::cout << "Enter size (KB): ";
            std::cin >> size;
            vfs.createFile(name, id, size);
        }
        else if (choice == 3) {
            vfs.displayCurrentDirectory();
        }
        else if (choice == 4) {
            std::string name;
            std::cout << "Enter folder name (or '..'): ";
            std::cin >> name;
            vfs.changeDirectory(name);
        }
        else if (choice == 5) {
            std::string task;
            std::cout << "Enter task name: ";
            std::cin >> task;
            vfs.addFormattingTask(task);
        }
        else if (choice == 6) {
            vfs.processNextTask();
        }
        else if (choice == 7) {
            int id, frag;
            std::cout << "Enter Disk Block ID: ";
            std::cin >> id;
            std::cout << "Enter current fragmentation % (0-100): ";
            std::cin >> frag;
            vfs.reportDiskBlockStatus(id, frag);
        }
        else if (choice == 8) {
            vfs.runDefragmentationStep();
        }
        else if (choice == 9) {
            std::string id;
            std::cout << "Enter File ID to look up: ";
            std::cin >> id;
            vfs.lookupFileById(id);
        }
        else if (choice == 10) {
            std::string name;
            std::cout << "Enter File Name to search: ";
            std::cin >> name;
            vfs.fastestSearchByName(name);
        }
        else if (choice == 11) {
            std::string name;
            std::cout << "Enter file name to DELETE: ";
            std::cin >> name;
            vfs.deleteFileByName(name);
        }
        else if (choice == 12) {
            vfs.displayDiskMap();
        }
        else if (choice == 13) {
            std::cout << "Shutting down Virtual File System...\n";
            break;
        } else {
            std::cout << "Invalid choice. Try again.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    return 0;
}