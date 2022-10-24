#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
	int pipefd1[2];
  int pid;
	int pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
	pipe(pipefd1);

  pid = fork();

  if (pid == 0)
    {
			dup2(pipefd1[0], 0);
			close(pipefd1[1]);

			pid2 = fork();

			if (pid2 == 0){
				
				dup2(pipefd1[0], 0);
        close(pipefd1[1]);

				char* sort_arg[] = {"sort", NULL};
				execvp("sort", sort_arg);
			}
			else{
				// child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      dup2(pipefd1[1], 1);

      // close unused hald of pipe
			close(pipefd1[0]);

      // execute grep

      execvp("grep", argv);
			}
      
    }
  else
    {
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute cat

      execvp("cat", cat_args);
    }
}