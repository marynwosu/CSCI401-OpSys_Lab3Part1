//  pipes_processes3.c
//  Created by Mary Nwosu and Melanie White on 11/23/22.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
**/

int main(int argc, char **argv) {
  int pipefd1[2], pipefd2[2];
  int pid1, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL, NULL};
  
  // check to see if # of args is 2, if not exit
  if (argc != 2) {
    printf("Usage: ./pipes_processes3 <filter>\n");
    exit(0);
  }

  // make a pipe (fds go in pipefd[0] and pipefd[1])
  pipe(pipefd1);    // pipe for cat to grep
  pipe(pipefd2);    // pipe for grep and sort

  pid1 = fork();
  if (pid1 == 0) {
    pid2 = fork();
    if (pid2 == 0) {
      // child gets here and handles "grep Villanova"
      // replace standard input with input part of pipe
      dup2(pipefd1[0], 0);
      dup2(pipefd2[1], 1);

      // close unused halves of pipes
      close(pipefd1[1]);
      close(pipefd2[0]);

      // execute grep
      grep_args[1] = argv[1];  // set grep_args to the argv parameter
      execvp("grep", grep_args);
    } else {
      // parent gets here and handles "cat scores"
      // replace standard output with output part of pipe
      dup2(pipefd1[1], 1);

      // close unused halves of pipes
      close(pipefd1[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      // execute cat
      execvp("cat", cat_args);
    }
  } else {
    dup2(pipefd2[0], 0);

    // close unused halves of pipes
    close(pipefd2[1]);
    close(pipefd1[0]);
    close(pipefd1[1]);

    // execute sort
    execvp("sort", sort_args);
  }
}
