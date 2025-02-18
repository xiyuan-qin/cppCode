#include <iostream>
using namespace std;

template<class T>
class minHeap {
public:
    T* heap;
    int heap_size;
    int array_length;

    minHeap(int heap_size) {//初始化堆
        this->heap_size = heap_size;
        heap = new T[heap_size + 1];
        array_length = heap_size;

        for (int i = 1; i <= heap_size; i++) {
            cin >> heap[i];
        }
        //从最后一个有孩子的节点开始，自底向上调整堆
        for (int root = heap_size / 2; root >= 1; root--) {
            T root_element = heap[root];
            int child = root * 2;
            while (child <= heap_size) {
                //找到左右孩子中较小的一个
                if (child < heap_size && heap[child] > heap[child + 1]) {
                    child++;
                }

                if (root_element <= heap[child]) {
                    break;
                }

                heap[child / 2] = heap[child];
                child *= 2;
            }
            heap[child / 2] = root_element;
        }
    }

    void push(T ele) {
        //如果数组满了，重新分配空间
        if (array_length == heap_size) {
            T* temp = new T[array_length * 2];
            for (int i = 1; i <= heap_size; i++) {
                temp[i] = heap[i];
            }
            array_length *= 2;
            delete[] heap;
            heap = temp;
        }

        //插入新元素
        int cur_node = ++heap_size;//首先插入到最后一个位置
        while (cur_node != 1 && heap[cur_node / 2] > ele) {//逐个判断是否需要上移
            heap[cur_node] = heap[cur_node / 2];
            cur_node /= 2;
        }
        heap[cur_node] = ele;
    }

    void pop() {
        if (heap_size == 0) return;

        //删除堆顶元素
        T last_ele = heap[heap_size--];
        int cur_node = 1, child = 2;
        //为最后一个元素找到合适的位置
        while (child <= heap_size) {
            //找到左右孩子中较小的一个
            if (child < heap_size && heap[child] > heap[child + 1]) {
                child++;
            }
            //一旦找到合适的位置，退出循环
            if (last_ele <= heap[child]) break;

            //不然交换上下节点，同时继续向下查找
            heap[cur_node] = heap[child];
            cur_node = child;
            child *= 2;
        }
        heap[cur_node] = last_ele;
    }

    T top() {
        return heap[1];
    }
};

void heapSort(int n) {
    minHeap<int> heap(n);
    for (int i = 0; i < n; i++) {
        cout << heap.top() << " ";
        heap.pop();
    }
    cout << endl;
}

int main() {
    int n;
    cin >> n;
    minHeap<int> heap(n);
    cout<<heap.top()<<endl;
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int opt;
        cin >> opt;
        switch (opt) {
            case 1:
                int num;
                cin >> num;
                heap.push(num);
                cout << heap.top() << endl;
                break;
            case 2:
                heap.pop();
                cout << heap.top() << endl;
                break;
            case 3:
                int n;
                cin >> n;
                heapSort(n);
                break;
        }
    }
    return 0;
}