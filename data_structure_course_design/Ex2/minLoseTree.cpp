#include "minLoseTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <climits>

// 初始化数据到磁盘文件
void init(Variable &va) {
    std::cout << "请输入您想要的模拟磁盘位置，接下来的操作都是在当前路径下进行，输入样例：/Users/username/Desktop/text/" << std::endl;
    std::cout << "请输入："; 
    std::cin >> va.path; 
    va.file = va.path + va.fileName;
    
    std::cout << "请输入您想要在磁盘中初始化数字的个数，这些数字将用于模拟外排序过程："; 
    std::cin >> va.n;
    
    std::cout << "请输入缓冲区大小（此处为缓冲区能存储数字的个数）："; 
    std::cin >> va.m;
    
    std::cout << "请问您是否想要将原始数据，顺串，最终数据显示在控制台中('y' or 'n')："; 
    std::cin >> va.ch1; 
    std::cout << std::endl;

    std::ofstream fout1(va.file);
    if (!fout1.is_open()) {
        std::cout << "无法打开指定磁盘文件，无法初始化磁盘" << std::endl;
        exit(0);
    }
    
    if (va.ch1 == 'y') 
        std::cout << "原始磁盘文件有：" << std::endl;
    
    srand(time(nullptr));
    for (int i = 1; i <= va.n; i++) {
        int x = rand() % 1000 + 1;
        fout1 << x << ' ';
        if (va.ch1 == 'y') 
            std::cout << x << ' ';
    }
    
    if (va.ch1 == 'y') 
        std::cout << std::endl << std::endl;
    
    fout1.close();
}

// 构造顺串
void SequentialStringConstruction(Variable &va, SequentialStringPlayer* ssplayer) {
    std::ifstream fin1(va.file);
    if (!fin1.is_open()) {
        std::cout << "无法打开指定磁盘文件，无法进行顺串构造" << std::endl;
        exit(0);
    }
    
    // 将数据读入缓冲区，进行顺串构造
    for (int i = 1; i <= va.m; i++) {
        fin1 >> ssplayer[i].key;
        ssplayer[i].id = 1;
        va.visitDiskTime++; // 访存次数+1
    }
    
    MinimumLoserTree<SequentialStringPlayer> sstree(ssplayer, va.m);
    int num = 0;
    
    for (int i = 1; i <= va.n; i++) {
        // 依次从磁盘中取出元素，放入顺串生成树
        if (!(fin1 >> num)) 
            num = INT_MIN;
        else 
            va.visitDiskTime++;

        SequentialStringPlayer tempwinner = ssplayer[sstree.getTheWinner()];
        SequentialStringPlayer tempnum; 
        tempnum.key = num;

        if (num >= tempwinner.key) //局部决策：算法不会全局比较所有顺串的末尾元素，而是只与刚输出的元素比较
            tempnum.id = tempwinner.id;
        else 
            tempnum.id = (num == INT_MIN) ? INT_MAX : tempwinner.id + 1;

        sstree.rePlay(sstree.getTheWinner(), tempnum);

        std::string smallfile = va.path + "smallfile" + std::to_string(tempwinner.id) + ".txt";
        va.numberOfDisk = std::max(va.numberOfDisk, tempwinner.id);
        std::ofstream fout2(smallfile, std::ios::app);
        fout2 << tempwinner.key << ' ';

        fout2.close();
        va.visitDiskTime++;
    }
    
    fin1.close();
    std::cout << "顺串分配完毕，生成" << va.numberOfDisk << "个顺串，顺串文件见您当前路径下出现的新文件" << std::endl;
    
    if (va.ch1 == 'y') {
        std::cout << "我们将这些数据在这里显示如下：" << std::endl;
        for (int i = 1; i <= va.numberOfDisk; i++) {
            std::string smallfile = va.path + "smallfile" + std::to_string(i) + ".txt";
            std::ifstream finx(smallfile);
            int tempx = 0;
            std::cout << "smallfile" << i << ".txt: ";
            while (finx >> tempx)
                std::cout << tempx << ' ';
            std::cout << std::endl;
            finx.close();
        }
    }
}

// 生成最终结果
void GenerateTheFinalResult(Variable &va) {
    std::cout << std::endl << "请问是否将最终排序结果放入原文件，如果否，则程序将新建一个Disk2.txt文件并放入此文件中（输入'y'或'n'代表'是'与'否')："; 
    std::cin >> va.ch;
    
    std::string newFile;
    if (va.ch == 'y') 
        newFile = va.file;
    else 
        newFile = va.path + "Disk2.txt";

    std::ofstream fout3(newFile);

    if (va.numberOfDisk == 1) { // 只生成了一个顺串文件，直接将其加入目标文件
        std::string smallfile = va.path + "smallfile" + std::to_string(1) + ".txt";
        std::ifstream fin4(smallfile);
        int tempnumber;
        while (fin4 >> tempnumber) {
            fout3 << tempnumber << ' ';
            va.visitDiskTime += 2;
        }
        fout3.close();
        fin4.close();
        std::cout << "由于只生成了1个顺串，直接将此结果放入目标文件中，磁盘访存次数为" << va.visitDiskTime << "次，原因是每个数据都读写4次" << std::endl;
        
        if (va.ch1 == 'y') {
            std::cout << "最终外排序结果如下：" << std::endl;
            std::ifstream finy(newFile);
            int tempy;
            while (finy >> tempy)
                std::cout << tempy << ' ';
            std::cout << std::endl;
            finy.close();
        }
        return;
    }

    // 初始化K路归并
    int* dplayer = new int[va.numberOfDisk + 10]; // 初始化队列
    int* pointer = new int[va.numberOfDisk + 10]; // 各个小磁盘文件的指针
    
    for (int i = 1; i <= va.numberOfDisk; i++) {
        std::string smallfile = va.path + "smallfile" + std::to_string(i) + ".txt";
        std::ifstream fin2(smallfile);
        fin2 >> dplayer[i];
        pointer[i] = fin2.tellg();
        fin2.close();
    }
    
    MinimumLoserTree<int> dtree(dplayer, va.numberOfDisk);
    int cnt = 0;
    
    while (cnt < va.n) {
        cnt++;
        int temp = dtree.getTheWinner();
        int tempwinner = dplayer[temp];
        fout3 << tempwinner << ' ';
        va.visitDiskTime++;
        
        std::string smallfile = va.path + "smallfile" + std::to_string(temp) + ".txt";
        std::ifstream fin3(smallfile);
        fin3.clear(); 
        fin3.seekg(pointer[temp] + 1);

        int tempnum;
        if (pointer[temp] + 1 == 0) 
            tempnum = INT_MAX;
        else {
            fin3 >> tempnum;
            pointer[temp] = fin3.tellg();
            if (pointer[temp] + 1 == 0) 
                tempnum = INT_MAX;
        }
        
        va.visitDiskTime++;
        dtree.rePlay(temp, tempnum);
        fin3.close();
    }
    
    fout3.close();
    delete[] dplayer;
    delete[] pointer;
    
    std::cout << std::endl;
    std::cout << "将这些文件进行" << va.numberOfDisk << "路归并，已经结果存入到目标文件中。磁盘访存次数为" << va.visitDiskTime << "次，原因是每个数据都读写4次" << std::endl;
    
    if (va.ch1 == 'y') {
        std::cout << "最终外排序结果如下：" << std::endl;
        std::ifstream finy(newFile);
        int tempy;
        while (finy >> tempy)
            std::cout << tempy << ' ';
        std::cout << std::endl;
        finy.close();
    }
}

int main() {
    Variable va;

    init(va); // 初始化，生成随机磁盘文件

    SequentialStringPlayer* ssplayer = new SequentialStringPlayer[va.n + 10];

    SequentialStringConstruction(va, ssplayer); // 构造顺串

    GenerateTheFinalResult(va); // 生成最终结果

    delete[] ssplayer;
    return 0;
}