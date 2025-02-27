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
    // 构造函数：接受外部节点的初始值，构建输者树
    LoserTree(const std::vector<T>& values) : extValues(values) {
        k = values.size();
        if (k < 2) {
            throw std::invalid_argument("At least two external nodes are required.");
        }
        loserTree.resize(k - 1, -1);// 把容器重新规划大小
        build();
    }

    // 构建输者树
    void build() {
        for (int i = 0; i < k; ++i) {
            adjust(i);
        }
    }

    // 调整外部节点i的值变化后的输者树
    void adjust(int i) {
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

    // 获取当前胜者的索引
    int getWinnerIndex() const {
        return winner;
    }

    // 获取当前胜者的值
    T getWinnerValue() const {
        return extValues[winner];
    }
};
