#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<algorithm>
#include<sstream>
#include<fstream> // 添加文件操作头文件

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
    // 定义数据文件目录常量
    const string DATA_DIR = "./dat/";
    
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
        
        // 先回到根目录
        current = root;
        
        // 如果路径就是根目录，直接返回成功
        if (path == "/") {
            return true;
        }
        
        vector<string> pathComponents = parsePath(path);
        if (pathComponents.empty()) {
            return false;
        }
        
        // 跳过第一个元素(根目录标记)，逐级查找目录
        for (size_t i = 1; i < pathComponents.size(); i++) {
            bool found = false;
            for (auto child : current->children) {
                // 检查目录名是否匹配（不带/）
                string dirName = child->isFile ? child->name : child->name.substr(0, child->name.length() - 1);
                if (dirName == pathComponents[i] && !child->isFile) {
                    current = child;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
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
            cout << "当前目录已经是根目录" << endl;
            return false;
        }
        current = current->parent;
        return true;
    }

    bool cd(string name) {
        for(auto i : current->children) {
            // 对于目录，比较不带斜杠的名称
            if (!i->isFile) {
                string dirName = i->name.substr(0, i->name.length() - 1);
                if (dirName == name) {
                    current = i;
                    return true;
                }
            }
        }
        cout << "未找到该目录" << endl;
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

    // 递归保存目录树结构的辅助函数
    void saveTreeHelper(catalogTreeNode* node, ofstream& file, string path) {
        // 保存当前节点
        file << path << " " << (node->isFile ? "1" : "0") << endl;
        
        // 递归保存子节点
        for(auto child : node->children) {
            string childPath = path;
            if(path != "/") // 避免路径中出现双斜杠
                childPath += child->name;
            else
                childPath = path + child->name;
            saveTreeHelper(child, file, childPath);
        }
    }

    // 保存目录树到文件
    void save(string filename) {
        // 构建完整的文件路径，保存到dat文件夹
        string fullPath = DATA_DIR + filename;
        ofstream file(fullPath);
        if(!file) {
            cout << "无法打开文件进行保存，请确保 " << DATA_DIR << " 目录存在" << endl;
            return;
        }
        
        saveTreeHelper(root, file, "/");
        file.close();
        cout << "目录树已保存到 " << fullPath << endl;
    }

    // 递归删除节点及其所有子节点
    void deleteNode(catalogTreeNode* node) {
        if(!node) return;
        
        // 先删除所有子节点
        for(auto& child : node->children) {
            deleteNode(child);
        }
        
        // 删除节点本身
        delete node;
    }

    // 从字符串路径创建目录结构
    void createPathFromString(string path, bool isFile) {
        // 如果路径是根节点，已经存在，不需要创建
        if(path == "/") return;
        
        vector<string> components = parsePath(path);
        if(components.empty()) return;
        
        catalogTreeNode* currentNode = root;
        
        // 从根开始遍历路径
        for(size_t i = 1; i < components.size(); i++) {
            string component = components[i];
            bool isLastComponent = (i == components.size() - 1);
            
            // 查找子节点中是否已存在此组件
            bool found = false;
            for(auto child : currentNode->children) {
                string childName;
                if(child->isFile)
                    childName = child->name;
                else
                    childName = child->name.substr(0, child->name.length() - 1);
                
                if(childName == component) {
                    currentNode = child;
                    found = true;
                    break;
                }
            }
            
            // 如果不存在，创建新节点
            if(!found) {
                catalogTreeNode* newNode;
                
                if(isLastComponent && isFile) {
                    newNode = new catalogTreeNode(component);
                    newNode->isFile = true;
                } else {
                    newNode = new catalogTreeNode(component + "/");
                }
                
                newNode->parent = currentNode;
                currentNode->children.push_back(newNode);
                currentNode = newNode;
            }
        }
    }

    // 从文件加载目录树
    void load(string filename) {
        // 构建完整的文件路径，从dat文件夹加载
        string fullPath = DATA_DIR + filename;
        ifstream file(fullPath);
        if(!file) {
            cout << "无法打开文件 " << fullPath << endl;
            return;
        }
        
        // 清除现有树结构，但保留根节点
        for(auto& child : root->children) {
            deleteNode(child);
        }
        root->children.clear();
        current = root;
        
        string line;
        while(getline(file, line)) {
            stringstream ss(line);
            string path;
            int isFile;
            ss >> path >> isFile;
            
            // 创建路径
            createPathFromString(path, isFile == 1);
        }
        
        file.close();
        cout << "目录树已从 " << fullPath << " 加载" << endl;
    }

    // 打印整个目录树结构
    void tree() {
        cout << "目录结构:" << endl;
        cout << root->name << endl;
        printTreeNode(root, "");
    }

    // 递归打印树节点的辅助函数
    void printTreeNode(catalogTreeNode* node, string indent) {
        // 按字母顺序对子节点进行排序（先目录后文件）
        vector<catalogTreeNode*> dirs;
        vector<catalogTreeNode*> files;
        
        for(auto child : node->children) {
            if(child->isFile)
                files.push_back(child);
            else
                dirs.push_back(child);
        }
        
        // 先打印目录
        for(size_t i = 0; i < dirs.size(); i++) {
            bool isLast = (i == dirs.size() - 1 && files.empty());
            cout << indent << (isLast ? "└── " : "├── ") << dirs[i]->name << endl;
            printTreeNode(dirs[i], indent + (isLast ? "    " : "│   "));
        }
        
        // 再打印文件
        for(size_t i = 0; i < files.size(); i++) {
            bool isLast = (i == files.size() - 1);
            cout << indent << (isLast ? "└── " : "├── ") << files[i]->name << endl;
        }
    }
};

