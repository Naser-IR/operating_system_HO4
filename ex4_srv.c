//Naser Rabah
//207811282
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <string.h>


char* merge(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char* str = (char*) malloc(len1 + len2 +1);
    int i = 0;
    for (; i < len1; ++i) {
        str[i] = *str1 ++;
    }
    for (; i < len2 + len1; ++i) {
        str[i] = *str2 ++;
    }
    str[i] = '\0';
    return str;
}

void alarm_hand(int sig){
    //new
    printf("The server was closed because no service request was received for the last 60 second\n");
   // while(wait(NULL) != -1);
    exit(0);
}

void signal_hand(int sig) {
    //printf("signal 1 got");
    //signal(SIGUSR1, signal_hand);
    alarm(0);
    int fd=0;
    int fd1=0;
    char a1='\0';
    char user[100]={0x0};
    char num1[4]={0x0};
    char num2[4]={0x0};
    char num3[4]={0x0};
    pid_t pid1 = fork();
    if (pid1 == 0) {
        fd1 = open("to_srv.txt", O_RDONLY);
        while (1){
            int i =0;
            ssize_t x1 = read(fd1, &a1, 1);
            while (a1 != ','){
                user[i] = a1;
                x1 = read(fd1, &a1, 1);
                i++;
            }
            user[i] = '\0';
            i=0;
            x1 = read(fd1, &a1, 1);
            while (a1 != ','){
                num1[i] = a1;
                x1 = read(fd1, &a1, 1);
                i++;
            }
            if(i<=3) {
                num1[i] = '\0';
            }// check the i value befor
            i=0;
            x1 = read(fd1, &a1, 1);
            while (a1 != ','){
                num2[i] = a1;
                x1 = read(fd1, &a1, 1);
                i++;
            }
            if(i<=3) {
                num2[i] = '\0';
            }
            i=0;
            x1 = read(fd1, &a1, 1);
            while (a1 != ',' && x1 !=0){
                num3[i] = a1;
                x1 = read(fd1, &a1, 1);
                i++;
            }
            if(i<=3) {
                num3[i] = '\0';
            }
            if(x1 == 0 ){
                break;
            }
        }
        close(fd1);
        char * name = merge(merge("to_client_",user),".txt");
        fd = open(name, O_CREAT | O_TRUNC | O_WRONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        remove("to_srv.txt");
        int p1 = atoi(num1);
        int p2 = atoi(num2);
        int p3 = atoi(num3);
        int cur_pid = atoi(user);
        if (p2 == 1){
            int z = p1 + p3;
            char tmp[100]={0x0};
            sprintf(tmp,"%d", z);
            write(fd, tmp, strlen(tmp));
        } else if(p2 ==2){
            int z = p1 - p3;
            char tmp[100]={0x0};
            sprintf(tmp,"%d", z);
            write(fd, tmp, strlen(tmp));
        } else if (p2 == 3){
            int z = p1 * p3;
            char tmp[100]={0x0};
            sprintf(tmp,"%d", z);
            write(fd, tmp, strlen(tmp));
        } else if(p2 == 4){
            if(p3 == 0){
                write(fd, "CANNOT_DIVIDE_BY_ZERO", 21);
            }
            else {
                int z = p1 / p3;
                char tmp[100]={0x0};
                sprintf(tmp,"%d", z);
                write(fd, tmp, strlen(tmp));
            }
        }
        close(fd);
        kill(cur_pid, SIGUSR2);
        exit(0);
    }
}

int main() {

    char* name ="to_srv.txt" ;
    if(access(name,F_OK)== 0) {
        remove(name);
    }

    signal(SIGALRM, alarm_hand);
    signal(SIGUSR1,signal_hand);
    alarm(60);
    // wait
    while (1) {
        pause();
        alarm(60);
    }
    
}
