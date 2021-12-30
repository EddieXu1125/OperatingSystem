from random import *

# Init
page_box = 3    # 页框数量 
Max_logical_page = 7    # 最大逻辑页号
random_length = 17  # 随机序列长度 


class page:
    def __init__(self,num,time):      
        # 记录页面号
        self.num = num  
        # 记录调入内存时间
        self.time = time

class main:
    def __init__(self,sequence):
        # 初始化内存单元
        self.b = [page(-1,page_box-i-1) for i in range(0,page_box)]
        # 初始化内存当前状态，缓冲区
        self.state = [[-1 for i in range(0,random_length)] for j in range(0,page_box)]
        # 记录调入队列
        self.queue = []
        self.k = -1
        self.flag =-1
        self.process(sequence)


    def print_string(self):
        print("|"+(random_length-1)*"---+"+"---|")

    # 取得在内存中停留最久的页面，默认状态下为最早点入的页面
    def get_max(self,b):
        max = -1
        flag = 0
        for i in range(0,page_box):
            if b[i].time >max:
                max = b[i].time
                flag = i
        return flag 

    # 判断页面是否已在内存中
    def equation(self,fold,b):
        for i in range(0,page_box):
            if fold == b[i].num:
                return i
        return -1
        
    # LRU 算法
    # 本质就是找内存中驻留时间最长的然后进行替换。
    def lru(self, fold, b):
        val = self.equation(fold, b)
        if val >= 0:
            b[val].time = 0
            for i in range(0, page_box):
                if i != val:
                    b[i].time += 1
        else:
            self.queue.append(fold)
            self.k += 1
            val = self.get_max(b)
            b[val].num = fold
            b[val].time = 0
            for i in range(0, page_box):
                if (i != val):
                    b[i].time += 1

    # FIFO 算法
    # 轮流替换内存中的值
    def fifo(self, fold, b):
        val = self.equation(fold, b)
        if val >= 0:
            pass
        else:
            self.queue.append(fold)
            self.k += 1
            self.flag += 1
            self.flag %= page_box
            self.b[self.flag].num = fold


    # 打印内存状态
    def Myprint(self,a):
        self.print_string()
        for j in range(0, random_length):
            print("|%2d" % (a[j]), end=" ")
        print("|")
        self.print_string()
        for i in range(0, page_box):
            for j in range(0, random_length):
                if self.state[i][j] == -1:
                    print("|%2c" % (32), end=" ")
                else:
                    print("|%2d" % (self.state[i][j]), end=" ")
            print("|")
        self.print_string()
        print("调入队列为")
        for i in range(0, self.k + 1):
            print("%2d" % (self.queue[i]), end=" ")
        print("\n缺页次数为:%6d\n缺页率:%16.6f" % (self.k + 1, (float)(self.k + 1) / random_length))
    
    # 主程序
    def process(self,a):
        for i in range(0, random_length):
            self.fifo(a[i], self.b)
            self.state[0][i] = a[i]

            # 记录当前的内存单元中的页面
            for j in range(0, page_box):
                self.state[j][i] = self.b[j].num
        # 结果输出
        print("fifo算法内存状态为:")
        self.Myprint(a)

        # 初始化内存单元
        self.b = [page(-1, page_box - i - 1) for i in range(0, page_box)]
        # 初始化内存当前状态，缓冲区
        self.state = [[-1 for i in range(0, random_length)] for j in range(0, page_box)]
        # 记录调入队列
        self.queue = []
        self.k = -1
        for i in range(0, random_length):
            self.lru(a[i], self.b)
            self.state[0][i] = a[i]

            # 记录当前的内存单元中的页面
            for j in range(0, page_box):
                self.state[j][i] = self.b[j].num
        # 结果输出
        print("lru算法内存状态为")
        self.Myprint(a)

if __name__ == "__main__":
    print('页面置换算法仿真实验')
    print('1 指定页框，最大逻辑页，随机生成指定长度（大于1000）的访问序列，并计算其FIFO LRU缺页率')
    print('2 进行3个页框，最大逻辑页号为7，20个页面访问序列7 0 1 2 0 3 0 4 2 3 0 3 2 5 4 3 6 5 3 2的仿真，打印输出内存置换情况')
    res = input('请选择1/2: ')
    
    if int(res) == 1:
        page_box = int(input('请输入页框数量: '))
        Max_logical_page = int(input('请输入最大逻辑页号: '))
        random_length = int(input('请输入一个不小于1000的数，以采用伪随机数发生器产生相应次页面访问的序列，统计缺页率(为了方便输出，输入30左右的数字也行): '))
        s = [randint(0,Max_logical_page) for _ in range(random_length)]

    else:
        page_box = 3
        Max_logical_page = 7
        s = [7,0,1,2,0,3,0,4,2,3,0,3,2,5,4,3,6,5,3,2]
        random_length = len(s)       
       
    main(s)