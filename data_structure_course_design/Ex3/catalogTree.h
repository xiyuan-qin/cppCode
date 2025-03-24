#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<algorithm>
#include<sstream>

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
        
        // 创建默认目录结构
        catalogTreeNode* home = new catalogTreeNode("home/");
        home->parent = root;
        root->children.push_back(home);
        
        catalogTreeNode* bin = new catalogTreeNode("bin/");
        bin->parent = root;
        root->children.push_back(bin);
        
        catalogTreeNode* etc = new catalogTreeNode("etc/");
        etc->parent = root;
        root->children.push_back(etc);
        
        catalogTreeNode* usr = new catalogTreeNode("usr/");
        usr->parent = root;
        root->children.push_back(usr);
        
        catalogTreeNode* tmp = new catalogTreeNode("tmp/");
        tmp->parent = root;
        root->children.push_back(tmp);
        
        // 在home目录下创建一个默认用户目录
        catalogTreeNode* user = new catalogTreeNode("user/");
        user->parent = home;
        home->children.push_back(user);
        
        // 在user目录下创建一些默认文件
        catalogTreeNode* readme = new catalogTreeNode("readme.txt");
        readme->parent = user;
        readme->isFile = true;
        user->children.push_back(readme);
    }

    // 解析路径字符串为路径分量
    vector<string> parsePath(const string& path) {
        vector<string> components;
        stringstream ss(path);
        string component;
        
        // 跳过开头的斜杠
        if (!path.empty() && path[0] == '/') {
            // 绝对路径标记
            components.push_back("/");
        }
        
        // 使用/分割路径
        while (getline(ss, component, '/')) {
            if (!component.empty()) {
                components.push_back(component);
            }
        }
        
        return components;
    }

    // 处理绝对路径的导航
    bool cdAbsolute(const string& path) {
        if (path[0] != '/') {
            return false; // 不是绝对路径
        }
        
        vector<string> pathComponents = parsePath(path);
        if (pathComponents.empty()) {
            return false;
        }
        
        // 回到根目录
        while(cd_()) {
            // 持续返回上级直到根目录
        }
        
        // 跳过第一个元素(根目录标记)
        for (size_t i = 1; i < pathComponents.size(); i++) {
            if (!cd(pathComponents[i])) {
                cout << "路径错误：" << pathComponents[i] << " 不存在" << endl;
                return false;
            }
        }
        return true;
    }

    // 处理cd命令，支持相对路径和绝对路径
    void handleCdCommand(const string& path) {
        if (path == "..") {
            cd_();
        } else if (path[0] == '/') {
            cdAbsolute(path);
        } else {
            cd(path);
        }
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

    bool cd_() {
        if(current->parent == nullptr) {
            // cout << "当前目录已经是根目录" << endl;
            return false;
        }
        current = current->parent;
        return true;
    }

    bool cd(string name) {
        for(auto i : current->children) {
            if(i->name == name + "/") {
                current = i;
                return true;
            }
        }
        //TODO: cd进绝对路径
        // cout << "未找到该目录" << endl;
        return false;
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

