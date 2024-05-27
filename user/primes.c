#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define WRITEEND 1
#define READEND 0

void primes(int rd){
    int n;
    // int prime;
    if(read(rd,&n,sizeof(int))){
        printf("prime %d\n",n);
        // prime=n;
    }
    else{
        exit(0);
    }

    int p[2];
    pipe(p);

    if(fork()==0){
        close(p[WRITEEND]);
        primes(p[READEND]);
        close(p[READEND]);
    }
    else{
        close(p[READEND]);
        int i;
        while(read(rd,&i,sizeof(int))){
            if(i%n)
            write(p[WRITEEND],&i,sizeof(int));
        }
        close(p[WRITEEND]);
        wait(0);
        exit(0);
    }

}

int main(int argc,char *argv[]){
    int p[2];
    pipe(p);

    if(fork()==0){//child
        close(p[WRITEEND]);
        primes(p[READEND]);
        close(p[READEND]);
    }
    else{         //parent
        close(p[READEND]);
        for(int i=2;i<=35;i++){
            write(p[WRITEEND],&i,sizeof(int));
        }
        close(p[WRITEEND]);
        wait(0);
        exit(0);
    }

    exit(0);
}

//git try