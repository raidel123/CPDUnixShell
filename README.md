# UnixShell ( MYSHELL )

A simplified version of Unix Shell program

## Project
### Project Objectives: 
* Practicing Unix system calls
* Understanding Unix process management
* Synchronization
* Inter-process communication (IPC)

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

## Authors:
* [Raidel Hernandez](https://github.com/raidel123)

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
