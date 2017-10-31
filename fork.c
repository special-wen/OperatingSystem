/*************************************************************************
	> File Name: fork.c
	> Author: zxw
	> Mail: 
	> Created Time: 2017年09月25日 星期一 16时08分41秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(void){
    pid_t pid;

    printf("Process Creation Study\n");
    pid = fork();
    switch(pid){
        case 0:
            printf("Child pricess is running,CurPid is %d,ParentPid is %d\n",pid,getppid());
        break;
        case -1:
            perror("Process creation failde\n");
            break;
        default:
            printf("Parent process is running,ChildPid is %d,ParentPid is %d\n",pid,getpid());
            break;
    }
    exit(0);
}
