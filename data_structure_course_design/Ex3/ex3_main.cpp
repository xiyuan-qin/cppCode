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
        if (command == "q" || command == "quit") {
            break;
        } 
        
        if (command == "ls" || command == "dir") {
            tree.ls();
        } else if (command == "pwd") {
            tree.pwd();
        } else if (command == "cd") {
            string name;
            // 检查是否有额外输入
            if (cin.peek() == '\n') {
                // 无参数的cd命令，打印当前路径
                tree.pwd();
            } else {
                cin >> name;
                tree.handleCdCommand(name);
            }
        } else if (command == "mkdir") {
            string name;
            cin >> name;
            tree.mkdir(name);
        } else if (command == "mkfile") {
            string name;
            cin >> name;
            tree.mkfile(name);
        } else if (command == "rm" || command == "delete") {
            string name;
            cin >> name;
            tree.rm(name);
        } else if (command == "save") {
            string filename;
            cin >> filename;
            tree.save(filename);
        } else if (command == "load") {
            string filename;
            cin >> filename;
            tree.load(filename);
        } else if (command == "tree") {
            tree.tree();
        } else {
            cout << "未知命令" << endl;
        }
    }
    return 0;
}