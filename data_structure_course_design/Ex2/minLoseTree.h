#ifndef MIN_LOSER_TREE_H
#define MIN_LOSER_TREE_H

#include <vector>
#include <iostream> // 添加用于调试输出

template <class T>
class LoserTree {
private:
    std::vector<T> extValues;  // 外部结点值
    std::vector<int> loserTree; // 败者树
    int k;                     // 外部结点数
    int winner;                // 冠军（赢者）

public:
    LoserTree(const std::vector<T>& values);
    void build();
    void adjust(int i);
    int WinnerIndex() const;
    T WinnerValue() const;
    void printTree() const; // 添加打印树的方法用于调试
};

// 模板类实现
template <class T>
LoserTree<T>::LoserTree(const std::vector<T>& values) : extValues(values) {
    k = values.size();
    if (k == 0) {
        winner = -1;
        return; // 防止空数组
    }
    
    loserTree.resize(k - 1, -1);
    winner = 0; // 初始化胜者
    build();
}

template <class T>
void LoserTree<T>::build() {
    // 初始化loserTree数组
    for (int i = 0; i < k - 1; ++i) {
        loserTree[i] = -1;
    }
    
    // 从最后一个外部节点开始调整
    for (int i = k - 1; i >= 0; --i) {
        adjust(i);
    }
}

template <class T>
void LoserTree<T>::adjust(int i) {
    if (k <= 0) return;
    
    int parent = (i + k - 1) / 2;  // 计算父节点位置
    
    while (parent > 0) {
        // 如果尚未有败者，或者当前节点值大于已存储的败者，则当前节点成为新败者
        if (loserTree[parent] == -1 || extValues[i] > extValues[loserTree[parent]]) {
            std::swap(i, loserTree[parent]);
        }
        parent /= 2;  // 向上移动到树的下一层
    }
    
    // 处理根节点（即全局胜者的对手）
    if (loserTree[0] == -1 || extValues[i] > extValues[loserTree[0]]) {
        std::swap(i, loserTree[0]);
    }
    
    winner = loserTree[0];  // 根节点存储的是全局胜者的对手（即最小值）
}

template <class T>
int LoserTree<T>::WinnerIndex() const {
    return winner;
}

template <class T>
T LoserTree<T>::WinnerValue() const {
    if (k > 0 && winner >= 0 && winner < k)
        return extValues[winner];
    return T(); // 返回默认值
}

template <class T>
void LoserTree<T>::printTree() const {
    std::cout << "External values: ";
    for (size_t i = 0; i < extValues.size(); ++i) {
        std::cout << extValues[i] << " ";
    }
    std::cout << "\nLoser tree: ";
    for (size_t i = 0; i < loserTree.size(); ++i) {
        std::cout << loserTree[i] << " ";
    }
    std::cout << "\nWinner index: " << winner << std::endl;
}

#endif // MIN_LOSER_TREE_H
