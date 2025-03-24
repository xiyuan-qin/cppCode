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

    // 计算s值 (s=2^log(n-1)-1)
    int s; 
    for (s = 1; 2 * s <= n - 1; s += s);
    
    lowExt = 2 * (n - s); 
    offset = 2 * s - 1;

    // 最下面一层开始比赛
    for (int i = 2; i <= lowExt; i += 2)
        play((i + offset) / 2, i - 1, i);

    int temp = 0;
    if (n % 2 == 1) {  // 奇数个结点
        play(n / 2, advance[n - 1], lowExt + 1);
        temp = lowExt + 3;
    }
    else temp = lowExt + 2;  // 偶数个结点

    // 处理次下层
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
void MinimumLoserTree<T>::rePlay(int thePlayer, T newvalue) {
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n) {
        std::cout << "Error! the number must >0 & <=n" << std::endl;
        return;
    }

    player[thePlayer] = newvalue;

    int matchNode,     // 将要比赛的场次
        leftChild,     // 比赛结点的左儿子
        rightChild;    // 比赛结点的右儿子

    if (thePlayer <= lowExt) {  // 如果要比赛的结点在最下层
        matchNode = (offset + thePlayer) / 2;
        leftChild = 2 * matchNode - offset;
        rightChild = leftChild + 1;
    }
    else {  // 要比赛的结点在次下层
        matchNode = (thePlayer - lowExt + n - 1) / 2;
        if (2 * matchNode == n - 1) {  // 特殊情况，其中一方是晋级后的人
            leftChild = advance[2 * matchNode];
            rightChild = thePlayer;
        }
        else {
            leftChild = 2 * matchNode - n + 1 + lowExt;  // 这个操作是因为上面matchNode计算中/2取整了
            rightChild = leftChild + 1;
        }
    }

    // 比赛重构，分两种情况
    if (thePlayer == tree[0]) {  // 当要重构的点是上一场比赛的赢家
        for (; matchNode >= 1; matchNode /= 2) {
            int oldLoserNode = tree[matchNode];  // 上一场比赛的输者
            tree[matchNode] = loser(oldLoserNode, thePlayer);
            advance[matchNode] = winner(oldLoserNode, thePlayer);
            thePlayer = advance[matchNode];
        }
    }
    else {  // 其他情况重构
        tree[matchNode] = loser(leftChild, rightChild);
        advance[matchNode] = winner(leftChild, rightChild);
        if (matchNode == n - 1 && n % 2 == 1) {  // 特殊情况
            matchNode /= 2;
            tree[matchNode] = loser(advance[n - 1], lowExt + 1);
            advance[matchNode] = winner(advance[n - 1], lowExt + 1);
        }
        matchNode /= 2;
        for (; matchNode >= 1; matchNode /= 2) {
            tree[matchNode] = loser(advance[matchNode * 2], advance[matchNode * 2 + 1]);
            advance[matchNode] = winner(advance[matchNode * 2], advance[matchNode * 2 + 1]);
        }
    }
    tree[0] = advance[1];  // 最终胜者
}

// 初始化数据到磁盘文件
void init(Variable &va);

// 构造顺串
void SequentialStringConstruction(Variable &va, SequentialStringPlayer* ssplayer);

// 生成最终结果
void GenerateTheFinalResult(Variable &va);

#endif // MIN_LOSER_TREE_H
