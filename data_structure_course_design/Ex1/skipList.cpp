#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono>


using namespace std;

class SkipNode {
public:
    int key;
    vector<SkipNode*> next;// 创建一个链表的指针数组，表示n层指针

    SkipNode(int key, int level) {
        this->key = key;
        this->next = vector<SkipNode*>(level, nullptr);//创建新节点时只实例化，不连接
    }
};

class SkipList {
private:
    SkipNode* header;// 头节点
    int maxLevel;
    int currentLevel;
    float probability;

    int randomLevel() {
        int lvl = 1;
        while ((rand() / (float)RAND_MAX) < probability && lvl < maxLevel)
            lvl++;
        return lvl;
    }// 随机生成一个层数

public:
    SkipList(int maxLvl, float p = 0.5) : maxLevel(maxLvl), currentLevel(1), probability(p) {
        header = new SkipNode(INT_MIN, maxLevel);// 创建头节点
        srand(time(0));
    }

    ~SkipList() {
        SkipNode* node = header->next[0];
        while (node) {
            SkipNode* next = node->next[0];
            delete node;
            node = next;
        }
        delete header;
    }

    bool search(int key) {
        SkipNode* current = header;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < key)
                current = current->next[i];
        }
        current = current->next[0];
        return current != nullptr && current->key == key;
    }

    void insert(int key) {
        vector<SkipNode*> update(maxLevel, nullptr);
        SkipNode* current = header;

        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < key)//找到每一层比插入元素大的就停下来
                current = current->next[i];
            update[i] = current;// 记录每一层最后一个小于插入元素的节点
        }

        current = current->next[0];
        if (current && current->key == key) return;//不用插入重复元素

        int newLevel = randomLevel();//插入的层数
        if (newLevel > currentLevel) {
            for (int i = currentLevel; i < newLevel; i++)
                update[i] = header;
            currentLevel = newLevel;
        }//超出的话就把多出来的层的update都指向头节点，相当于头节点就是最后一个小于插入元素的节点

        SkipNode* newNode = new SkipNode(key, newLevel);
        for (int i = 0; i < newLevel; i++) {
            newNode->next[i] = update[i]->next[i];
            update[i]->next[i] = newNode;
        }//插
    }

    bool remove(int key) {
        vector<SkipNode*> update(maxLevel, nullptr);
        SkipNode* current = header;

        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < key)
                current = current->next[i];
            update[i] = current;
        }

        current = current->next[0];
        if (!current || current->key != key) return false;

        for (int i = 0; i < currentLevel; i++) {
            if (update[i]->next[i] != current) break;
            update[i]->next[i] = current->next[i];
        }

        delete current;
        while (currentLevel > 1 && header->next[currentLevel - 1] == nullptr)
            currentLevel--;

        return true;
    }

    int deleteMin() {
        SkipNode* minNode = header->next[0];
        if (!minNode) return INT_MIN;

        int minKey = minNode->key;
        for (int i = 0; i < minNode->next.size(); i++)
            header->next[i] = minNode->next[i];

        while (currentLevel > 1 && header->next[currentLevel - 1] == nullptr)
            currentLevel--;

        delete minNode;
        return minKey;
    }

    int deleteMax() {
        SkipNode* current = header;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i])
                current = current->next[i];
        }

        if (current == header) return INT_MIN;
        int maxKey = current->key;
        remove(maxKey);
        return maxKey;
    }

    void rebuildSkipList(SkipList& sk, vector<int>& keys) {
        // 收集所有键值
        vector<int> allKeys;
        SkipNode* node = sk.header->next[0];
        while (node) {
            allKeys.push_back(node->key);
            node = node->next[0];
        }
    
        // 重建跳表
        sk.~SkipList();
        new (&sk) SkipList(20); // 重新初始化
        for (int key : allKeys)
            sk.insert(key);
    }
};



void experiment() {
    const int MAX_LEVEL = 20;
    const int INIT_SIZE = 1000;
    const int OPS = 10000;

    // 初始化跳表
    SkipList sk(MAX_LEVEL);
    for (int i = 0; i < INIT_SIZE; i++)
        sk.insert(rand() % 100000);

    // 操作序列
    double totalInsert = 0, totalSearch = 0, totalDelete = 0;
    for (int i = 0; i < OPS; i++) {
        int op = rand() % 3;
        int key = rand() % 100000;

        auto start = chrono::high_resolution_clock::now();
        switch(op) {
            case 0: sk.insert(key); break;
            case 1: sk.search(key); break;
            case 2: sk.remove(key); break;
        }
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        if (op == 0) totalInsert += time;
        else if (op == 1) totalSearch += time;
        else totalDelete += time;
    }

    cout << "Average Time (ns):\nInsert: " << totalInsert/OPS
         << "\nSearch: " << totalSearch/OPS
         << "\nDelete: " << totalDelete/OPS << endl;
}

int main() {
    experiment();
    return 0;
}