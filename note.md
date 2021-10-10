# 进程管理

### 信号量机制

**整形信号量**

用一个整形变量作为信号量，数值表示某种资源数

除初始化外，仅能通过两个标准的原子操作(Atomic Operation) wait(S)和signal(S)来访问。

这两个操作一直被分别称为P、V操作。（荷兰语的proberen和verhogen）

```c
wait(S){
    while (S≤0);//wait操作，只要是信号量S≤0， 就会不断地测试。
    S=S-1;
}
signal(S){
    S=S+1;
}
```

存在的问题：不满足**让权等待**原则，会发生“忙等”。（系统的资源暂时不够的话，某一进程会一直占用cpu，一直循环检查）

**记录型信号量**

```c
Typedef struct{
    int value; //表示某种资源数
    struct process_control_block *L; //指向等待该资源的队列
} semaphore;
```

用一个整型变量代表资源数目, 进程链表链接上述的所有等待进程, 采用了记录型的数据结构
```c
void wait(semaphore *S){
    S.value--;
    if (S.value<0){
        block(S.L);
    }
    //如果剩余资源数不够,将该进程先阻塞
}
signal(semaphore *S){
    S->value++;
    if (S.value<=0) {
        wakeup(S->L)
    };
}
```

可以用记录型信号量实现系统资源的"申请"和"释放"
可以用记录型信号量实现进程互斥、进程同步。




**用信号量实现进程互斥**

1、分析并发进程的关键活动，划定临界区（可以理解为一种特殊的资源、这种资源只有一个，他只能被一个进程所使用）
2、设置互斥信号量**mutex**，初值为**1**
3、在临界区之前执行P(mutex)
4、在临界区之后执行V(mutex)

```c
semaphore mutex=1

p1(){
    P(mutex)
    临界区代码段
    V(mutex)
}

p2(){
    P(mutex)
    临界区代码段
    V(mutex)
}
```

**将信号量设置为1能够实现临界区的互斥访问**
注意：对不同临界资源需要设置不同的互斥信号量

P V 操作必须成对出现。缺少P(mutex)就不能保证临界资源的互斥访问。缺少V(mutex)就会导致资源永不被释放，等待进程永不被唤醒。







**用信号量实现进程同步**

进程同步：要让各并发进程按要求有序推进。
即使原先异步并发的进程也能相互配合、有序推进。

1. 分析什么地方需要实现“同步关系”，即必须保证“一前一后”执行的两个操作（或两句代码）
2. 设置同步信号量S，初始值为0
3. 在“前操作”之后执行V(S)
4. 在“后操作”之前执行P(S)

   
```c

semaphore S = 0; //初始化同步信号量，初始值为0

//这里假如执行code4之前必须执行code1和code2，
//那么需要在code2之后执行V(S),在code4之前执行P(S)
p1(){
    code1;
    code2;
    V(S);
    code3;
}

//若先执行P(S),S变为-1，那么p2会进行自我阻塞。
//当转换到p1进程,会执行V(S)，再次唤醒p2进程
p2(){
    P(S);
    code4;
    code5;
    code6;
}
```

**用信号量实现进程前驱**

![](D:\class\操作系统\img\1 (1).png)

和同步关系类似，只不过是多重同步。

每一对前驱关系都是一个进程同步问题（需要保证一前一后的操作）
因此，

1. 要为每一对前驱关系各设置一个同步变量
2. 在“前操作”之后对相应的同步变量执行V(S)
3. 在“后操作”之前对相应的同步变量执行P(S)

**前v后p**

![](./img/1 (2).png)



信号量实现对一类系统资源的申请和释放：初始值为资源的数量、本质上也属于同步问题，若无空闲资源，则申请资源的进程需要等待别的进程资源释放后才能继续往下执行



**生产者、消费者问题**



![image-20211010133523103](C:\Users\eddie\AppData\Roaming\Typora\typora-user-images\image-20211010133523103.png)





需要三对信号量

```c
semaphore mutex = 1;	//互斥信号量，实现对缓冲区的互斥访问
semaphore empty = n;	//同步信号量，表示空闲缓冲区的数量
semaphore full = 0;		//同步信号量，表示产品的数量，也即非空缓冲区的数量
```



![image-20211010134248257](C:\Users\eddie\AppData\Roaming\Typora\typora-user-images\image-20211010134248257.png)

实现互斥的p操作一定要在实现同步的p操作之后

v操作不会导致进程阻塞，因此两个v操作顺序可以交换



**读者--写者问题**

![image-20211010135801569](C:\Users\eddie\AppData\Roaming\Typora\typora-user-images\image-20211010135801569.png)





读者优先的情况

![image-20211010140331067](C:\Users\eddie\AppData\Roaming\Typora\typora-user-images\image-20211010140331067.png)



读写公平法：

![image-20211010153855200](C:\Users\eddie\AppData\Roaming\Typora\typora-user-images\image-20211010153855200.png)
