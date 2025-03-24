#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<algorithm>

using namespace std;

/**
 * 每次存入节点的string格式是 “name/”
 * 因为根节点已经是“/”了
 */


class catalogTreeNode {
public:
    string name;
    vector<catalogTreeNode*> children;
    catalogTreeNode* parent;
    bool isFile = false;

    catalogTreeNode(string name) {
        this->name = name;
        parent = nullptr;
    }
};


class catalogTree {
public:

    catalogTreeNode* root;
    catalogTreeNode* current;

    catalogTree() {
        root = new catalogTreeNode("/");
        current = root;  // 初始化current为root
    }

    void ls() {
        for(auto i : current->children) {
            cout << i->name << " ";
        }
        cout << endl;
    }

    void pwd() {
        stack<catalogTreeNode*> s;
        s.push(current);
        catalogTreeNode* temp = current;

        while(temp->parent != nullptr) {
            s.push(temp->parent);
            temp = temp->parent;
        }

        while(!s.empty()) {
            cout << s.top()->name;
            s.pop();
        }
        cout << endl;
    }

    void cd_() {
        if(current->parent == nullptr) {
            cout << "当前目录已经是根目录" << endl;
            return;
        }
        current = current->parent;
    }

    void cd(string name) {
        for(auto i : current->children) {
            if(i->name == name + "/") {
                current = i;
                return;
            }
        }
        //TODO: cd进绝对路径
        cout << "未找到该目录" << endl;
    }

    void mkdir(string name) {
        catalogTreeNode* temp = new catalogTreeNode(name + "/");
        temp->parent = current;
        current->children.push_back(temp);
    }

    void mkfile(string file_name) {
        catalogTreeNode* temp = new catalogTreeNode(file_name);
        temp->parent = current;
        temp->isFile = true;
        current->children.push_back(temp);
    }

    void rm(string name) {
        for(auto i = current->children.begin(); i != current->children.end(); i++) {
            // 检查目录和文件两种情况
            if(((*i)->isFile && (*i)->name == name) || 
               (!(*i)->isFile && (*i)->name == name + "/")) {
                current->children.erase(i);
                return;
            }
        }
        cout << "未找到该目录或文件" << endl;
    }
};

