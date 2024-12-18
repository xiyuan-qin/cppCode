#include <iostream>
using namespace std;

template <class T>
class weightedEdge{//加权边类
public:
    int start;
    int end;
    long long weight;  

    weightedEdge() : start(0), end(0), weight(0) {} 
};

template <class T>
class minHeap {
private:
    T* heap;
    int heap_size;

    void siftDown(int root) {//抽取出来上下浮动的操作
        T temp = heap[root];
        int child = 2 * root;
        while (child <= heap_size) {
            if (child < heap_size && heap[child].weight > heap[child + 1].weight)
                child++;
            if (temp.weight <= heap[child].weight)
                break;
            heap[child/2] = heap[child];
            child *= 2;
        }
        heap[child/2] = temp;
    }

public:
    minHeap(T* edges, int size) {
        heap_size = size;
        heap = new T[size + 1];
        // 复制数据
        for (int i = 1; i <= size; i++) {
            heap[i] = edges[i];
        }
        // 建堆
        for (int i = heap_size/2; i >= 1; i--) {
            siftDown(i);
        }
    }

    ~minHeap() { delete[] heap; }

    bool empty() const { return heap_size == 0; }
    T top() const { return heap[1]; }

    void pop() {
        if (heap_size == 0) return;
        heap[1] = heap[heap_size--];
        if (heap_size > 0) siftDown(1);
    }
};

class DisjointSet {//并查集
private:
    int* parent;

public:
    DisjointSet(int n) {
        parent = new int[n + 1]();  // 初始化
    }

    ~DisjointSet() {
        delete[] parent;
    }

    int find(int x) {
        int root = x;
        while (parent[root] != 0) {
            root = parent[root];
        }
        // 路径压缩
        while (x != root) {
            int temp = parent[x];
            parent[x] = root;
            x = temp;
        }
        return root;
    }

    void unite(int x, int y) {
        parent[y] = x;
    }
};

int main()
{
    int n, e;
    cin >> n >> e;
    weightedEdge<int>* edges = new weightedEdge<int>[e + 1];
    
    for (int i = 1; i <= e; i++) {
        cin >> edges[i].start >> edges[i].end >> edges[i].weight;
    }

    minHeap<weightedEdge<int>> heap(edges, e);
    DisjointSet ds(n);
    
    long long total_weight = 0;
    int edges_used = 0;

    while (!heap.empty() && edges_used < n - 1) {
        weightedEdge<int> edge = heap.top();heap.pop();
        
        int a = ds.find(edge.start);
        int b = ds.find(edge.end);
        if (a != b) {
            ds.unite(a, b);
            total_weight += edge.weight;
            edges_used++;
        }
    }

    cout << total_weight << endl;
    delete[] edges;
    return 0;
}