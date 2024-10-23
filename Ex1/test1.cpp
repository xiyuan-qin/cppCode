#include<iostream>   
 
using namespace std;

static int result = 0;//存储结果的变量

int value(int a[], int n){//计算一个数组序列的价值
    int value=0;
     for(int i = 1; i <= n; i++){
        value += i * a[i-1];
    }
    return value;
}


//最后需要和0异或

void gSubset(int a[] , int subset[] , int length_of_a , int subsize , int index_of_a){
    //如果当指针指向了原数组的最后一个数值，完成组合，打印子集
    if(index_of_a == length_of_a){
        /*for(int i = 0;i<subsize;i++){
            cout<<subset[i]<<" ";
        }
        cout<<endl;*/
        result = result ^ value(subset , subsize);
        return ;
    }
    //选择指针元素并且压入数组
    subset[subsize] = a[index_of_a];
    gSubset(a , subset , length_of_a , subsize + 1 , index_of_a +1 );
    //不选择指针元素继续递归
    gSubset(a , subset , length_of_a , subsize , index_of_a + 1 );

}

int main(){
    int length;
    cin>>length;
    int *a = new int [length] ; 
    for(int i = 0 ; i < length ; i++){
        cin>>a[i];
    }
    int subset[100];//临时存储子集的数组
    
    gSubset(a , subset , length ,0 ,0 );
    cout<<(int)(result^0);
    
    return 0;
}