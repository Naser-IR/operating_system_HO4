//Naser Rabah
//207811282
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <libgen.h>
#include <signal.h>

char mypid[100]={0x0};

// now i need just to set a time out for the client side
// we need also to deal with the zombies in the server
// and check all the open files thing

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
    printf("Client closed because no response was recived from the server for 30 second\n");
    exit(0);
}


void signal_hand(int sig) {
    //exit(0);
    char a1='\0';
    char * name = merge(merge("to_client_",mypid),".txt");
    int fd1=0;
    fd1 = open(name, O_RDONLY);
    while (1){
        int i =0;
        ssize_t x1 = read(fd1, &a1, 1);
        if(x1 == 0 ){
            printf("\n");
            break;
        }
        if(a1 != ' ') {
        printf("%c",a1);
        }

    }
    close(fd1);
    remove(name);
}

int main(int argc, char**argv ) {
    if (argc !=5 ) {
        printf("ERROR_FROM_EX4\n");
        return 0;
    }
    if(atoi(argv[3])>4||atoi(argv[3])<1){
        printf("ERROR_FROM_EX4\n");
        return 0;
    }
    int fd=0;
    char* name ="to_srv.txt" ;
    int n = 0;
    int w =0;
    for (; n < 10; ++n) {
        fd = open(name, O_CREAT |O_EXCL | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |S_IWOTH);
        if(fd == -1){
            int x = rand(); // need to switch to getrandom
            x= (x%5) + 1;
            sleep(x);
            w++;
        } else {
            break;
        }

    }
    if( w==10 ){
        printf("ERROR_FROM_EX4\n");
        return 0;
    }


    int pi1 = atoi(argv[1]);
    char * pi2 = argv[2];
    char * pi3 = argv[3];
    char * pi4 = argv[4];
    int pid = getpid();
    sprintf(mypid, "%d", pid);


    write(fd,mypid, strlen(mypid));
    write(fd,",", 1);
    write(fd,pi2, strlen(pi2));
    write(fd,",", 1);
    write(fd,pi3, strlen(pi3));
    write(fd,",", 1);
    write(fd,pi4, strlen(pi4));

    close(fd);
    signal(SIGALRM, alarm_hand);
    alarm(30);
    kill(pi1, SIGUSR1);
    signal(SIGUSR2,signal_hand);
    pause();
    return 0;
}
