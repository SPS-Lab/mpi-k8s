#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t done = 0;

void term(int signum)
{
   printf("Caught SIGTERM! \n");
   printf("Performing checking point for 5 seconds... \n");
   sleep(5);
   done = 1;
}

int main(int argc, char *argv[])
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    int pid = getpid();
    int parentID = getppid();
    printf("\nPID is %d  and Parent is %d \n",pid, parentID);

    int loop = 0;
    while (!done)
    {
      printf("Start loop run %d.\n", loop);
      printf("\tPID is %d  and Parent is %d \n",pid, parentID);
      int t = sleep(10);
      /* sleep returns the number of seconds left if
       * interrupted */
      while (t > 0){
	printf("\tLoop run was interrupted with %d sec to go, finishing...\n", t);
	t = sleep(t);
      }
      printf("Finished loop run %d.\n", loop++);
    }

    printf("done.\n");
    return 0;
}
