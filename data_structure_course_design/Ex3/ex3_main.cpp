#include "catalogTree.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    catalogTree tree;
    string command;
    while (1) {
        cout << "请输入命令：";
        cin >> command;
        if (command == "q") {
            break;
        } 
        
        if (command == "ls") {
            tree.ls();
        } else if (command == "pwd") {
            tree.pwd();
        } else if (command == "cd") {
            string name;
            cin >> name;
            tree.handleCdCommand(name);
        } else if (command == "mkdir") {
            string name;
            cin >> name;
            tree.mkdir(name);
        } else if (command == "mkfile") {
            string name;
            cin >> name;
            tree.mkfile(name);
        } else if (command == "rm") {
            string name;
            cin >> name;
            tree.rm(name);
        } else {
            cout << "未知命令" << endl;
        }
    }
    return 0;
}