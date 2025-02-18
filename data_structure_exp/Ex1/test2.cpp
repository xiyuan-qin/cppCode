#include<iostream>

using namespace std;

//static int result = 0;

int value(int a[], int n){//计算一个数组序列的价值
    int value=0;
     for(int i = 1; i <= n; i++){
        value += i ^ a[i-1];
    }
    return value;
}

template<class T>

void swap(int& a , int& b ){
    int  temp = a;
    a = b;
    b = temp;
}

void permutation(int a[] , int k ,int m ,int &result){
    if(k==m){
        result = result | value(a , m+1);
        /*for(int i = 0 ; i<=m;i++){
            cout<<a[i]<<" ";
        }
        cout<<endl;*/
    }
    else {
        for(int i= k ;i<= m;i++){
            swap(a[k],a[i]);
            permutation(a,k+1,m,result);
            swap(a[k],a[i]);
        }
    }
}
int main(){
    int length;
    cin>>length;
    int *a = new int [length] ; 
    for(int i = 0 ; i < length ; i++){
        cin>>a[i];
    }
    int result = 0;
    permutation( a, 0 , length-1,result);
    //cout<<value(a,3);
    //int b[] = {3, 1 , 2};
    //cout<<value(b,3);
    //cout<<(0 | 2 | 6 | 6 | 6 | 4 );
    cout<<(result|0);
    return 0;
}
