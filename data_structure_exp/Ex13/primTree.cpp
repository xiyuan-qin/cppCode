#include<iostream>
using namespace std;

class vertexNode {// 顶点节点类
public:
    int weight; // 从起始点到该点的 边 的权重
    int element;// 顶点编号
    vertexNode* next;// 指向下一个节点的指针

    vertexNode() {
        weight = 0;
        element = 0;
        next = NULL;
    }

    vertexNode(const int& element, const int& weight) {
        this->element = element;
        this->weight = weight;
        this->next = NULL;
    }

    vertexNode(const int& element, const int& weight, vertexNode* next) {
        this->element = element;
        this->weight = weight;
        this->next = next;
    }
};

int reach[2000000] = { 0 };//题目给的最大数量限制

class minHeap {
private:
    vertexNode* heap;
    int heap_size;    
    int array_length; 
public:
    minHeap(int n) {
        heap_size = 0;
        heap = new vertexNode[n + 1];
        array_length = n + 1;
    }

    ~minHeap() { delete[] heap; }

    bool empty() { return heap_size == 0; }

    int size() { return heap_size; } 

    vertexNode& top() { return heap[1]; }

    void push(const vertexNode &x) {
        if (heap_size == array_length - 1) {
            array_length *= 2;
            vertexNode* new_heap = new vertexNode[array_length]; 
            for (int i = 1; i <= heap_size; i++) {
                new_heap[i] = heap[i];
            }
            delete[] heap;
            heap = new_heap;
        }
        int current_pos = ++heap_size;  
        while (current_pos != 1 && x.weight < heap[current_pos / 2].weight) {
            heap[current_pos] = heap[current_pos / 2];
            current_pos /= 2;
        }
        heap[current_pos] = x;
    }

    void pop() {
        vertexNode last_element = heap[heap_size--];  
        int current_pos = 1;  
        int child_pos = 2;    
        while (child_pos <= heap_size) {
            if (child_pos < heap_size && heap[child_pos].weight > heap[child_pos + 1].weight) 
                child_pos++;
            if (last_element.weight <= heap[child_pos].weight) 
                break;
            heap[current_pos] = heap[child_pos];
            current_pos = child_pos;
            child_pos *= 2;
        }
        heap[current_pos] = last_element;
    }
};

class graph {
private:
    int num_vertices;  // 顶点数
    int num_edges;     // 边数
    vertexNode* list;// 邻接表的头指针数组
public:
    graph(int n, int e) {
        num_vertices = n;
        num_edges = e;
        list = new vertexNode[n + 1];// 从1-n个顶点
        for (int i = 1; i <= n; i++) {
            list[i].element = i;// 节点的编号
            list[i].next = NULL;
            list[i].weight = 0;// 到下一个节点的权重
        }
    }

    void insert(int start_vertex, int end_vertex, int weight) {  
        if (list[start_vertex].next == NULL) {// 如果该节点下一个为空
            list[start_vertex].next = new vertexNode(end_vertex, weight);
        }
        else {// 如果不为空，插入到链表的头部
            list[start_vertex].next = new vertexNode(end_vertex, weight, list[start_vertex].next);
        }
        
        // 处理无向图的另一个方向          
        if (list[end_vertex].next == NULL) { 
            list[end_vertex].next = new vertexNode(start_vertex, weight);
        }
        else {
            list[end_vertex].next = new vertexNode(start_vertex, weight, list[end_vertex].next);
        }

        // 维护边数
        num_edges++;
    }

    void prim() {
        minHeap min_heap(num_vertices);  
        reach[1] = 1;// 第一个节点肯定到了

        vertexNode* current_node = list[1].next;  
        long long total_weight = 0;     

        for (int i = 2; i <= num_vertices; i++) {// 遍历所有的节点
            while (current_node != NULL) {
                if (reach[current_node->element] != 1) {
                    vertexNode new_node(current_node->element, current_node->weight);
                    min_heap.push(new_node);
                }
                current_node = current_node->next;
            }
            while (reach[min_heap.top().element] == 1) {// 把重复加进来的访问过的节点去掉
                // 确保始终选择的是未访问顶点的最小权重边，避免了环的产生
                min_heap.pop();
            }
            total_weight += min_heap.top().weight;
            reach[min_heap.top().element] = 1;//只把最小边的节点标记为已访问
            current_node = list[min_heap.top().element].next;//把移动访问的指针移动到新节点
            //但堆中仍保留着所有已访问顶点的未处理边
            //最小堆会自动选择所有可用边中权重最小的
            min_heap.pop();
        }
        cout << total_weight << endl;
    }
};

int main() {
    int n, e;
    cin >> n >> e;
    graph g(n, e);
    for (int i = 0; i < e; i++) {
        int y, j, w;
        cin >> y >> j >> w;
        g.insert(y, j, w);
    }
    g.prim();
    return 0;
}