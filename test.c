

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid = getpid();
    dprintf(2, "Printf from test.c %d\n", pid);
    kill(pid, SIGSTOP);
	sleep(20);
    dprintf(2, "TEST\n");
    kill(pid, SIGSTOP);
    dprintf(2, "TEST2\n");
    return (0);
}