#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc,char *argv[]){

    if(argc<2){
        printf("Useage: xargs command");
        exit(1);
    }

    int xargc=argc-1;
    char *xargv[MAXARG];
    for(int i = 0; i < xargc; i++) {
        xargv[i]=malloc(sizeof(argv[i + 1])+1);
        strcpy(xargv[i],argv[i + 1]);
    }

    while(1) {

        //int result_read;
        char buf[100],*p;
        int index=xargc;
        p=buf;
        while(read(0,p,1)&& *p!='\n'){
            if(*p==' '){
                *p='\0';
                xargv[index]=malloc(strlen(buf)+1);
                strcpy(xargv[index],buf);
                index++;
                // strcpy(buf,"\0");
                p=buf;
            }
            else  p++;
        }

        if(*p!='\n'/*&& p==buf*/){
            for(int i=0;i<index;i++){
                free(xargv[i]);
            }
            break;
        }

        *p = '\0'; // 终止最后的字符串
        if (p != buf) {
            xargv[index] = malloc(strlen(buf) + 1);
            strcpy(xargv[index], buf);
            index++;
        }

        xargv[index] = 0;
        if(fork()==0){
            exec(xargv[0],xargv);
            exit(0);
        }
        else{
            wait(0);
        }

        for(int i=xargc;i<index;i++){
            free(xargv[i]);
        }
    }

    for (int i = 0; i < xargc; i++) {
        free(xargv[i]);
    }

    exit(0);
}

//sh < xargstest.sh