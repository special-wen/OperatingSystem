/*************************************************************************
	> File Name: fork2.c
	> Author: zxw
	> Mail: 
	> Created Time: 2017年09月26日 星期二 17时59分53秒
 ************************************************************************/

#include<stdio.h>

#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(void){
    pid_t pid;
    char * msg;
    int k;

    printf("Process Creation Study\n");
    pid = fork();
    switch(pid){
        case 0:
            msg = "Child process is running";
            k = 3;
            break;
        case -1:
            perror("Process Creation failed\n");
            break;
        default:
            msg = "Parent process is running";
            k = 5;
            break;
    }
    while(k>0){
        puts(msg);
        sleep(1);
        k--;
    }
    exit(0);
}
