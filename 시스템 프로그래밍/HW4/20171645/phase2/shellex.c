/* $begin shellmain */
#include "csapp.h"
#include<errno.h>
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv, int* argvn);
int builtin_command(char **argv);
void run_pipes(char **argv, int pipe_idx[MAXARGS], int pipe_count);

int main() 
{
    char cmdline[MAXLINE]; /* Command line */
    
    while (1) {
        /* Read */
        printf("CSE4100-SP-P4> ");
        fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);
        
        /* Evaluate */
        eval(cmdline);
    }
}
/* $end shellmain */

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    int builtin;         /* check commands */
    int argvn;           /* argument number */
    pid_t pid;           /* Process id */
    /* pipe variables */
    int pipe_flag = 0;
    int pipe_count = 0;
    int len = 0;
    int pipe_idx[MAXARGS];
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv, &argvn);
    
    /* set index of '|' to pipe_idx */
    /* and make '|' argv to NULL to run piping */
    for(int i=0; argv[i] != NULL; i++, len++)
    {
        if (!strcmp(argv[i],"|"))
        {
            argv[i] = NULL;
            pipe_idx[++pipe_count] = i+1;
            pipe_flag = 1;
        }
    }
    
    if (argv[0] == NULL)
        return;   /* Ignore empty lines */
    
    builtin = builtin_command(argv);
    
    if (builtin) {
        return;
    }
    else if (builtin == 2) // cd
        chdir(argv[1]);
    else if ((pid = fork()) == 0) { //child process
        if (pipe_flag) { //piping
            run_pipes(argv, pipe_idx, pipe_count);
            exit(0);
        }
        else { //single command
            if (execvp(argv[0], argv) < 0) {
                   printf("%s: Command Not Found\n", argv[0]);
                   exit(0);
               }
        }
    }
    
    if (!bg) {
        int status;
        if (waitpid(pid, &status, 0) < 0)
            unix_error("waitfg : waitpid error");
    }
    else
        printf("%d %s", pid, cmdline);
    
    
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "exit"))
        exit(0);
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
        return 1;
    if (!strcmp(argv[0], "cd"))   /* 내장명령어 cd */
        return 2;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv, int *argvn)
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */
    
    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;
    
    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
        return 1;
    
    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;
    
    *argvn = argc;
    return bg;
}
/* $end parseline */
/* start run_pipes */
void run_pipes(char **argv, int pipe_idx[MAXARGS], int pipe_count)
{
    int pipes[MAXARGS][2]; /* pipe list */
    int pid;
    int status;
    
    /* run 1st command */
    pipe(pipes[0]);
    if ((pid = fork()) < 0) {
        printf("pid %d : fork() error\n", pid);
        exit(1);
    }
    else if (pid == 0) {
        close(STDOUT_FILENO);
        dup2(pipes[0][1], STDOUT_FILENO);
        close(pipes[0][1]);
        if (execvp(argv[pipe_idx[0]], &argv[pipe_idx[0]]) < 0) {
            printf("%s: Command Not Found\n", argv[pipe_idx[0]]);
            exit(0);
        }
    }
    close(pipes[0][1]);
    if (waitpid(pid, &status, 0) < 0)
        unix_error("waitfg : waitpid error");
    
    /* run 2nd ~ command except final command */
    for(int i=0; i<pipe_count-1; i++) {
        pipe(pipes[i+1]);
        if ((pid = fork()) < 0) {
            printf("pid %d : fork() error\n", pid);
            exit(1);
        }
        else if (pid == 0) {
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            dup2(pipes[i][0], STDIN_FILENO);
            dup2(pipes[i+1][1], STDOUT_FILENO);
            close(pipes[i][0]);
            close(pipes[i+1][1]);
            if (execvp(argv[pipe_idx[i+1]], &argv[pipe_idx[i+1]]) < 0) {
                printf("%s: Command Not Found\n", argv[pipe_idx[i+1]]);
                exit(0);
            }
        }
        close(pipes[i+1][1]);
        if (waitpid(pid, &status, 0) < 0)
            unix_error("waitfg : waitpid error");
    }
    
    /* final command */
    if ((pid = fork()) < 0) {
        printf("pid %d : fork() error\n", pid);
        exit(1);
    }
    else if (pid == 0) {
        close(STDIN_FILENO);
        dup2(pipes[pipe_count-1][0], STDIN_FILENO);
        close(pipes[pipe_count-1][0]);
        close(pipes[pipe_count-1][1]);
        if (execvp(argv[pipe_idx[pipe_count]], &argv[pipe_idx[pipe_count]]) < 0) {
            printf("%s: Command Not Found\n", argv[pipe_count]);
            exit(0);
        }
    }
    if (waitpid(pid, &status, 0) < 0)
        unix_error("waitfg : waitpid error");
    return;
}
/* $end run_pipes */

