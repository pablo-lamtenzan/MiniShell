

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid = getpid();
    dprintf(2, "%d\n", pid);
    kill(pid, SIGSTOP);
    dprintf(2, "TEST\n");
    return (0);
}