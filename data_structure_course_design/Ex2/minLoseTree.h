#ifndef LOSER_TREE_H
#define LOSER_TREE_H

#include <vector>
#include <stdexcept>
#include <algorithm>

template <class T>
class LoserTree {
private:
    int k; // 外部节点数目
    std::vector<int> loserTree; // 内部节点，数目为k-1
    std::vector<T> extValues;   // 外部节点的值
    int winner;                 // 当前胜者的索引

public:
    //接受外部节点的初始值，构建输者树
    LoserTree(const std::vector<T>& values);

    // 构建输者树
    void build();

    // 调整外部节点i的值变化后的输者树
    void adjust(int i);

    // 获取当前胜者的索引
    int getWinnerIndex() const;

    // 获取当前胜者的值
    T getWinnerValue() const;
};

#include "minLoseTree.cpp"  // 包含模板实现

#endif // LOSER_TREE_H
