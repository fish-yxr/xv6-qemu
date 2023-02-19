#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if(argc > 1) {
        printf("Usage : pingpong");
        exit(1);
    }
    //pipe
    char buf1[1], buf2[1]; char sig[1];
    *buf1 = 0; *buf2 = 0; *sig = 1;
    int ptoc[2], ctop[2], pipe_ptoc = -1, pipe_ctop = -1;
    pipe_ptoc = pipe(ptoc);
    pipe_ctop = pipe(ctop);
    if(pipe_ptoc == -1 || pipe_ctop == -1) {
        printf("Err: fail to create pipe\n");
        exit(1);
    }
    //fork
    int pid = fork();
    if(pid < 0) {
        printf("Err: fail to fork\n");
        exit(1);
    }
    else if(pid == 0) {
        //child
        close(ptoc[1]);
        close(ctop[0]);
        read(ptoc[0],buf1,1);
        if(*buf1) {
            printf("%d: received ping\n",getpid());
            write(ctop[1],sig,1);
        }
        close(ptoc[0]);
        close(ctop[1]);
    }
    else {
        //parent
        close(ptoc[0]);
        close(ctop[1]);
        write(ptoc[1],sig,1);
        read(ctop[0],buf2,1);
        if(*buf2) {
           printf("%d: received pong\n",getpid()); 
        }
        close(ptoc[1]);
        close(ctop[0]);
    }
    exit(0);
}