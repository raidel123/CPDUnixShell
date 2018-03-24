# UnixShell ( MYSHELL )

A simplified version of Unix Shell program

## Project
### Project Objectives:
* Practicing Unix system calls
* Understanding Unix process management
* Synchronization
* Inter-process communication (IPC)

## Installation Instructions (Ubuntu )
### How to compile using make
```
	$ make 	// will build both executables and name them myshell and myls respectively.
```
### How to execute:
Run MyShell Program
```
	$ ./myshell
```
External command implementation for 'ls -l' (setenv for myls in MyShell)
```
	$ ./myls
```

### How to set MYPATH and run myls as external command (do not include a trailing '/' after the last directory listed for MYPATH):
```
    $ ./myshell
	$ <myshell> set MYPATH=/path/to/file 	
	$ <myshell> myls
```

## File Description:
* main.c - myshell program code
* p_globals.h - all global variables used in main.c
* myls.c - myls program code
* makefile - will build both myshell.c and myls.c and will name their executable myshell and myls respectively

## MyShell Description:
### MyShell Supports
Four built-in commands
  * "exit"
  * "cd"
  * "pwd"
  * "set"

One external command (program):
  * "myls"

All existing external commands in the system. For these commands, "exec" functions are used in the shell implementation.

Pipes:
* e.g, command1 | command2 | command3 | command4.

I/O redirections (i.e):
* command1 < file1
* command1 > file1
* command1 < file1 > file2

Simple form of background processes, that is, commands followed by an ampersand (&). We do not require a full support of background processes.

MyShell assumed that pipe and I/O redirection will not appear together in any command line. You can also assume that pipe and background process will not appear together in any command line.

MyShell does not support any other special characters (in particular, special character expansion such as *).

## Commands Supported

### exit:
Terminate the shell
### cd:
change the current working directory. Syntax: cd dir. dir is the directory where the user wants to change to. If dir starts with a slash (/"), dir is an absolute path. Otherwise, dir is a path related to the current directory.
### pwd:
print the absolute path of the current working directory

### set:
set the value of the environment variables. You only need to support one environment value MYPATH, which is the search path for the external command you implement (see below).
* Syntax: set MYPATH=path1:path2...

### myls:
List files under a directory. The output format should be similar to "ls -l".
Implemented this command via file and directory access functions.

Your shell should also terminate if end of file character (CTRL-D) is encountered.

## limitations that myshell may have:

* It supports pipes as required, but will hang after 4 pipes (so the limit of pipes it can 	handle is up to 4).

* myls is supported as long as the path is set in MYPATH, it will only look in the first 		location stored in MYPATH for myls.

* Background processes will only execute external commands when triggered (using execv), they will not call any of the built in command. Press enter after call to background process to input next line.


## Authors:
* [Raidel Hernandez](https://github.com/raidel123)

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
