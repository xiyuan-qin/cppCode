#include "minLoseTree.h"
#include <iostream>

template <class T>
LoserTree<T>::LoserTree(const std::vector<T>& values) : extValues(values) {
    k = values.size();
    if (k < 2) {
        throw std::invalid_argument("At least two external nodes are required.");
    }
    loserTree.resize(k - 1, -1);
    build();
}

template <class T>
void LoserTree<T>::build() {
    for (int i = 0; i < k; ++i) {
        adjust(i);
    }
}

template <class T>
void LoserTree<T>::adjust(int i) {
    int current = i;
    int s = k - 1 + current; // 转换为完全二叉树的叶子索引
    int parent = (s - 1) / 2;

    while (parent >= 0) {
        int opponent = loserTree[parent];
        if (opponent == -1) {
            // 父节点未初始化，直接设置败者
            loserTree[parent] = current;
            break;
        }
        if (extValues[current] < extValues[opponent]) {
            // current是胜者，更新父节点败者为opponent
            std::swap(current, opponent);
        }
        // 更新父节点败者为current的对手
        loserTree[parent] = opponent;
        // 向上处理父节点
        s = parent;
        parent = (s - 1) / 2;
    }
    // 更新全局胜者
    winner = current;
}

template <class T>
int LoserTree<T>::getWinnerIndex() const {
    return winner;
}

template <class T>
T LoserTree<T>::getWinnerValue() const {
    return extValues[winner];
}