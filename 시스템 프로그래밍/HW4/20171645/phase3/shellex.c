/* $begin shellmain */
#include "csapp.h"
#include <errno.h>
#include <ctype.h>
#define MAXARGS   128

/* data structure */
struct process {
    pid_t pid;
    char command[MAXARGS];
    int flag;
} process_list[5000];

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv, int* argvn);
int builtin_command(char **argv);
void run_pipes(char **argv, int pipe_idx[MAXARGS], int pipe_count);
void sigchild_handler(int signal);
void sigquit_handler(int signal);
void sigtstp_handler(int signal);
int getpid_f(int index);
void jobs();
void print_shell();
int find_percent(char *str);

/* global variables */
int fg; /* check whether currently fg running or not */
int plen; /* length of process */
sigset_t mask, prev;

int main() 
{
    char cmdline[MAXLINE]; /* Command line */
    fg = 0;
    plen = 0;
    
    //set signal handler
    signal(SIGCHLD,sigchild_handler);
    signal(SIGTSTP,sigtstp_handler);
    signal(SIGCONT,sigquit_handler);
    
    // cltr+c cltr+\ block
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigaddset(&mask,SIGQUIT);
    
    while (1) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        /* Read */
        print_shell();
        fgets(cmdline, MAXLINE, stdin);
        
        if (feof(stdin))
            exit(0);
        
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
    int flag = 0; /* running flag */
    int status;
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv, &argvn); // bg == 1 if background
    flag = bg; // flag == bg
    
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
    if (builtin == 3) // jobs
        jobs();
    if (builtin > 0 && argv[1] == NULL) return;
    if (builtin == 1) {
        return;
    }
    else if (builtin == 2) // cd
        chdir(argv[1]);
    else if (builtin == 4) { // fg
        fg = 0;
        int i;
        int status, ppid;
        
        if (find_percent(argv[1])) {
            int tpid = atoi(argv[1]+1);
            ppid = getpid_f(tpid);
        }
        else ppid = atoi(argv[1]); //get pid

        for(i=0; i<plen; i++)
            if (ppid == process_list[i].pid) {
                process_list[i].flag = 0;
                break;
            }
        kill(ppid, SIGCONT); //send signal to continue
        printf("%s : pid %d is runin foreground\n",process_list[i].command, ppid);
        waitpid(ppid,&status,WUNTRACED); //wait to terminate or stop
        if (WIFSTOPPED(status)) {
            process_list[i].flag = 1;
            fg = 1;
        }
        if (WIFEXITED(status))
        {
            process_list[i].flag = 0;
            fg = 1;
        }
    }
    else if (builtin == 5) { //bg command
        fg = 0;
        int i;
        int status, ppid;
        if (find_percent(argv[1])) {
            int tpid = atoi(argv[1]+1);
            ppid = getpid_f(tpid);
        }
        else ppid = atoi(argv[1]); // get pid

        for(i=0; i<plen; i++)
            if (ppid == process_list[i].pid) {
                process_list[i].flag = 1;
                break;
            }
        kill(ppid, SIGCONT); // send signal to continue
        printf("%s : pid %d is runin background\n",process_list[i].command, ppid);
    }
    else if (builtin == 6) { //kill command
        int ppid = -1;
        if (find_percent(argv[1])) {
            int tpid = atoi(argv[1]+1);
            ppid = getpid_f(tpid);
        }
        else ppid = atoi(argv[1]);
        
        if (ppid != -1)
            kill(ppid,9); // 강제종료
        else printf("Invalid Input\n");
        sleep(1); // ???
    }
    else { //system command
        sigprocmask(SIG_UNBLOCK, &mask, &prev);
        pid = fork();
        if (pid<0) {
            perror("Fork Error\n");
            exit(-1);
        }
        else if (pid==0) { // child process
            if (pipe_flag) {
                run_pipes(argv,pipe_idx, pipe_count); // run pipeline
                exit(0);
            }
            else {
                if (execvp(argv[0], argv) < 0) {
                    printf("%s: Command Not Found\n", argv[0]);
                    exit(0);
                }
            }
        }
        else { // parent process
            //add process list
            process_list[plen].pid = pid;
            process_list[plen].flag = 0;
            plen++;
            if (flag == 0) { // not background
                fg = 1;
                strncpy(process_list[plen-1].command, cmdline, strlen(cmdline)-1);
                waitpid(pid,&status,WUNTRACED); //wait for children process to terminate or stop
                if (WIFSTOPPED(status)) {
                    process_list[plen-1].flag = 1;
                }
                fg = 0;
            }
            else { // background
                strncpy(process_list[plen-1].command, cmdline, strlen(cmdline)-1);
                process_list[plen-1].flag = 1;
                printf("%d %s\n",process_list[plen-1].pid, process_list[plen-1].command);
            }
        }
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "exit")) /* 내장명령어 exit */
        exit(0);
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
        return 1;
    if (!strcmp(argv[0], "cd"))   /* 내장명령어 cd */
        return 2;
    if (!strcmp(argv[0], "jobs"))   /* 내장명령어 jobs */
        return 3;
    if (!strcmp(argv[0], "fg"))   /* 내장명령어 fg */
        return 4;
    if (!strcmp(argv[0], "bg"))   /* 내장명령어 bg */
        return 5;
    if (!strcmp(argv[0], "kill"))   /* 내장명령어 kill */
        return 6;
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

/* SIGCHLD handler */
void sigchild_handler(int signal)
{
    int status;
    int pid;
    int idx;
    // wait for signal
    pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
    for(idx = 0; idx < plen; idx++)
    {
        if (process_list[idx].pid == pid)
        {
            process_list[idx].flag = 0; // terminated
            if (WIFCONTINUED(status)) {
                process_list[idx].flag = 1; // resume process
            }
            break;
        }
    }
    return;
}

/* signal handler*/
void sigquit_handler(int signal)
{
    return;
}
void sigtstp_handler(int signal)
{
    return;
}

/* get pid by index of unterminated process */
int getpid_f(int index)
{
    int count = 0;
    for(int i=0; i<plen; i++) {
        if (process_list[i].flag == 1) count++;
        if (count == index) return process_list[i].pid;
    }
    return -1;
}

/* print unterminated process */
void jobs()
{
    int count=1;
    for(int i=0; i<plen; i++)
        if (process_list[i].flag == 1) {
            printf("[%d]\t%s\t%d\n",count++,process_list[i].command,process_list[i].pid);
        }
    return;
}

/* print shell prompt */
void print_shell()
{
    printf("CSE4100-SP-P4> ");
}

/* in string, if there is '%' return 1. else return 0 */
int find_percent(char *str)
{
    for(int i=0; i<strlen(str); i++)
        if (str[i] == '%') return 1;
    return 0;
}
