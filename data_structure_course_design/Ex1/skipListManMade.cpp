#include<iostream>
#include<vector>

using namespace std;


class skipNode {
public:
    int key;
    vector<skipNode*> next;
    skipNode(int k, int level) : key(k), next(level, nullptr) {}
};

