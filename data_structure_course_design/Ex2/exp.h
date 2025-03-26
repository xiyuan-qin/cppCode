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
#include <sys/stat.h> 
#include <vector> 

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
        fout1 << x << ' ';// 将数据写入磁盘文件
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

// 多阶段归并处理函数
int MultiStageMerge(Variable &va, const std::vector<std::string>& inputFiles, 
                   const std::string& outputFile, bool isLastStage) {
    int maxMergeWays = va.m; // 最多使用缓冲区大小作为归并路数
    int totalFiles = inputFiles.size();
    int mergeGroups = (totalFiles + maxMergeWays - 1) / maxMergeWays; // 向上取整，计算需要多少组
    std::vector<std::string> nextStageFiles;
    
    std::cout << "多阶段归并: " << totalFiles << "个文件，分为" << mergeGroups << "组进行处理" << std::endl;
    
    for (int group = 0; group < mergeGroups; group++) {
        // 计算当前组的起始和结束索引
        int startIdx = group * maxMergeWays;
        int endIdx = std::min((group + 1) * maxMergeWays, totalFiles);
        int currentGroupSize = endIdx - startIdx;
        
        // 当前组只有一个文件并且是最后一组，直接复制到输出
        if (currentGroupSize == 1 && group == mergeGroups - 1 && isLastStage) {
            std::ifstream inFile(inputFiles[startIdx]);
            std::ofstream outFile(outputFile);
            int value;
            while (inFile >> value) {
                outFile << value << " ";
                va.visitDiskTime += 2; // 读和写
            }
            inFile.close();
            outFile.close();
            return 1; // 返回剩余文件数
        }
        
        // 确定当前组的输出文件名
        std::string currentOutputFile;
        if (mergeGroups == 1 && isLastStage) {
            currentOutputFile = outputFile;
        } else {
            currentOutputFile = va.path + "stageMerge_" + std::to_string(group) + ".txt";
            nextStageFiles.push_back(currentOutputFile);
        }
        
        std::cout << "处理组 " << (group + 1) << "/" << mergeGroups
                  << " (" << currentGroupSize << " 个文件)" << std::endl;
                  
        // 准备K路归并
        int* dplayer = new int[currentGroupSize + 1];
        int* pointer = new int[currentGroupSize + 1];
        std::vector<std::ifstream*> fileStreams(currentGroupSize + 1);
        
        // 初始化每个文件的第一个值和指针位置
        for (int i = 0; i < currentGroupSize; i++) {
            fileStreams[i + 1] = new std::ifstream(inputFiles[startIdx + i]);
            *fileStreams[i + 1] >> dplayer[i + 1];
            pointer[i + 1] = fileStreams[i + 1]->tellg();
            va.visitDiskTime++;
        }
        
        // 构建败者树
        MinimumLoserTree<int> dtree(dplayer, currentGroupSize);
        
        // 打开输出文件
        std::ofstream outFile(currentOutputFile);
        int fileElementCount = 0;
        
        // K路归并过程
        while (true) {
            int winnerIdx = dtree.getTheWinner();
            int winner = dplayer[winnerIdx];
            
            // 写入当前最小值
            outFile << winner << " ";
            fileElementCount++;
            va.visitDiskTime++;
            
            // 从获胜文件读取下一个值
            std::ifstream* fin = fileStreams[winnerIdx];
            fin->clear();
            fin->seekg(pointer[winnerIdx] + 1);
            
            int nextVal;
            if (!(*fin >> nextVal)) {
                // 文件结束，使用最大值代替
                nextVal = INT_MAX;
            } else {
                pointer[winnerIdx] = fin->tellg();
                va.visitDiskTime++;
            }
            
            // 更新败者树
            dtree.rePlay(winnerIdx, nextVal);
            
            // 检查是否所有文件都已处理完
            bool allDone = true;
            for (int i = 1; i <= currentGroupSize; i++) {
                if (dplayer[i] != INT_MAX) {
                    allDone = false;
                    break;
                }
            }
            
            if (allDone) break;
            
            // 显示进度
            if (fileElementCount % 10000 == 0) {
                std::cout << "已处理 " << fileElementCount << " 个元素" << std::endl;
            }
        }
        
        outFile.close();
        
        // 清理资源
        for (int i = 1; i <= currentGroupSize; i++) {
            fileStreams[i]->close();
            delete fileStreams[i];
        }
        delete[] dplayer;
        delete[] pointer;
    }
    
    // 递归处理下一阶段
    if (nextStageFiles.size() > 1) {
        return MultiStageMerge(va, nextStageFiles, outputFile, true);
    } else if (nextStageFiles.size() == 1) {
        // 只有一个中间文件，直接重命名为最终输出
        if (isLastStage) {
            std::ifstream inFile(nextStageFiles[0]);
            std::ofstream outFile(outputFile);
            int value;
            while (inFile >> value) {
                outFile << value << " ";
                va.visitDiskTime += 2;
            }
            inFile.close();
            outFile.close();
        }
        return 1;
    }
    
    return mergeGroups;
}

// 修改最终结果生成函数，直接使用多阶段归并
void GenerateTheFinalResult(Variable &va) {
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│            开始多阶段K路归并...                       │" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
    std::cout << "请问是否将最终排序结果放入原文件，如果否，则程序将新建一个Disk2.txt文件并放入此文件中（输入'y'或'n'代表'是'与'否')："; 
    std::cin >> va.ch;
    
    std::string newFile;
    if (va.ch == 'y') 
        newFile = va.file;
    else 
        newFile = va.path + "Disk2.txt";

    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 处理只有一个顺串的情况
    if (va.numberOfDisk == 1) {
        std::cout << "只有一个顺串，直接复制到目标文件..." << std::endl;
        
        std::ofstream fout3(newFile);
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
    
    // 直接采用多阶段归并方法
    std::cout << "使用多阶段K路归并 (缓冲区大小m=" << va.m << ")..." << std::endl;
    
    // 计算多阶段归并的理论阶段数
    int numberOfRuns = va.numberOfDisk;
    int bufferSize = va.m;
    int theoreticalStages = 0;
    int temp = numberOfRuns;
    while (temp > 1) {
        temp = (temp + bufferSize - 1) / bufferSize; // 向上取整
        theoreticalStages++;
    }
    
    std::cout << "理论归并阶段数: " << theoreticalStages << std::endl;
    
    // 准备初始顺串列表
    std::vector<std::string> inputFiles;
    for (int i = 1; i <= va.numberOfDisk; i++) {
        inputFiles.push_back(va.path + "smallfile" + std::to_string(i) + ".txt");
    }
    
    // 执行多阶段归并
    int actualStages = MultiStageMerge(va, inputFiles, newFile, true);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    std::cout << "\n┌───────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│            多阶段归并完成                             │" << std::endl;
    std::cout << "├───────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│  磁盘访问次数: " << std::setw(8) << va.visitDiskTime << std::setw(27) << "│" << std::endl;
    std::cout << "│  用时: " << std::setw(8) << elapsedTime << " ms" << std::setw(27) << "│" << std::endl;
    std::cout << "│  归并路数: " << std::setw(8) << va.m << std::setw(27) << "│" << std::endl;
    std::cout << "│  归并阶段数: " << std::setw(8) << actualStages << std::setw(27) << "│" << std::endl;
    std::cout << "└───────────────────────────────────────────────────────┘\n" << std::endl;
    
    // 打印关键理论指标
    int totalIO = va.visitDiskTime;
    int theoreticalIO = (2 * va.n * (theoreticalStages + 1)); // 理论I/O次数
    
    std::cout << "理论分析:\n" << std::endl;
    std::cout << "  - 理论最优I/O次数: " << theoreticalIO << std::endl;
    std::cout << "  - 实际I/O次数: " << totalIO << std::endl;
    std::cout << "  - I/O效率: " << (theoreticalIO > 0 ? (double)theoreticalIO / totalIO * 100 : 0) << "%" << std::endl << std::endl;
    
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
