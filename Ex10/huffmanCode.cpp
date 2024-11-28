#include <iostream>
#include <string>
using namespace std;

// 自定义队列类
template<class T>
class Queue {
private:
    int queuefront;  // 第一个数前一个位置的索引
    int queueback;   // 最后一个数的索引
    int arraylength; // 队列长度
    T* queue0;       // 队列数组

public:
    Queue(int l) {
        arraylength = l;
        queue0 = new T[arraylength];
        queuefront = queueback = 0;
    }
    ~Queue() { delete[] queue0; }
    bool empty() const { return queuefront == queueback; } // 队列是否为空
    int qsize() const { return queueback - queuefront; }   // 队列大小
    T& front() { return queue0[queuefront]; }             // 返回队列头部
    void pop() { queuefront++; }                          // 删除队列头部元素
    void push(const T& theelement) {                      // 插入队列尾部元素
        queue0[queueback++] = theelement;
    }
};

// Huffman树节点
class HuffmanNode {
public:
    int weight;          // 权值
    int height;          // 节点到根的高度
    HuffmanNode* leftchild;
    HuffmanNode* rightchild;

    HuffmanNode() : weight(0), height(0), leftchild(nullptr), rightchild(nullptr) {}
};

// 最小堆
// 特殊化模板类minHeap，霍夫曼节点的最小堆
class minHeap {
    
private:
    HuffmanNode** heap; // 节点指针数组
    int heapSize;       // 堆中的元素个数

public:

    minHeap(HuffmanNode** theHeap, int theSize) {
        heap = new HuffmanNode*[theSize + 1];
        for (int i = 1; i <= theSize; i++) {
            heap[i] = theHeap[i - 1];
        }
        heapSize = theSize;
        for (int root = heapSize / 2; root >= 1; root--) {
            HuffmanNode* rootElement = heap[root];
            int child = 2 * root;
            while (child <= heapSize) {
                if (child < heapSize && heap[child]->weight > heap[child + 1]->weight) {
                    child++;
                }
                if (rootElement->weight <= heap[child]->weight) break;
                heap[child / 2] = heap[child];
                child *= 2;
            }
            heap[child / 2] = rootElement;
        }
    }

    ~minHeap() { delete[] heap; }

    HuffmanNode* top() { return heap[1]; } // 返回根节点

    void pop() { // 删除堆顶元素
        HuffmanNode* lastElement = heap[heapSize--];
        int currentNode = 1;
        int child = 2;
        while (child <= heapSize) {
            if (child < heapSize && heap[child]->weight > heap[child + 1]->weight) {
                child++;
            }
            if (lastElement->weight <= heap[child]->weight) break;
            heap[currentNode] = heap[child];
            currentNode = child;
            child *= 2;
        }
        heap[currentNode] = lastElement;
    }

    void push(HuffmanNode* x) { // 插入元素
        int currentNode = ++heapSize; // 从新的叶节点开始
        while (currentNode != 1 && x->weight < heap[currentNode / 2]->weight) {
            heap[currentNode] = heap[currentNode / 2]; // 父节点下移
            currentNode /= 2;
        }
        heap[currentNode] = x;
    }

};

// Huffman树
class huffmanTree {

private:
    int num;          // 树的节点数
    HuffmanNode* root; // 根节点

public:

    huffmanTree(int* a, int n) { // 构造Huffman树
        //创建一个霍夫曼树的指针数组用于构造最小堆
        HuffmanNode** heapArray = new HuffmanNode*[n];
        for (int i = 0; i < n; i++) {
            heapArray[i] = new HuffmanNode();
            heapArray[i]->weight = a[i];//每次都把权值赋值给weight
        }//此时数组里是每个给定的元素和它的权值，给定元素用下标表示

        minHeap heap(heapArray, n);
        HuffmanNode *z, *l, *r;
        for (int i = 1; i < n; i++) {
            l = heap.top();heap.pop();//取出最小的两个
            r = heap.top();heap.pop();

            z = new HuffmanNode;
            z->leftchild = l;
            z->rightchild = r;
            z->weight = l->weight + r->weight;
            heap.push(z);//最后栈顶是完好的树
        }
        num = n;
        root = heap.top();
        heap.pop();
        delete[] heapArray;
    }

    void length() { // 计算编码长度
        int height = 0;
        Queue<HuffmanNode*> q(num * 2 - 1);
        HuffmanNode* temp;
        //中序遍历
        q.push(root);
        root->height = 0;
        while (!q.empty()) {
            temp = q.front();q.pop();

            //每访问一次height加一
            if (temp->leftchild != nullptr) {
                q.push(temp->leftchild);
                temp->leftchild->height = temp->height + 1;
            }

            if (temp->rightchild != nullptr) {
                q.push(temp->rightchild);
                temp->rightchild->height = temp->height + 1;
            }
            //访问到叶子节点时，计算高度
            if (temp->leftchild == nullptr && temp->rightchild == nullptr) {
                height += temp->height * temp->weight;
            }

        }
        cout << height << endl;
    }
};

int main() {
    string str;
    cin >> str;

    //统计每个字符出现的次数
    int* num = new int[26]();
    int num_of_unique_letter = 0; 
    for (char c : str) {
        num[c - 'a']++;
        if (num[c - 'a'] == 1) num_of_unique_letter++;
    }
    //b是用来存储非0元素的数组
    int* b = new int[num_of_unique_letter];
    int d = 0;
    for (int i = 0; i < 26; i++) {
        if (num[i] != 0) b[d++] = num[i];
    }


    huffmanTree huffman_tree(b, num_of_unique_letter);
    huffman_tree.length();
    delete[] num;
    delete[] b;
    return 0;
}
