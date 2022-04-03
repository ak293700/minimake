#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    int chld = fork();
    if (chld == 0)
        execlp("/bin/sh", "supershell", "-c", argv[1], (char *)NULL);

    int status;
    waitpid(chld, &status, 0);
    printf("process exit status: %d\n", WEXITSTATUS(status));

    return 0;
}
