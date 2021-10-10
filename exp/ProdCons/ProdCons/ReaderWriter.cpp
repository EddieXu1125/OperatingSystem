//����д�����⣺��������
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include<iostream>
using namespace std;
DWORD dwID;

#define C(S) CreateSemaphore(NULL, 1, 3, (S))
//���ڴ����ź���
#define P(S) WaitForSingleObject((S), INFINITE)
//If dwMilliseconds is INFINITE, the function will return only when the object is signaled.
#define V(S) ReleaseSemaphore((S), 1, NULL)
#define CT(func, args) CreateThread(NULL, 0, (func), (args), 0, &dwID)
//���ڴ����߳�
//������������This pointer represents the starting address of the thread.
//���ĸ�������A pointer to a variable to be passed to the thread.
//�����������0 --- The thread runs immediately after creation.
//A pointer to a variable that receives the thread identifier.
#define PM(num, S) WaitForMultipleObjects((num), (S), true, INFINITE)
//num:The number of object handles in the array pointed to by lpHandles.
//S:An array of object handles. For a list of the object types whose handles can be specified


struct TInfo{
    //ÿһ���̵߳���Ϣ 
    int id;   //�߳�id
    char type;//�߳����ͣ����߻���д��
    double delay; //��д�����Ŀ�ʼʱ�䣺�̴߳������ӳ���Ӧʱ��(��λΪ��)�󷢳��Թ�����Դ�Ķ�д����
    double persist; //��д�����ĳ���ʱ�䡣���̶߳�д����ɹ��󣬿�ʼ�Թ�����Դ�Ķ�д�������ò���������Ӧʱ�����������ͷŹ�����Դ��
};



TInfo threads[100];//���txt�ļ���thread����Ϣ
HANDLE hThread[100];//���ڴ���߳�
HANDLE mutex;//���ڱ�֤��Reader_Count�����Ļ������
HANDLE rw; //����ʵ�ֶ��ļ��Ļ�����ʡ���ʾ��ǰ�Ƿ��н����ڷ��ʹ����ļ�

int c = 0;//��¼�ļ����̸߳���
int Reader_Count = 0;//��¼������Ŀ

//��������ͷ
void Read_TInfo();//��ȡtxt�ı�����Ϣ
void RP();//�����̵߳Ĵ���
DWORD WINAPI Reader(LPVOID lpParam);//�����߳�
DWORD WINAPI Writer(LPVOID lpParam);//д���߳�


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
    PM(c, hThread); //�ȴ������߳̽���
}

//WINAPI�Ǻ���������ʽ��windows API��������__stdcall��׼����Լ��
//���ɱ����ú���������ջ�еĲ��������ַ�ʽ�ǲ���ʵ�ֿɱ�����ġ� 
DWORD WINAPI Reader(LPVOID lpParam){
    TInfo* arg = (TInfo*)lpParam;
    Sleep(arg->delay * 1000);
    P(mutex);
    if (Reader_Count == 0) {
        P(rw);
    }
    Reader_Count++;
    V(mutex);
    printf("�߳� %d ���ڶ�!\n", arg->id);
    Sleep(arg->persist * 1000);
    printf("�߳� %d ������!\n", arg->id);
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
    printf("�߳� %d ����д!\n", arg->id);
    Sleep(arg->persist * 1000);
    printf("�߳� %d д����!\n", arg->id);
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
