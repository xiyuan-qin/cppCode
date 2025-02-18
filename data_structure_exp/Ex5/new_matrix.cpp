#include<iostream>
using namespace std;

class MatrixTerm {
public:
    int row, col, value;
    void operator=(MatrixTerm& a) {  // 重载=
        row = a.row;
        col = a.col;
        value = a.value;
    }
};

class SparseMatrix {
private:
    int rows, cols;  // 行、列
    int size = 0;  // 非零元素的个数初始化为0
    MatrixTerm* term;  // 存非零元素的结构体数组

public:
    SparseMatrix(int n = 0, int m = 0) {
        rows = n;
        cols = m;
        term = new MatrixTerm[(n + 13) * (m + 13)];
    }

    ~SparseMatrix() {  // 析构函数
        delete[] term;
    }

    void operator=(SparseMatrix& a) {  // 重载=，将一个矩阵赋给另一个矩阵
        rows = a.rows;
        cols = a.cols;
        size = a.size;
        delete[] term;  // 释放旧的内存
        term = new MatrixTerm[a.size];
        for (int i = 0; i < size; i++) term[i] = a.term[i];
    }

    // 输入矩阵
    void input(int x, int y, int v) {
        term[size].row = x;
        term[size].col = y;
        term[size].value = v;
        size++;
    }

    void reset() {//重置矩阵
        int n, m;
        cin >> n >> m;
        rows = n;
        cols = m;
        size = 0;
        delete[] term;
        term = new MatrixTerm[(n + 13) * (m + 13)];

        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                int v;
                cin >> v;
                if (v != 0) {
                    term[size].row = i;
                    term[size].col = j;
                    term[size].value = v;
                    size++;
                }
            }
        }
    }


    //O（n^3）
    void times() {//矩阵乘法
        int n, m, nz;
        cin >> n >> m >> nz;
        SparseMatrix b(n, m);
        for (int i = 0; i < nz; i++) {
            int x, y, v;
            cin >> x >> y >> v;
            b.input(x, y, v);
        }

        if (cols != b.rows) {
            *this = b;
            cout << -1 << endl;
            return;
        }

        SparseMatrix c(rows, b.cols);
        int* row_none_0 = new int[b.rows + 1];  // b第i行非零元素的个数
        int* row_index = new int[b.rows + 1];  // b第i行第一个非零元素的索引
        int* result = new int[b.cols + 1];  // 乘后中每一行的结果

        for (int i = 1; i <= b.rows; i++) row_none_0[i] = 0;
        for (int i = 0; i < b.size; i++) row_none_0[b.term[i].row]++;

        row_index[1] = 0;
        for (int i = 2; i <= b.rows; i++) row_index[i] = row_index[i - 1] + row_none_0[i - 1];

        int id = 0;
        for (int i = 1; id < size; i++) {//计算第i行，i 表示当前正在处理的行，而 id 表示当前处理的非零元素的索引。
            //只要id走到size证明走完了
            //清零result数组
            for (int j = 1; j <= b.cols; j++) result[j] = 0;

            while (term[id].row == i) {
                if (row_none_0[term[id].col] != 0) {//如果b中对应的列有非零元素，往下乘row_none_0个元素
                    for (int x = row_index[term[id].col]; x < row_index[term[id].col] + row_none_0[term[id].col]; x++)
                        result[b.term[x].col] += term[id].value * b.term[x].value;//把结果存到result
                }
                id++;
            }
            
            for (int p = 1; p <= b.cols; p++) {
                if (result[p] != 0) {
                    c.term[c.size].value = result[p];
                    c.term[c.size].row = i;
                    c.term[c.size].col = p;
                    c.size++;
                }
            }
        }
        
        *this = c;
        delete[] row_none_0;
        delete[] row_index;
        delete[] result;
    }

    //复杂度O(a.size + b.size) ,遍历两个数组的元素相加
    void add() {//矩阵加法 

        int n, m, nz;
        cin >> n >> m >> nz;
        SparseMatrix b(n, m);
        for (int i = 0; i < nz; i++) {
            int x, y, v;
            cin >> x >> y >> v;
            b.input(x, y, v);
        }

        if (cols != b.cols || rows != b.rows) {
            *this = b;
            cout << -1 << endl;
            return;
        }

        SparseMatrix c(rows, cols);
        int an = 0, bn = 0;
        while (an < size && bn < b.size) {
            int aindex = (term[an].row - 1) * cols + term[an].col;//a在实际矩阵中的相对位置
            int bindex = (b.term[bn].row - 1) * cols + b.term[bn].col;//b在实际矩阵中的相对位置

            if (aindex < bindex) {
                c.term[c.size] = term[an];//a小a就先插
                an++;
                c.size++;
            }
            else if (aindex == bindex) {//相等的情况下，相加
                if (term[an].value + b.term[bn].value != 0) {
                    c.term[c.size].col = term[an].col;
                    c.term[c.size].row = term[an].row;
                    c.term[c.size].value = term[an].value + b.term[bn].value;
                    c.size++;
                }
                an++;
                bn++;
            }
            else {//b小，b相加
                c.term[c.size] = b.term[bn];
                bn++;
                c.size++;
            }
        }
        //累加剩下的元素
        while (an < size) {
            c.term[c.size] = term[an];
            an++;
            c.size++;
        }

        while (bn < b.size) {
            c.term[c.size] = b.term[bn];
            bn++;
            c.size++;
        }

        *this = c;//把加好的矩阵赋给原来的
    }

    void print() {  // 输出
        cout << rows << " " << cols << endl;
        
        int t = 0;
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (t < size && term[t].row == i && term[t].col == j) {
                    cout << term[t].value << " ";
                    t++;
                }
                else {
                    cout << 0 << " ";
                }
            }
            cout << endl;
        }
    }

    //复杂度: O(size) , 遍历原矩阵的非0元素，其他的都是累加相当于k * size ， k拿掉
    void transpose() {  // 转置
        SparseMatrix b(cols, rows);
        b.size = size;

        int* col_none_0 = new int[cols + 1];//新矩阵行非0元素个数
        int* col_index = new int[cols + 1];//首个非0元素位置

        for (int i = 1; i <= cols; i++) col_none_0[i] = 0;
        for (int i = 0; i < size; i++) col_none_0[term[i].col]++;

        col_index[1] = 0;
        for (int i = 2; i <= cols; i++) col_index[i] = col_index[i - 1] + col_none_0[i - 1];

        for (int i = 0; i < size; i++) {
            int j = col_index[term[i].col];
            b.term[j].col = term[i].row;
            b.term[j].row = term[i].col;
            b.term[j].value = term[i].value;
            col_index[term[i].col]++;
        }

        *this = b;
        delete[] col_none_0;
        delete[] col_index;
    }
};

int main() {
    int w;
    SparseMatrix a;
    cin >> w;
    for (int i = 0; i < w; i++) {
        int x;
        cin >> x;
        if (x == 1) {
            a.reset();  
        }
        else if (x == 2) {
            a.times(); 
        }
        else if (x == 3) {
            a.add();  
        }
        else if (x == 4) {
            a.print();
        }
        else if (x == 5) {
            a.transpose();
        }
    }
    return 0;
}
