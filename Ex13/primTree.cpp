#include<iostream>
using namespace std;

class vertexNode {
public:
    int weight;
    int element;
    vertexNode* next;

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
};

int reach[2000000] = { 0 };//记录是否到达过该点

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

    void push(vertexNode x) {
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
    int num_vertices;  
    int num_edges;     
    vertexNode* list;
public:
    graph(int n, int e) {
        num_vertices = n;
        num_edges = e;
        list = new vertexNode[n + 1];
        for (int i = 1; i <= n; i++) {
            list[i].element = i;
            list[i].next = NULL;
            list[i].weight = 0;
        }
    }

    void insert(int start_vertex, int end_vertex, int weight) {  
        vertexNode* insert_node = new vertexNode(end_vertex, weight);
        vertexNode* next_node = list[start_vertex].next;
        if (next_node == NULL) {
            list[start_vertex].next = insert_node;
        }
        else {
            list[start_vertex].next = insert_node; 
            insert_node->next = next_node;
        }
        
        // 处理无向图的另一个方向
        vertexNode* reverse_node = new vertexNode(start_vertex, weight);  
        vertexNode* reverse_next = list[end_vertex].next;                 
        if (reverse_next == NULL) { 
            list[end_vertex].next = reverse_node;
        }
        else {
            list[end_vertex].next = reverse_node; 
            reverse_node->next = reverse_next;
        }
        num_edges++;
    }

    void prim() {
        minHeap min_heap(num_vertices);  // 原mh
        reach[1] = 1;
        vertexNode* current_node = list[1].next;  // 原nextnode
        long long total_weight = 0;               // 原sumw
        for (int i = 2; i <= num_vertices; i++) {
            while (current_node != NULL) {
                if (reach[current_node->element] != 1) {
                    vertexNode new_node(current_node->element, current_node->weight);
                    min_heap.push(new_node);
                }
                current_node = current_node->next;
            }
            while (reach[min_heap.top().element] == 1) {
                min_heap.pop();
            }
            total_weight += min_heap.top().weight;
            reach[min_heap.top().element] = 1;
            current_node = list[min_heap.top().element].next;
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