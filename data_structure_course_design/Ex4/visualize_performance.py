#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import csv

def create_ascii_chart(data):
    """创建简单的ASCII文本图表"""
    node_counts = [row[0] for row in data[1:]]
    greedy_times = [float(row[1]) for row in data[1:]]
    dp_times = [float(row[2]) for row in data[1:]]
    
    # 找出最大值用于缩放
    max_time = max(max(greedy_times), max(dp_times))
    scale = 40 / max_time  # 40个字符宽度
    
    print("\n===== 算法执行时间比较 =====")
    print("节点数量 | 贪心算法 | 动态规划")
    print("-" * 50)
    
    for i in range(len(node_counts)):
        greedy_bar = "#" * int(greedy_times[i] * scale)
        dp_bar = "*" * int(dp_times[i] * scale)
        print(f"{node_counts[i]:8} | {greedy_bar} {greedy_times[i]:.2f}ms")
        print(f"         | {dp_bar} {dp_times[i]:.2f}ms")
        print("-" * 50)
    
    print("图例: # 贪心算法, * 动态规划算法")
    
    # 创建比率图
    print("\n===== 动态规划 / 贪心算法 时间比率 =====")
    for i in range(len(node_counts)):
        ratio = dp_times[i] / greedy_times[i]
        bar = "=" * int(ratio * 10)
        print(f"{node_counts[i]:8} | {bar} {ratio:.2f}")
    
    print("-" * 50)
    print("值 > 1: 动态规划更慢, 值 < 1: 动态规划更快")

def plot_with_libraries():
    """使用pandas和matplotlib生成高质量图表"""
    try:
        import pandas as pd
        import matplotlib.pyplot as plt
    except ImportError:
        print("警告: 缺少必要的库pandas或matplotlib。")
        print("请先创建虚拟环境并安装库:")
        print("\npython3 -m venv venv")
        print("source venv/bin/activate")
        print("pip install pandas matplotlib\n")
        return False
    
    # 读取性能测试数据
    csv_path = os.path.join('data', 'performance_results.csv')
    data = pd.read_csv(csv_path)
    
    # 创建图形和子图
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 15))
    
    # 图1：算法执行时间比较
    ax1.plot(data['NodeCount'], data['GreedyTime(ms)'], 'o-', color='blue', label='贪心算法')
    ax1.plot(data['NodeCount'], data['DPTime(ms)'], 's-', color='red', label='动态规划算法')
    ax1.set_title('算法执行时间比较')
    ax1.set_xlabel('节点数量')
    ax1.set_ylabel('执行时间 (ms)')
    ax1.grid(True, linestyle='--', alpha=0.7)
    ax1.legend()
    
    # 图2：算法执行时间比率
    ratio = data['DPTime(ms)'] / data['GreedyTime(ms)']
    ax2.plot(data['NodeCount'], ratio, 'o-', color='green')
    ax2.axhline(y=1, color='grey', linestyle='--', alpha=0.7)  # 添加基准线
    ax2.set_title('动态规划 / 贪心算法 时间比率')
    ax2.set_xlabel('节点数量')
    ax2.set_ylabel('时间比率')
    ax2.grid(True, linestyle='--', alpha=0.7)
    
    # 图3：算法放置的放大器数量比较
    ax3.bar(data['NodeCount'] - 1.5, data['GreedyAmplifiers'], width=3, color='blue', alpha=0.6, label='贪心算法')
    ax3.bar(data['NodeCount'] + 1.5, data['DPAmplifiers'], width=3, color='red', alpha=0.6, label='动态规划算法')
    ax3.set_title('算法放置的放大器数量比较')
    ax3.set_xlabel('节点数量')
    ax3.set_ylabel('放大器数量')
    ax3.grid(True, linestyle='--', alpha=0.7)
    ax3.legend()
    
    # 保存图表到data目录
    plt.tight_layout()
    output_path = os.path.join('data', 'algorithm_comparison.png')
    plt.savefig(output_path, dpi=300)
    
    print(f"图表已保存至: {output_path}")
    
    # 显示图表
    plt.show()
    return True

def main():
    # 确保在脚本所在目录下运行
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    
    # 读取CSV文件
    csv_path = os.path.join('data', 'performance_results.csv')
    try:
        with open(csv_path, 'r') as file:
            csv_reader = csv.reader(file)
            data = list(csv_reader)
    except Exception as e:
        print(f"无法读取性能结果文件: {e}")
        return
    
    # 先尝试使用matplotlib和pandas生成图表
    if not plot_with_libraries():
        # 如果失败，使用ASCII图表
        print("\n使用ASCII文本图表展示结果:")
        create_ascii_chart(data)
        print("\n如需生成更美观的图表，请按照上面的说明安装pandas和matplotlib库。")

if __name__ == '__main__':
    main()