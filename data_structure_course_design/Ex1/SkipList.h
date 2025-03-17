#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

class SkipNode {
public:
    int key;
    std::vector<SkipNode*> next;
    
    // 添加静态内存池以减少内存分配开销
    static std::vector<SkipNode*> pool;
    
    static void* operator new(size_t size);
    static void operator delete(void* ptr);
    
    SkipNode(int key, int level);
};

class SkipList {
private:
    SkipNode* header;
    int maxLevel;
    int currentLevel;
    float probability;

    int randomLevel();

public:
    SkipList(int maxLvl = 32, float p = 0.25);
    ~SkipList();

    bool search(int key);
    void insert(int key);
    bool erase(int key);
    int deleteMin();
    int deleteMax();
    void rebuildSkipList(SkipList& sk);
    // std::vector<int> rangeQuery(int minKey, int maxKey);
    int getCurrentLevel() const;
    int getMaxLevel() const;
};

#endif // SKIP_LIST_H
