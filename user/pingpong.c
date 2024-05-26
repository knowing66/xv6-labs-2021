#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int pp2c[2];
  int pc2p[2];
  char buf[1];
  pipe(pp2c);
  pipe(pc2p);

  int pid=fork();

  if(pid<0){
    exit(1);
  }
  else if(pid==0){
    close(pp2c[1]);
    close(pc2p[0]);
    read(pp2c[0],buf,1);
    printf("%d: received ping\n",getpid());
    write(pc2p[1],buf,1);
    close(pp2c[0]);
    close(pc2p[1]);
    exit(0);
  }
  else{
    close(pp2c[0]);
    close(pc2p[1]);
    write(pp2c[1],"A",8);
    read(pc2p[0],buf,1);
    printf("%d: received pong\n",getpid());
    close(pp2c[1]);
    close(pc2p[0]);
    exit(0);
  }
}
