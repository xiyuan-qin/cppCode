#include<iostream>
using namespace std;

template<class T>
class Queue{
public:
    T *queue;
    int queue_front;
    int queue_back;
    int size;

public:
    Queue(int initial_size = 10){
        queue = new T[initial_size];
        size = initial_size;
        queue_front = 0;
        queue_back = 0;
    }

    ~Queue(){
        delete []queue;
    }

    bool empty() const {
        return queue_front == queue_back;
    }

    int size_of_queue() const{
        return (size + queue_back - queue_front) % size;
    }

    T& front() const{
        return queue[(queue_front + 1) % size];
    }

    T& back() const{
        return queue[queue_back];
    }

    void pop(){
        queue_front = (queue_front + 1) % size; 
        //queue[queue_front] = 0;
    }

    void push(T ele){
        if((queue_back + 1) % size == queue_front){ //满了
            // 扩容
            T *newQueue = new T[size * 2];
            int index = 0;
            for(int i = (queue_front + 1) % size; i != (queue_back + 1) % size; i = (i + 1) % size){
                newQueue[index++] = queue[i];
            }
            size *= 2;
            delete [] queue;
            queue = newQueue;
            queue_front = -1;
            queue_back = index - 1;
        }
        queue_back = (queue_back + 1) % size;
        queue[queue_back] = ele;
    }


};

int main(){
    int n;
    cin>>n;
    Queue<int> poke(n);
    for(int i = 1 ; i <= n ; i++){
        poke.push(i);
    }
    while (poke.size_of_queue() >= 2){
        poke.pop();
        poke.push(poke.front());
        poke.pop();
    }
    cout<<poke.front();
}