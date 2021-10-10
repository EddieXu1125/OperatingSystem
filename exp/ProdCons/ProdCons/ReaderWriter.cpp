//读者写者问题：读者优先
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include<iostream>
using namespace std;
DWORD dwID;

#define C(S) CreateSemaphore(NULL, 1, 3, (S))
//用于创建信号量
#define P(S) WaitForSingleObject((S), INFINITE)
//If dwMilliseconds is INFINITE, the function will return only when the object is signaled.
#define V(S) ReleaseSemaphore((S), 1, NULL)
#define CT(func, args) CreateThread(NULL, 0, (func), (args), 0, &dwID)
//用于创建线程
//第三个变量：This pointer represents the starting address of the thread.
//第四个变量：A pointer to a variable to be passed to the thread.
//第五个变量：0 --- The thread runs immediately after creation.
//A pointer to a variable that receives the thread identifier.
#define PM(num, S) WaitForMultipleObjects((num), (S), true, INFINITE)
//num:The number of object handles in the array pointed to by lpHandles.
//S:An array of object handles. For a list of the object types whose handles can be specified


struct TInfo{
    //每一个线程的信息 
    int id;   //线程id
    char type;//线程类型，读者还是写者
    double delay; //读写操作的开始时间：线程创建后，延迟相应时间(单位为秒)后发出对共享资源的读写申请
    double persist; //读写操作的持续时间。当线程读写申请成功后，开始对共享资源的读写操作，该操作持续相应时间后结束，并释放共享资源。
};



TInfo threads[100];//存放txt文件中thread的信息
HANDLE hThread[100];//用于存放线程
HANDLE mutex;//用于保证对Reader_Count变量的互斥访问
HANDLE rw; //用于实现对文件的互斥访问。表示当前是否有进程在访问共享文件

int c = 0;//记录文件中线程个数
int Reader_Count = 0;//记录读者数目

//函数声明头
void Read_TInfo();//读取txt文本的信息
void RP();//进行线程的创建
DWORD WINAPI Reader(LPVOID lpParam);//读者线程
DWORD WINAPI Writer(LPVOID lpParam);//写者线程


///////////////////////////////////


void Read_TInfo(){
    FILE* fp;
    fp = fopen("in.txt", "r");
    while (fscanf(fp, "%d %c %lf %lf", &threads[c].id, &threads[c].type, &threads[c].delay, &threads[c].persist) != EOF) {
        c++;
    }
    fclose(fp);
    cout << "Finish reading file" << endl;
}

void RP(){
    mutex = C("mutex");
    rw = C("rw");
    for (int i = 0; i < c; ++i) {
        if (threads[i].type == 'W') {
            hThread[i] = CT(Writer, &threads[i]);
        }
        else {
            hThread[i] = CT(Reader, &threads[i]);
        }
    }
    PM(c, hThread); //等待所有线程结束
}

//WINAPI是函数调用形式，windows API函数采用__stdcall标准调用约定
//即由被调用函数来清理栈中的参数，这种方式是不能实现可变参数的。 
DWORD WINAPI Reader(LPVOID lpParam){
    TInfo* arg = (TInfo*)lpParam;
    Sleep(arg->delay * 1000);
    P(mutex);
    if (Reader_Count == 0) {
        P(rw);
    }
    Reader_Count++;
    V(mutex);
    printf("线程 %d 正在读!\n", arg->id);
    Sleep(arg->persist * 1000);
    printf("线程 %d 读完了!\n", arg->id);
    cout << "------------------------" << endl;
    P(mutex);
    Reader_Count--;
    if (Reader_Count == 0) {
        V(rw);
    }
    V(mutex);
    return 0;                                                                            
}

DWORD WINAPI Writer(LPVOID lpParam)
{
    TInfo* arg = (TInfo*)lpParam;
    Sleep(arg->delay * 1000);
    P(rw);
    printf("线程 %d 正在写!\n", arg->id);
    Sleep(arg->persist * 1000);
    printf("线程 %d 写完了!\n", arg->id);
    cout << "------------------------" << endl;
    V(rw);
    return 0;
}


/// ////////////////////////////////////////

int main() {
    Read_TInfo();
    RP();
    return 0;
}
