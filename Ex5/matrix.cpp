#include<iostream>
using namespace std;

template<class T>
class matrixTerm{//三元组
public:
    int row; // 行号
    int col; // 列号
    T value; // 值
};

template<class T>
class sparseMatrix{
public:
    //构造方法
    sparseMatrix(int rows, int cols) {
        this->cols = cols;
        this->rows = rows;
        term = new matrixTerm<T>[(rows+13) * (cols+13)];
    }

    sparseMatrix() : term(nullptr), rows(0), cols(0), size(0) {}


    void operator=(sparseMatrix& a){
        rows = a.rows;
        cols = a.cols;
        size = a.size;
        for(int i = 0; i <size ; i++) term[i] = a.term[i];
    }

    //重置矩阵 1
    void reset(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        int temp_size = (rows + 13) * (cols + 13);//重定义线性表最大大小
        
        delete[] term; // 释放旧的内存
        term = new matrixTerm<T>[temp_size + 1];
        
        size = 0;//size才是真正的稀疏矩阵的大小，随着每输入一个元素增加
        int ele;
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                cin >> ele;
                if (ele != 0) {
                    term[size++] = {i, j, ele};  
                }
            }
        }
    }

    //矩阵转置 5
    void transpose() {
        if (size == 0) {//纯0矩阵直接调换行列数目
            int temp = rows;
            rows = cols;
            cols = temp;
            return;
        }

        sparseMatrix<T> b(cols, rows); // 新建转置后的矩阵
        b.size = size;

        int* col_size = new int[cols + 1]();//colSize用于存储原来矩阵每行非0元素的个数

        for (int i = 1; i <= size; ++i) {
            col_size[term[i].col]++;//统计
        }

        int* start_pos = new int[cols + 1];//startPos存储新矩阵每行第一个元素出现的位置
        start_pos[1] = 1;
        for (int i = 2; i <= cols; ++i) {
            start_pos[i] = start_pos[i - 1] + col_size[i - 1];
        }

        //转置
        for (int i = 1; i <= size; ++i) {
            int col = term[i].col;
            int pos = start_pos[col]++;//每次插入之后++，保证如果有新元素在同一列插在后面，防止被覆盖
            if (pos <= b.size) { // 确保不会越界
                b.term[pos] = {term[i].col, term[i].row, term[i].value};
            }
        }

        //把临时矩阵的属性赋回来
        delete[] term;
        term = b.term;
        rows = b.rows;
        cols = b.cols;
        size = b.size;
        b.term = nullptr;

        //维护内存
        delete[] col_size;
        delete[] start_pos;
    }

    //矩阵乘法 2
    void times(int other_rows, int other_cols, int t) {
        sparseMatrix<T> Q(other_rows, other_cols);
        Q.size = t;

        // 输入矩阵 Q
        for (int i = 1; i <= t; i++) {
            cin >> Q.term[i].row >> Q.term[i].col >> Q.term[i].value;
        }

        // 判断矩阵乘法是否合法
        if (cols != Q.rows) {
            delete[] term;
            term = Q.term;
            size = Q.size;
            rows = Q.rows;
            cols = Q.cols;
            cout << -1 << endl;
            return;
        }

        sparseMatrix<T> c(rows , Q.cols);
        int *row_none_0 = new int[Q.rows+ 1]();//Q第i行非零元素的个数
        int *row_index = new int[Q.cols + 1]();//第i行第一个非零元素的索引
        int *result = new int [Q.cols + 1]();
        for(int i = 0 ; i < Q.size ; i++){
            row_none_0[Q.term[i].row++];
        }
        row_index[1] = 0;
        for(int i = 2 ; i <= Q.rows ; i++) row_index[i] = row_none_0[i - 1] + row_index[i-1];

        int id = 0;
        for(int i = 1 ; id < size ; i++){//i是结果的行序号
            while(term[id].row == i){//通过while求出第i行所有结果
                if(row_none_0[term[id].col] != 0){
                    for(int x = row_index[term[id].col] ; x < row_index[term[id].col]+ row_none_0[term[id].col] ; x++){
                        result[Q.term[x].col] += term[id].value * Q.term[x].value;
                    }
                }
                id++;
            }
            
            for(int p = 1 ; p <= Q.cols ; p++ ){
                if(result[p] != 0){
                    c.term[c.size++] = {i ,p , result[p]};
                }
            }
        }

        *this = c;
        delete[] row_none_0;
        delete[] row_index;
        delete[] result;
    }

    void add(int other_rows, int other_cols, int t) {
        if (size == 0 && t == 0) return;//全是0直接返回

        sparseMatrix<T> Q(other_rows, other_cols);
        Q.size = t;

        //输入矩阵
        for (int i = 1; i <= t; i++) {
            cin >> Q.term[i].row >> Q.term[i].col >> Q.term[i].value;
        }

        //不能加的直接返回
        if (!(rows == Q.rows && cols == Q.cols)) {
            delete[] term;
            term = Q.term;
            size = Q.size;
            rows = Q.rows;
            cols = Q.cols;
            cout << -1 << endl;
            return;
        }

        sparseMatrix<T> c(rows, cols);//创建结果矩阵存放结果
        c.size = 0;

        int cp = 1, cq = 1;

        while (cp <= size && cq <= Q.size) {//直到有一个加完才退出
            if (term[cp].row < Q.term[cq].row ||//原先矩阵的位置小
                (term[cp].row == Q.term[cq].row && term[cp].col < Q.term[cq].col)) {

                c.term[++c.size] = term[cp++];
            } 
            else if (term[cp].row > Q.term[cq].row ||//新矩阵位置小
                       (term[cp].row == Q.term[cq].row && term[cp].col > Q.term[cq].col)) {

                c.term[++c.size] = Q.term[cq++];
            } 
            else {//两个矩阵能加
                T sum = term[cp].value + Q.term[cq].value;
                if (sum != 0) {
                    c.term[++c.size].row = term[cp].row;
                    c.term[c.size].col = term[cp].col;
                    c.term[c.size].value = sum;
                }
                //移位指针
                cp++;
                cq++;
            }
        }

        //累加后面的元素
        while (cp <= size) {
            c.term[++c.size] = term[cp++];
        }
        while (cq <= Q.size) {
            c.term[++c.size] = Q.term[cq++];
        }

        delete[] term;
        term = c.term;
        size = c.size;
        rows = c.rows;
        cols = c.cols;
        c.term = nullptr;
    }

    void print() {//打印数组
        cout << rows << " " << cols << endl;
        int k = 1;
        for (int i = 1; i <= rows; ++i) {
            for (int j = 1; j <= cols; ++j) {
                //相当于走一遍二维数组，有符合位置的输出三元组的value，没有输出0
                if (k <= size && term[k].row == i && term[k].col == j) {
                    cout << term[k++].value << " ";
                } else {
                    cout << 0 << " ";
                }
            }
            cout << endl;
        }
    }

public:
    int rows; // 行数
    int cols; // 列数
    int size = 0; // 线性表的大小
    matrixTerm<T> *term; // 从1开始
};

int main() {
    int w;
    cin >> w;
    sparseMatrix<int> matrix;
    for (int i = 0; i < w; i++) {
        int act, n, m, t;
        cin >> act;
        switch (act) {
        case 1:
            cin >> n >> m;
            matrix.reset(n, m);
            break;
        case 2:
            cin >> n >> m >> t;
            matrix.times(n, m, t);
            break;
        case 3:
            cin >> n >> m >> t;
            matrix.add(n, m, t);
            break;
        case 4:
            matrix.print();
            break;
        case 5:
            matrix.transpose();
            break;
        }
    }
    return 0;
}