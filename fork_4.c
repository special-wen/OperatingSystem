/*************************************************************************
	> File Name: fork_4.c
	> Author: zxw
	> Mail: 
	> Created Time: 2017年10月10日 星期二 14时07分54秒
 ************************************************************************/
/* POSIX下进程控制的实验程序 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<ctype.h>
/* 允许建立的子进程个数最大值 */
#define MAX 10
/* 子进程睡眠时间 */
#define SLEEP 2
int proc_num = 0;
void do_something();

int main(int argc,char* argv[]){

    //子进程个数
    int child_proc_num = MAX;
    int i,ch;
    int status;
    pid_t temp;
    pid_t child_pid;
    pid_t pid[10]={0}; //存放每个子进程的id

    if(argc >1){ //命令行参数第一个参数表示的子进程个数
        child_proc_num = atoi(argv[1]);
        child_proc_num = (child_proc_num)>10 ? 10:child_proc_num;
    }
    for(i = 0;i<child_proc_num;i++){
        temp = fork();
        switch(temp){
            case -1:
                printf("fork failed!\n");
            exit(-1);
            case 0:
                proc_num = i;
                do_something();
                break;
            default:
                pid[i] = temp;
                break;
        }
    }
	//让用户选择需要杀死的进程，数字表示杀死该进程，q退出
    while((ch = getchar() != 'q')){
        if(isdigit(ch)){
            kill(pid[ch-'0'],SIGTERM);
        }
    }
    kill(0,SIGTERM);
     return 0;
}

void do_something(){
    for(;;){
        printf("This is process No .%d and its pid is %d\n",proc_num,getpid());
        sleep(SLEEP); //主动阻塞两秒钟
    }
}
