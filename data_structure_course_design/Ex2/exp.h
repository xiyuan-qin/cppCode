#ifndef EX2_EXP_H
#define EX2_EXP_H

#include "minLoseTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <chrono>
#include <iomanip>
#include <sys/stat.h> // 用于创建目录

// 检查目录是否存在
bool dirExists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false; // 目录不存在
    }
    return (info.st_mode & S_IFDIR) != 0; // 确保是目录
}

// 创建目录
bool createDir(const std::string& path) {
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

// 初始化数据到磁盘文件
void init(Variable &va) {
    // 默认磁盘位置设为Ex2/test_output文件夹
    va.path = "/Users/code/cppCode/data_structure_course_design/Ex2/test_output/";
    
    // 确保目录存在
    if (!dirExists(va.path)) {
        std::cout << "创建输出目录: " << va.path << std::endl;
        if (!createDir(va.path)) {
            std::cout << "警告: 无法创建目录。将继续使用此路径，但可能会出错。" << std::endl;
        }
    }
    
    std::cout << "使用默认磁盘位置: " << va.path << std::endl;
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
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│            开始构造顺串...                            │" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::ifstream fin1(va.file);
    if (!fin1.is_open()) {
        std::cout << "无法打开指定磁盘文件，无法进行顺串构造" << std::endl;
        exit(0);
    }
    
    // 将数据读入缓冲区，进行顺串构造
    std::cout << "读取初始数据到缓冲区..." << std::endl;
    for (int i = 1; i <= va.m; i++) {
        fin1 >> ssplayer[i].key;
        ssplayer[i].id = 1;
        va.visitDiskTime++; // 访存次数+1
    }
    
    std::cout << "初始化败者树..." << std::endl;
    MinimumLoserTree<SequentialStringPlayer> sstree(ssplayer, va.m);
    int num = 0;
    
    std::cout << "开始分配顺串..." << std::endl;
    for (int i = 1; i <= va.n; i++) {
        // 依次从磁盘中取出元素，放入顺串生成树
        if (!(fin1 >> num)) 
            num = INT_MIN;
        else 
            va.visitDiskTime++;

        SequentialStringPlayer tempwinner = ssplayer[sstree.getTheWinner()];
        SequentialStringPlayer tempnum; 
        tempnum.key = num;

        if (num >= tempwinner.key) 
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
        
        // 显示进度
        if (i % (va.n / 10) == 0) {
            std::cout << "已处理: " << i * 100 / va.n << "%" << std::endl;
        }
    }
    
    fin1.close();
    
    auto endTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│            顺串构造完成                               │" << std::endl;
    std::cout << "├───────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│  生成顺串数量: " << std::setw(8) << va.numberOfDisk << std::setw(29) << "│" << std::endl;
    std::cout << "│  用时: " << std::setw(8) << elapsedTime << " ms" << std::setw(27) << "│" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
    if (va.ch1 == 'y') {
        std::cout << "顺串数据显示如下：" << std::endl;
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
        std::cout << std::endl;
    }
}

// 生成最终结果
void GenerateTheFinalResult(Variable &va) {
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│            开始K路归并...                             │" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
    std::cout << "请问是否将最终排序结果放入原文件，如果否，则程序将新建一个Disk2.txt文件并放入此文件中（输入'y'或'n'代表'是'与'否')："; 
    std::cin >> va.ch;
    
    std::string newFile;
    if (va.ch == 'y') 
        newFile = va.file;
    else 
        newFile = va.path + "Disk2.txt";

    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::ofstream fout3(newFile);

    if (va.numberOfDisk == 1) { // 只生成了一个顺串文件，直接将其加入目标文件
        std::cout << "只有一个顺串，直接复制到目标文件..." << std::endl;
        
        std::string smallfile = va.path + "smallfile" + std::to_string(1) + ".txt";
        std::ifstream fin4(smallfile);
        int tempnumber;
        while (fin4 >> tempnumber) {
            fout3 << tempnumber << ' ';
            va.visitDiskTime += 2;
        }
        fout3.close();
        fin4.close();
        
        auto endTime = std::chrono::high_resolution_clock::now();
        double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        
        std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│            归并完成                                   │" << std::endl;
        std::cout << "├───────────────────────────────────────────────────────┤" << std::endl;
        std::cout << "│  磁盘访问次数: " << std::setw(8) << va.visitDiskTime << std::setw(27) << "│" << std::endl;
        std::cout << "│  用时: " << std::setw(8) << elapsedTime << " ms" << std::setw(27) << "│" << std::endl;
        std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
        
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

    std::cout << "初始化" << va.numberOfDisk << "路归并..." << std::endl;
    
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
    
    std::cout << "创建败者树进行归并..." << std::endl;
    MinimumLoserTree<int> dtree(dplayer, va.numberOfDisk);
    int cnt = 0;
    
    std::cout << "开始归并处理..." << std::endl;
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
        
        // 显示进度
        if (cnt % (va.n / 10) == 0) {
            std::cout << "已处理: " << cnt * 100 / va.n << "%" << std::endl;
        }
    }
    
    fout3.close();
    delete[] dplayer;
    delete[] pointer;
    
    auto endTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│            归并完成                                   │" << std::endl;
    std::cout << "├───────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│  磁盘访问次数: " << std::setw(8) << va.visitDiskTime << std::setw(27) << "│" << std::endl;
    std::cout << "│  用时: " << std::setw(8) << elapsedTime << " ms" << std::setw(27) << "│" << std::endl;
    std::cout << "│  归并路数: " << std::setw(8) << va.numberOfDisk << std::setw(27) << "│" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
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

// 外排序实验主函数
void experiment() {
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│          最小败者树外排序实验                         │" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
    // 初始化变量
    Variable va;
    
    // 初始化数据
    std::cout << ">>> 初始化数据\n" << std::endl;
    init(va);
    
    // 为顺串处理创建数组
    SequentialStringPlayer* ssplayer = new SequentialStringPlayer[va.n + 10];
    
    // 构造顺串
    SequentialStringConstruction(va, ssplayer);
    
    // 生成最终结果
    GenerateTheFinalResult(va);
    
    // 释放资源
    delete[] ssplayer;
    
    std::cout << "┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│          败者树外排序实验结束                         │" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘" << std::endl;
}

#endif // EX2_EXP_H
