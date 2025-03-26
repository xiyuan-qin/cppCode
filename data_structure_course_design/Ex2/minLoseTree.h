#ifndef MIN_LOSER_TREE_H
#define MIN_LOSER_TREE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <algorithm>

// 系统变量
class Variable {
public:
    Variable() {
        visitDiskTime = 0;
        n = 0; m = 0;
        numberOfDisk = 0;
    }
    std::string path;        // 磁盘的位置
    std::string fileName = "Disk.txt"; // 磁盘文件名称
    std::string file;        // 最终完整路径
    int visitDiskTime;       // 访问磁盘次数
    int n;                   // 数据总数
    int m;                   // 缓冲区大小
    int numberOfDisk;        // 小文件数量
    char ch1, ch;            // 用户选择
};

// 顺串构造器
class SequentialStringPlayer {
public:
    int id, key;
    bool operator<=(SequentialStringPlayer &p) {
        return (id != p.id) ? (id < p.id) : (key < p.key);
    }
};

// 输者树基类
template <class T>
class loserTree {
public:
    virtual ~loserTree() {}
    virtual void initialize(T* thePlayer, int number) = 0;
    virtual int getTheWinner() const = 0;
    virtual void rePlay(int thePlayer, T newvalue) = 0;
};

// 最小输者树
template <class T>
class MinimumLoserTree : public loserTree<T> {
public:
    MinimumLoserTree(T* thePlayer = nullptr, int theNumberOfPlayers = 0) {
        tree = nullptr; 
        advance = nullptr;
        initialize(thePlayer, theNumberOfPlayers);
    }
    
    ~MinimumLoserTree() {
        delete[] tree;
        delete[] advance;
    }
    
    void initialize(T* thePlayer, int theNumberOfPlayers);  // 初始化
    int getTheWinner() const { return tree[0]; }            // 输出当前的赢者
    void rePlay(int thePlayer, T newvalue);                 // 重构
    
private:
    int numberOfPlayers;
    int* tree;    // 记录内部结点，tree[0]是最终的赢者下标
    int* advance; // 记录比赛晋级的成员
    T* player;    // 参与比赛的元素
    int lowExt;   // 最底层外部结点的个数,2*(n-s)
    int offset;   // 2*s-1
    
    void play(int p, int leftChild, int rightChild);
    int winner(int x, int y) { return player[x] <= player[y] ? x : y; }  // 返回更小的元素下标
    int loser(int x, int y) { return player[x] <= player[y] ? y : x; }   // 返回更大的元素下标
};

// 初始化树结构
template <class T>
void MinimumLoserTree<T>::initialize(T* thePlayer, int theNumberOfPlayers) {
    int n = theNumberOfPlayers;
   
    player = thePlayer;
    numberOfPlayers = n;
    delete[] tree; 
    delete[] advance;
    tree = new int[n + 1];
    advance = new int[n + 1];

    // 计算s值 (s=2^log(n-1)-1)，用来计算最后一层的外部结点个数
    int s; 
    for (s = 1; 2 * s <= n - 1; s += s);
    
    lowExt = 2 * (n - s); // 最底层外部结点的个数
    offset = 2 * s - 1; // 最底层外部结点的起始下标

    // 最下面一层开始比赛
    for (int i = 2; i <= lowExt; i += 2)// 把所有的外部节点都比赛一次
        play((i + offset) / 2, i - 1, i);

    int temp = 0;
    if (n % 2 == 1) {  // 奇数个结点
        play(n / 2, advance[n - 1], lowExt + 1);
        temp = lowExt + 3;
    }
    else temp = lowExt + 2;  // 偶数个结点

    // 处理次下层，继续比赛直到胜者被选出
    for (int i = temp; i <= n; i += 2)
        play((i - lowExt + n - 1) / 2, i - 1, i);

    tree[0] = advance[1];  // tree[0]是最终的赢者
}

// 比赛过程
template <class T>
void MinimumLoserTree<T>::play(int p, int leftChild, int rightChild) {
    // tree结点存储相对较大的值，也就是这场比赛的输者
    tree[p] = loser(leftChild, rightChild);
    // advance结点存储相对较小的值，也就是这场比赛的晋级者
    advance[p] = winner(leftChild, rightChild);

    while (p % 2 == 1 && p > 1) {
        tree[p / 2] = loser(advance[p - 1], advance[p]);
        advance[p / 2] = winner(advance[p - 1], advance[p]);
        p /= 2;  // 向上搜索
    }
}

// 重构树，当一个选手改变值时
template <class T>
void MinimumLoserTree<T>::rePlay(int thePlayer, T newvalue) {// 一般是用来更新下一个选手的值
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n) {
        std::cout << "Error! the number must >0 & <=n" << std::endl;
        return;
    }

    player[thePlayer] = newvalue;

    int matchNode,     // 将要比赛的场次
        leftChild,     // 比赛结点的左儿子
        rightChild;    // 比赛结点的右儿子

    if (thePlayer <= lowExt) {  // 如果要更新的节点位于最底层外部节点
        // 根据偏移量计算该节点对应的父节点(比赛节点)的索引
        matchNode = (offset + thePlayer) / 2;
        // 计算该比赛节点的左右子节点
        leftChild = 2 * matchNode - offset;  // 通过反向计算得到左子节点
        rightChild = leftChild + 1;  // 右子节点紧跟左子节点
    }
    else {  // 要更新的节点位于次底层外部节点
        // 计算该节点对应的比赛节点索引，这里考虑了节点在不同层的位置差异
        matchNode = (thePlayer - lowExt + n - 1) / 2;
        if (2 * matchNode == n - 1) {  // 特殊情况：奇数个节点时的最后一个比赛
            // 这种情况下，一个选手是从下层晋级上来的
            leftChild = advance[2 * matchNode];  // 左子节点是晋级者
            rightChild = thePlayer;  // 右子节点是当前更新的节点
        }
        else {
            // 正常情况下的左子节点计算，需要考虑层级偏移
            leftChild = 2 * matchNode - n + 1 + lowExt;  // 调整因为整除导致的索引偏差
            rightChild = leftChild + 1;  // 右子节点紧跟左子节点
        }
    }

    // 重构比赛树，分为两种主要情况
    if (thePlayer == tree[0]) {  // 特殊情况：当前更新的节点是上一轮的总冠军
        // 从匹配节点开始，一路向上重构到根节点
        for (; matchNode >= 1; matchNode /= 2) {
            int oldLoserNode = tree[matchNode];  // 获取该节点记录的上一轮输家
            // 当前节点与旧输家重新比赛
            tree[matchNode] = loser(oldLoserNode, thePlayer);  // 记录新的输家
            advance[matchNode] = winner(oldLoserNode, thePlayer);  // 记录新的晋级者
            thePlayer = advance[matchNode];  // 晋级者继续参与上层比赛
        }
    }
    else {  // 一般情况：更新的节点不是上一轮冠军
        // 首先在当前匹配节点进行比赛
        tree[matchNode] = loser(leftChild, rightChild);  // 记录输家
        advance[matchNode] = winner(leftChild, rightChild);  // 记录晋级者
        
        // 处理奇数个节点时的特殊情况
        if (matchNode == n - 1 && n % 2 == 1) {  // 如果是奇数个节点的最后一场比赛
            matchNode /= 2;  // 移动到父节点
            // 与从最底层晋级的节点进行比赛
            tree[matchNode] = loser(advance[n - 1], lowExt + 1);
            advance[matchNode] = winner(advance[n - 1], lowExt + 1);
        }
        
        // 向上逐层更新，直到根节点
        matchNode /= 2;  // 移动到父节点开始
        for (; matchNode >= 1; matchNode /= 2) {
            // 每个内部节点比较其两个子节点的晋级者
            tree[matchNode] = loser(advance[matchNode * 2], advance[matchNode * 2 + 1]);  // 记录输家
            advance[matchNode] = winner(advance[matchNode * 2], advance[matchNode * 2 + 1]);  // 记录晋级者
        }
    }
    tree[0] = advance[1];  // 更新总冠军，即最终的赢家
}

// 初始化数据到磁盘文件
void init(Variable &va);

// 构造顺串
void SequentialStringConstruction(Variable &va, SequentialStringPlayer* ssplayer);

// 生成最终结果
void GenerateTheFinalResult(Variable &va);

#endif // MIN_LOSER_TREE_H
