# 银行家算法
import numpy as np
def security(work, need, allocation):
    # 安全性算法
    n = need.shape[0]
    finish = np.array([False] * n, dtype=bool) # 表示系统是否有足够的资源分配给进程使之运行完成，默认全部为 False
    while not(finish.all()): # .all()意思是所有项都为真，则该函数返回 True，否则返回 False
        flag = False
        for i in range(n):
            if not finish[i] and (need[i] <= work).all():
                print("P{}".format(i), end=' -> ')
                flag = True
                work += allocation[i]
                finish[i] = True
                break
        if not flag:
            return False
    print()
    return True # 只有所有的finish都为true才安全


def printTable(available, max_table, allocation, need):
    # 输出当前状态
    print("=="*30)
    print("进程\tMax\tAllo\tNeed")
    for i in range(5):
        print("P{}\t{}\t{}\t{}".format(
            i, max_table[i], allocation[i], need[i]))
    print("当前剩余资源:", available)


def loadTestData():
    # 导入测试数据
    return (np.array([10, 5, 7], dtype=int),
        np.array([[7, 5, 3],
                  [3, 2, 2],
                  [9, 0, 2],
                  [2, 2, 2],
                  [4, 3, 3]], dtype=int),
        np.array([[0, 1, 0],
                  [2, 0, 0],
                  [3, 0, 2],
                  [2, 1, 1],
                  [0, 0, 2]], dtype=int))


def main():
    # 主循环
    # 手动输入数据
    # m = int(input("输入资源种类数 m: "))

    # temp = input("输入进程数资源，空格隔开：").split()
    # available = np.array(temp, dtype=int)

    # n = int(input("输入进程数 n: "))
    # max_table = np.zeros([n, m], dtype=int)
    # allocation = np.zeros([n, m], dtype=int)
    
    # # 输入最大需求资源
    # for i in range(n):
    #     temp = input("输入进程 P{} 的最大需求向量 Max：".format(i)).split()
    #     max_table[i] = np.array(temp, dtype=int)

    #     if (available < max_table[i]).any():
    #         print("[ERROR] 输入有误，重新输入")
    #         i -= 1
    # # 输入已分配资源
    # for i in range(n):
    #     temp = input("输入进程 P{} 的已分配资源 Max：".format(i)).split()
    #     allocation[i] = np.array(temp, dtype=int)

    #     if (max_table[i] < allocation[i]).any():
    #         print("[ERROR] 输入有误，重新输入")
    #         i -= 1
    
    # 导入测试数据，不用一次次手动输入
    available, max_table, allocation = loadTestData()
    need = max_table - allocation

    # 计算出剩余资源
    for i in allocation:
        available -= i

    printTable(available, max_table, allocation, need)

    while (need != 0).any():
        proc_ind, req = input("输入请求，如：P1, 1 0 1: ").split(',')
        proc_ind = int(proc_ind[1:])
        req = np.array(req.split(), dtype=int)
        # 测试用例
        """P1, 1 0 2
           P4, 3 3 0
           P0, 0 2 0"""

        # 判断合法性
        if (req > max_table[proc_ind]).any():
            print("[ERROR] 输入有误，重新输入")

        # 判断安全性
        else:
            available -= req
            allocation[proc_ind] += req
            need[proc_ind] -= req
            if security(available.copy(), need, allocation): # 用copy不然available会被修改
                printTable(available, max_table, allocation, need) # 打印当前状态
                continue
            else:
                print("[ERROR] 不安全，不能分配")
                available += req
                allocation[proc_ind] -= req
                need[proc_ind] += req

if __name__ == '__main__':
    main()