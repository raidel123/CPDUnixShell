//
//  main.c
//  cop5570_proj1
//
//  Created by Raidel Hernandez on 10/5/17.
//  Copyright © 2017 Raidel Hernandez. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "p_globals.h"
 
// functions prototypes
SYSCALL get_syscall(char *buff);
int process_syscall(char ** buff_split);     // execute command entered by the user
int make_syscall(char** buff_split);
char** split_buff(char *buff, char* DELIM);
void run_shell(void);

void ioredirect(char** args, char* ifile, char* ofile);
void iredirect(char** args, char* ifile);
void oredirect(char** args, char* ofile);
char** ior_split(char** args, int delim);
char** pipe_split(char** args, int i_delim, int f_delim);
void pipe_it_up(char *** args);

int p_exit(char ** args);
int p_cd(char ** args);
int p_pwd(char ** args);
int p_set(char ** args);
int p_extern(char ** args);
int p_myls(char** args);
int p_background(char ** args);
int p_pipe(char ** args);
int p_redirect(char ** args);

int (*builtin_call[]) (char **) = {
    &p_exit,
    &p_cd,
    &p_pwd,
    &p_set
};

int main(int argc, const char * argv[]) {
    
    if(argc == 1)
        run_shell();        // run atomic loop to run program
    //else    // remove after testing fileio redirection
        //printf("ran the command program with the piped file\n");fflush(0);
    return 1;
}

// display the shell and read characters
void run_shell(void){
    
    char buff[512];
    char history[1000][512];
    int curr_history = 0;
    int curr_pos;
    
    int curr_cmd = 0;
    
    char ch;
    char** buff_split;
    
    curr_pos = 0;
    
    printf("<myshell:%d> ", curr_cmd); fflush(0);
    while(1){
        ch = getchar();
        
        if (ch == '\n') {
            //printf("Entered pressed\n");fflush(0);
            buff[curr_pos ++] = '\0';
			//printf("buf = %s\n", buff);fflush(0);
            
            
            buff_split = split_buff(buff, " ");
            process_syscall(buff_split);
            
            strcpy(history[curr_history++], buff);
            curr_cmd++;
            
            printf("<myshell:%d> ", curr_cmd); fflush(0);
            curr_pos = 0;
        
        } else if (ch == EOF) {         // ctrl-D
            printf("\n");
            p_exit(buff_split);
            //process_syscall(EXIT);
        
        } else {                      // just write some character to the buff
            buff[curr_pos ++] = ch;
        }
        //fflush(stdin);
    }
}

// functions definitons
int process_syscall(char ** buff_split){
    
    SYSCALL cmd = IGNORE;
    int special_call = 0;
    
	//printf("start process_syscall\n"); fflush(0);
    int i = 0;
	//printf("val buff_split[0]: %s", buff_split[0]);fflush(0);
    while(buff_split[i] != NULL){
        //printf("process_syscall: buff_split[%d]: %s\n", i, buff_split[i]);fflush(0);
        
        if(strcmp(buff_split[i], P_IREDIRECT) == 0 || strcmp(buff_split[i], P_OREDIRECT) == 0){
            cmd = REDIRECT;
            special_call = 1;
        }else if (strcmp(buff_split[i], P_PIPE) == 0){
            cmd = PIPE;
            special_call = 1;
        }
        
        if(buff_split[i+1] == NULL)
            if(strcmp(buff_split[i], P_BACKGROUND) == 0){
                cmd = BACKGROUND;
                special_call = 1;
            }
        
        i++;
		
		//printf("end process_syscall iteration\n"); fflush(0);
    }
    
    if (special_call == 0)
        cmd = get_syscall(buff_split[0]);
    
    switch (cmd) {
        case EXIT:
        case CD:
        case PWD:
        case SET:
            make_syscall(buff_split);
            break;
        
        case EXTERN:
            p_extern(buff_split);
            break;
        
        case PIPE:
            //printf("pipe\n");fflush(0);
            p_pipe(buff_split);
            break;
            
        case REDIRECT:
            //printf("redirect\n");fflush(0);
            p_redirect(buff_split);
            break;
            
        case IGNORE:
            //printf("ignore\n");fflush(0);
            break;
        
        case MYLS:
            p_myls(buff_split);
            break;
        
        case BACKGROUND:
            p_background(buff_split);
            break;
        default:
            
            break;
    }
    
    //change return
    return 1;
}

int make_syscall(char** buff_split){
    int i;
    for (i = 0; i < sizeof(builtin_cmd); i++)
        if (strcmp(buff_split[0], builtin_cmd[i]) == 0)
            return (*builtin_call[i])(buff_split);
    
    return 1;
}

int p_exit(char ** args){
    //printf("exit success\n");fflush(0);
    printf("\n");
    exit(EXIT_SUCCESS);
}

int p_cd(char ** args){
    
    if (args[1] == NULL)
        fprintf(stderr, "Usage: cd [location], argument required");
    else if (chdir(args[1]) != 0) {
        fprintf(stderr, "Could not change directory\n"); fflush(0);
    }
    
    return 1;
}

int p_pwd(char ** args){
    
    char pwd[512];
    
    if (getcwd(pwd, sizeof(pwd)) == NULL)
        perror("getcwd() error");
    else
        printf("%s\n", pwd);fflush(0);
    
    return 1;
}

int p_set(char ** args){
    
    char ** split_env = split_buff(args[1], "=");
    //printf("env: %s, vars: %s\n", split_env[0], split_env[1]);
    setenv(split_env[0], split_env[1], 1);
    
    return 1;
}

int p_extern(char ** args){
    
    int pid, stat;
    if (fork() == 0) {
        //printf("Making Call to: %s\n", args[0]);fflush(0);
        if (execvp(args[0], args) == -1) exit(-1);
    } else{
        pid = wait(&stat);
        //printf("command executed, status = %d\n", WEXITSTATUS(stat));fflush(0);
    }
    
    return 1;
}

int p_myls(char** args){
    int pid, stat;
    
    char *arr;
    char arr2[1000];
    
    //printf("wt!!!!!");
    if (fork() == 0) {
        //printf("Making Call to: %s\n", args[0]);fflush(0);
        arr = getenv("MYPATH");
        strcpy(arr2, arr);
        strcat(arr2, "/myls");
        args[0] = arr2;
        
        // check if inside mypath [at each path] myls exists using dir
        //printf("get MYPATH: %s,\n totalPath: %s\n", arr, args[0]);fflush(0);
        if (execv(args[0], args) == -1) exit(-1);
        
        exit(0);
    } else{
        pid = wait(&stat);
        //printf("command executed, status = %d\n", WEXITSTATUS(stat));fflush(0);
    }
    
    return 1;
}

int p_background(char ** args){
    
    //printf("Background!!!\n");
    int i = 0;
    while(args[i] != NULL)
        i++;
    
    //printf("Making Call to: %s\n", args[i-1]);fflush(0);
    args[i-1] = NULL;
    
    if (fork() == 0) {
        //printf("Making Call to: %s\n", args[0]);fflush(0);
        setpgid(0,0);
        //printf("pid = %d, pgid = %d\n", getpid(), getpgrp());
        if (execvp(args[0], args) == -1) exit(-1);
        exit(0);
    }
    
    return 1;
}

int p_pipe(char ** args){
    
    char *** pipe_param_list = (char***)malloc(sizeof(char**)*BUFFER_SIZE);
    
    int i = 0, param_num = 0;
    //int fd1;
    int i_delim = 0;
    while(args[i] != NULL){
        if (strcmp(args[i], P_PIPE) == 0){
            pipe_param_list[param_num] = pipe_split(args, i_delim, i);
            param_num++;
            i_delim = i;
        }
        i++;
    }
    
    pipe_param_list[param_num] = pipe_split(args, i_delim, i);
    
    param_num++;
    pipe_param_list[param_num] = NULL;
    
    pipe_it_up(pipe_param_list);
     
    return 1;
}

void pipe_it_up(char *** args){
    int i = 0, num_pipes = 0;
    int stat;
    //int write_to_pipe = 1;
    
    //printf("WELCOME TO PIPE IT UP\n");fflush(0);
    while(args[num_pipes] != NULL){
        //printf("args[%d][0] : %s\n", num_pipes, args[num_pipes][0]);
        //printf("args[%d][1] : %s\n", num_pipes, args[num_pipes][1]);
        num_pipes++;
    }
    
    //char *argv[3];
    
    int num_fd = (num_pipes-1)*2;
    //int fds[num_fd];
    
    int ** fds = (int**)malloc(sizeof(int*)*num_fd);
    //int stdin_dup = dup(STDIN_FILENO);
    //int stdout_dup = dup(STDOUT_FILENO);
    
    for(i = 0; i < num_fd; i++){
        fds[i] = (int*)malloc(sizeof(int)*2);
        pipe(fds[i]);
    }
    
    //pipe(fds);
    
    for(i = 0; i < num_pipes; i++){

        if (i == 0){
            if (fork()== 0) {
                close(1);
                dup(fds[0][1]);  // redirect standard output to fds[0]
                
                int j;
                for(j = 0; j < 2; j++){
                    close(fds[j][0]);
                    close(fds[j][1]);
                }
                
                //argv[0] = "ps";
                //argv[1] = "-e";
                //argv[2] = NULL;
                
                if(execvp(args[i][0], args[i])== -1) exit(-1);
                exit(0);
            }
            
            //wait(&stat);
        } else if(i==num_pipes-1){
            if (fork() == 0) {
                close(0);
                //dup(fds[2*(i-1)]); // redirect standard input to fds[1]
                dup(fds[i-1][0]);
                
                int j;
                for(j = 0; j < 2; j++){
                    close(fds[j][0]);
                    close(fds[j][1]);
                }
                
                if(execvp(args[i][0], args[i])== -1) exit(-1);
                exit(0);
            }
            
            //wait(&stat);
        }
        
        else {//if(write_to_pipe == 1){
            
            if (fork()== 0) {
                close(0);
                dup(fds[i-1][0]);
                
                close(1);
                dup(fds[i][1]);  // redirect standard output to fds[1]
                
                int j;
                for(j = 0; j < num_fd; j++){
                    close(fds[j][0]);
                    close(fds[j][1]);
                }
                
                if(execvp(args[i][0], args[i])== -1) exit(-1);
                exit(0);
            }
            
            //wait(&stat);
        }
        
    }
    
    int j;
    for(j = 0; j < num_fd; j++){
        close(fds[j][0]);
        close(fds[j][1]);
    }
    
    for(j = 0; j < num_pipes; j++)
        wait(&stat);
    
}

int p_redirect(char ** args){
    
    int i = 0;
    //int fd1;
    int ir_index = -1, or_index = -1;
    while(args[i] != NULL){
        if (strcmp(args[i], P_IREDIRECT) == 0){
            ir_index = i;
        }
        
        if (strcmp(args[i], P_OREDIRECT) == 0){
            or_index = i;
        }
        
        i++;
    }
    
    //close(STDIN_FILENO);
    //fd1 = open ("myjunk", O_RDWR | O_CREAT | O_TRUNC, 0777);
    
    if(ir_index != -1 && or_index != -1){
        //printf("Usage: command < file1 > file2\n");fflush(0);
        
        char ** redirect_args = ior_split(args, ir_index);
        ioredirect(redirect_args, args[ir_index+1], args[or_index+1]);
    }else if (ir_index != -1){
        //printf("Usage: command < file1 \n");fflush(0);
        
        char ** redirect_args = ior_split(args, ir_index);
        iredirect(redirect_args, args[ir_index+1]);
        
    }else if (or_index != -1){
        //printf("Usage: command > file2 \n");fflush(0);
        
        char ** redirect_args = ior_split(args, or_index);
        oredirect(redirect_args, args[or_index+1]);
    }
    
    return 1;
}

void ioredirect(char** args, char* ifile, char* ofile){
    int fd1, fd2, stdin_dup, stdout_dup;
    
    stdin_dup = dup(STDIN_FILENO);
    stdout_dup = dup(STDOUT_FILENO);
    
    close(STDIN_FILENO);
    fd1 = open (ifile, O_RDWR);
    
    close(STDOUT_FILENO);
    fd2 = open (ofile, O_RDWR | O_CREAT, 0777);
    
    p_extern(args);
    
    close(fd1);
    close(fd2);
    dup2(stdin_dup, STDIN_FILENO);
    dup2(stdout_dup, STDOUT_FILENO);
    close(stdin_dup);
    close(stdout_dup);
    
    //printf("got out, back to stdout\n");
    
}

void iredirect(char** args, char* ifile){
    int fd1, stdin_dup;
    
    stdin_dup = dup(STDIN_FILENO);
    
    close(STDIN_FILENO);
    fd1 = open (ifile, O_RDWR);
    
    // call to execvp in p_extern
    p_extern(args);
    
    close(fd1);
    dup2(stdin_dup, STDIN_FILENO);
    close(stdin_dup);
    
}

void oredirect(char** args, char* ofile){
    int fd1, stdout_dup;
    
    stdout_dup = dup(STDOUT_FILENO);
    
    close(STDOUT_FILENO);
    fd1 = open (ofile, O_RDWR | O_CREAT, 0777);
    
    p_extern(args);
    
    close(fd1);
    dup2(stdout_dup, STDOUT_FILENO);
    close(stdout_dup);
    
}

// get the type of syscall and return enum values
SYSCALL get_syscall(char *buff){
    SYSCALL retVal;
    
    if(buff == NULL){
        return IGNORE;
    }
    
    if(strcmp(P_EXIT, buff) == 0){
        retVal = EXIT;
    }else if (strcmp(P_CD, buff) == 0){
        retVal = CD;
    }else if (strcmp(P_PWD, buff) == 0){
        retVal = PWD;
    }else if (strcmp(P_SET, buff) == 0){
        retVal = SET;
    }else if (strcmp(P_MYLS, buff) == 0){
        retVal = MYLS;
    }else{
        retVal = EXTERN;
    }
    
    return retVal;
    //memcpy( subbuff, &buff[10], 4 );
}

// Helper functions to manipulate arrays
// ===============================================================

// look at all the arguments passed in and keep everything before "<" || ">"
// return the new array afer removing the trailing values (non-arguments)
char** ior_split(char** args, int delim){
    int i;
    //char tmp[80];
    
	//printf("Inside of ior_split\n");fflush(0);
    //strcpy(tmp, args[delim]);
    char ** ret = (char**)malloc(sizeof(char*)*BUFFER_SIZE);
    for(i=0; i<delim; i++){
        ret[i] = (char*)malloc(sizeof(char)*80);
        strcpy(ret[i], args[i]);
    }
    
    ret[delim] = NULL;
    
    /*
    i = 0;
    while(ret[i] != NULL){
        printf("ret[%d]: %s\n", i, ret[i]);fflush(0);
        i++;
    }
    */
     
    return ret;
}

char** pipe_split(char** args, int i_delim, int f_delim){
    int i;
    int max_itr;
    if(i_delim == 0)
        max_itr = f_delim-i_delim;
    else
        max_itr = f_delim-i_delim - 1;
    //char tmp[80];
    
    //printf("Inside of ior_split\n");fflush(0);
    //strcpy(tmp, args[delim]);
    char ** ret = (char**)malloc(sizeof(char*)*BUFFER_SIZE);
    
    for(i=0; i<max_itr; i++){
        ret[i] = (char*)malloc(sizeof(char)*80);
        if(i_delim == 0)
            strcpy(ret[i], args[i_delim+i]);
        else
            strcpy(ret[i], args[i_delim+1+i]);
    }
    
    ret[i] = NULL;
    
    /*
    i = 0;
    while(ret[i] != NULL){
        printf("ret[%d]: %s\n", i, ret[i]);fflush(0);
        i++;
    }
    */
    
    return ret;
}

// split the input using delim as the value to split it on
char** split_buff(char *buff, char DELIM[2]){
    
    char *token;
    char **tokens = (char**)malloc(sizeof(char*)*BUFFER_SIZE);
    
    int pos = 0;

    token = strtok(buff, DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        token = strtok(NULL, DELIM);
    }
    
    tokens[pos] = NULL;
	
    return tokens;
}
