#include<iostream>

using namespace std;

void swap(int& a , int& b ){
    int  temp = a;
    a = b;
    b = temp;
}

template<class T>

class Sort{
public:
    Sort(){}
    void rank_sort(T* a , int size_of_a  ){
        //初始化rank of a
        rank_of_a = new int[size_of_a]; 
        for(int i = 0 ; i < size_of_a ; i++){
            rank_of_a[i] = 0;
        }

        //挨个确定排位，最小排位为0
        for(int i = 1 ;i < size_of_a ; i++){
            for(int j = 0; j < i ; j++){
                if(a[j]<=a[i]) rank_of_a[i]++;
                else rank_of_a[j]++;
            }
        }

        //原地重排
        for(int i = 0; i < size_of_a; i++){
            while (rank_of_a[i] != i){
                swap( a[i] , a[rank_of_a[i]] );
                swap( rank_of_a[i] , rank_of_a[rank_of_a[i]] );
            }
        }
    }

    void selection_sort(T* a , int size_of_a){
        bool sorted = false;
        for(int size = size_of_a ; size > 1&& !sorted ; size--){
            sorted = true;
            int max_index = 0 ;
            for(int i = 0 ; i <size ; i++){
                if(a[max_index] <= a[i])//找出最大的
                    max_index = i;
                else sorted = false;
            }
            swap(a[size - 1] , a[max_index] );
        }
    }

    void bubble_sort(T* a , int size_of_a){//将一次冒泡过程和多次过程分开
        for(int i = 0; i < size_of_a && bubble(a , size_of_a) ; i++);
    }

    void insertion_sort(T* a , int size_of_a){
        for(int i = 1 ; i < size_of_a ; i++){
            int temp = a[i];
            int j;
            for( j = i - 1 ; j >= 0 && temp < a[j] ; j--){
                a[j+1] = a[j]; 
            } 
            a[j + 1] = temp;
        }
    }
private:
    T* rank_of_a;

    bool bubble(T* a , int size_of_a){//一次冒泡过程，顺便判断
        bool swaped = false;
        for(int i = 0 ; i < size_of_a - 1 ; i++){
            if(a[i]>a[i+1]){
                swap(a[i],a[i+1]);
                swaped = true;
            }    
        }
        return swaped;
    }
};


int main(){
    int n ;
    cin>>n;
    int *vec = new int[n];
    for(int i = 0; i < n ; i++){
        cin>>vec[i];
    }

    Sort<int> srt;

    srt.selection_sort(vec , n );

    for(int i = 0; i < n ; i++){
        cout<<vec[i]<<" ";
    }

    return 0;
}