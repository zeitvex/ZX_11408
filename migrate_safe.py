import os
import shutil
import glob
import re

src_math = r"C:\Users\31560\Documents\00__11408资料库\math2master"
src_cs408 = r"C:\Users\31560\Documents\00__11408资料库\CS408_Note"
dest = r"C:\Users\31560\Documents\00__11408资料库\ZX_11408"

def copy_file(src_path, dest_path, append=False):
    if not os.path.exists(src_path):
        print(f"Source not found: {src_path}")
        return
    mode = "a" if append else "w"
    try:
        with open(src_path, "r", encoding="utf-8") as f_src:
            content = f_src.read()
        with open(dest_path, mode, encoding="utf-8") as f_dest:
            if append:
                f_dest.write("\n\n")
            f_dest.write(content)
    except Exception as e:
        print(f"Failed to copy {src_path}: {e}")

# Math mappings
math_map = {
    "高等数学": {
        "高等数学·1 函数、极限、连续.md": "02_函数_极限_连续.md",
        "高等数学·2 一元函数微分学.md": "03_一元函数微分学.md",
        "高等数学·3 一元函数积分学.md": "04_一元函数积分学.md",
        "高等数学·4 向量代数和空间解析几何.md": "05_向量代数与空间解析几何.md",
        "高等数学·7 无穷级数.md": "08_无穷级数.md",
        "高等数学·8 常微分方程.md": "09_常微分方程.md",
    },
    "线性代数": {
        "0x00 行列式.md": "01_行列式.md",
        "0x01 矩阵.md": "02_矩阵.md",
        "0x02 向量.md": "03_向量.md",
        "0x03 线性方程组.md": "04_线性方程组.md",
        "0x04 矩阵的特征值和特征向量.md": "05_特征值与特征向量.md",
        "0x05 二次型.md": "06_二次型.md",
    },
    "概率统计": {
        "0x00 随机事件和概率.md": "01_随机事件与概率.md",
        "0x01 随机变量及其分布.md": "02_随机变量及其分布.md",
        "0x02 多维随机变量及其分布.md": "03_多维随机变量及其分布.md",
        "0x03 随机变量的数字特征.md": "04_随机变量的数字特征.md",
        "0x04 大数定律和中心极限定理.md": "05_大数定律与中心极限定理.md",
        "0x05 数理统计的基本概念.md": "06_数理统计基本概念.md",
        "0x06 参数估计.md": "07_参数估计.md",
        "0x07 假设检验.md": "08_假设检验.md",
    }
}

# Special handling for multiple files -> 1 file
copy_file(os.path.join(src_math, "高等数学", "高等数学·5-1 多元函数微分学.md"), os.path.join(dest, "高数", "06_多元函数微分学.md"))
copy_file(os.path.join(src_math, "高等数学", "高等数学·5-2 多元函数微分学.md"), os.path.join(dest, "高数", "06_多元函数微分学.md"), append=True)
copy_file(os.path.join(src_math, "高等数学", "高等数学·5-3 多元函数微分学-公式汇总.md"), os.path.join(dest, "高数", "06_多元函数微分学.md"), append=True)

copy_file(os.path.join(src_math, "高等数学", "高等数学·6-1 多元函数积分学-重积分.md"), os.path.join(dest, "高数", "07_多元函数积分学_重积分与曲线曲面积分.md"))
copy_file(os.path.join(src_math, "高等数学", "高等数学·6-2 多元函数积分学-曲线积分.md"), os.path.join(dest, "高数", "07_多元函数积分学_重积分与曲线曲面积分.md"), append=True)
copy_file(os.path.join(src_math, "高等数学", "高等数学·6-3 多元函数积分学-曲面积分.md"), os.path.join(dest, "高数", "07_多元函数积分学_重积分与曲线曲面积分.md"), append=True)


for folder, mapping in math_map.items():
    dest_folder = "概率论" if folder == "概率统计" else folder
    if dest_folder == "高等数学": dest_folder = "高数"
    for src_file, dest_file in mapping.items():
        src_path = os.path.join(src_math, folder, src_file)
        dest_path = os.path.join(dest, dest_folder, dest_file)
        copy_file(src_path, dest_path)

# CS408 mappings
cs408_map = {
    "DataStructure": {
        "课程笔记/1-summary.md": "01_绪论.md",
        "课程笔记/2-linear-list.md": "02_线性表.md",
        "课程笔记/3-stack_queue_and_array.md": "03_栈_队列_数组.md",
        "课程笔记/4-string.md": "04_串.md",
        "课程笔记/5-tree_and_binary_tree.md": "05_树与二叉树.md",
        "课程笔记/6-graph.md": "06_图.md",
        "课程笔记/7-search.md": "07_查找.md",
        "课程笔记/8-sort.md": "08_排序.md",
        "EX-DS相关总结.md": "09_考频与重难点总结.md"
    },
    "ComputerOrganization": {
        "课程笔记/0-overview.md": "01_计算机系统概述.md",
        "课程笔记/1-data-representation-and-operation.md": "02_数据的表示和运算.md",
        "课程笔记/2-storage-system.md": "03_存储系统.md",
        "课程笔记/3-instruction-system.md": "04_指令系统.md",
        "课程笔记/4-central-processing-unit.md": "05_中央处理器.md",
        "课程笔记/5-bus.md": "06_总线.md",
        "课程笔记/6-input-output-system.md": "07_输入输出系统.md",
        "EX-CO相关总结.md": "08_考频与重难点总结.md"
    },
    "OperateSystem": {
        "课程笔记/0-summary.md": "01_操作系统概述.md",
        "课程笔记/1-process-management.md": "02_进程管理.md",
        "课程笔记/2-memory-management.md": "03_内存管理.md",
        "课程笔记/3-file-management.md": "04_文件管理.md",
        "课程笔记/4-device-management-ex.md": "05_输入输出管理.md",
        "EX-OS相关总结.md": "06_考频与重难点总结.md"
    },
    "ComputerNetwork": {
        "课程笔记/0-summary.md": "01_计算机网络体系结构.md",
        "课程笔记/1-physical-layer.md": "02_物理层.md",
        "课程笔记/2-data-link-layer.md": "03_数据链路层.md",
        "课程笔记/3-network-layer.md": "04_网络层.md",
        "课程笔记/4-transport-layer.md": "05_传输层.md",
        "课程笔记/5-application-layer.md": "06_应用层.md",
        "EX-CN相关总结.md": "07_考频与重难点总结.md"
    }
}

dest_folders = {
    "DataStructure": "数据结构",
    "ComputerOrganization": "计算机组成原理",
    "OperateSystem": "操作系统",
    "ComputerNetwork": "计算机网络"
}

for folder, mapping in cs408_map.items():
    dest_folder = dest_folders[folder]
    for src_file, dest_file in mapping.items():
        src_path = os.path.join(src_cs408, folder, src_file.replace("/", os.sep))
        dest_path = os.path.join(dest, dest_folder, dest_file)
        copy_file(src_path, dest_path)

print("Migration done. Fixing texts safely...")

def process_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            
        new_content = content
        if '高数' in filepath or '线性代数' in filepath or '概率论' in filepath:
            new_content = new_content.replace('image/', '/images/math2master/')
            
        new_content = re.sub(r'\{\{', '{ {', new_content)
        
        if new_content != content:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(new_content)
    except Exception as e:
        pass

files = glob.glob(os.path.join(dest, '**', '*.md'), recursive=True)
for file in files:
    if 'node_modules' not in file:
        process_file(file)

print("Done.")