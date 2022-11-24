//  pipes_processes1.c
//  Created by Mary Nwosu and Melanie White on 11/19/22.
//  C program to demonstrate use of fork() and pipe()

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
  
int main() {
    // We use two pipes
    // First pipe to send input string from parent
    // Second pipe to send concatenated string from child
  
    int fd1[2];  // Used to store two ends of first pipe
    int fd2[2];  // Used to store two ends of second pipe
  
    char input_str[100], input_str2[100], concat_str[100], concat_str2[200];
    pid_t p;
  
    if (pipe(fd1)==-1) {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd2)==-1) {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str);
    input_str[101] = '\0';  //input null terminator;
    p = fork();
  
    if (p < 0) {
        fprintf(stderr, "fork Failed" );
        return 1;
    }
  
    // Parent process
    else if (p > 0) {
        close(fd1[0]);  // Close reading end of Parent pipe
        write(fd1[1], input_str, strlen(input_str)+1);  // Write input string
        
        wait(NULL);    // Wait for child to send a string
      
        read(fd2[0], concat_str2, 100);
  
        printf("%s\n", concat_str2);
  
        close(fd2[1]); // Close writing end of pipes
        close(fd1[1]);
    }
  
    // child process
    else {
        close(fd1[1]);  // Close writing end of first pipe
        close(fd2[0]);
      
        read(fd1[0], concat_str, 100);  // Read a string using first pipe
        strcat(concat_str, "howard.edu");
        printf("%s\n", concat_str);

        printf("Enter a 2nd string to concatenate:");
        scanf("%s", input_str2);
      
        strcat(concat_str, input_str2);

        write(fd2[1], concat_str, strlen(concat_str)+1);

        // Close both reading ends
        close(fd1[0]);
        close(fd2[0]);

        exit(0);
    }
}
