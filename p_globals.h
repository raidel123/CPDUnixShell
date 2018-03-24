//
//  p_globals.h
//  cop5570_proj1
//
//  Created by Raidel Hernandez on 10/16/17.
//  Copyright © 2017 Raidel Hernandez. All rights reserved.
//

#ifndef p_globals_h
#define p_globals_h

// global/const variables

// enums
typedef enum {EXIT, CD, PWD, SET, EXTERN, PIPE, REDIRECT, IGNORE, MYLS, BACKGROUND} SYSCALL;

// const

const int   BUFFER_SIZE = 100;
const char  P_EXIT[]        = "exit";
const char  P_CD[]          = "cd";
const char  P_PWD[]         = "pwd";
const char  P_SET[]         = "set";

const char  P_MYLS[]        = "myls";
const char  P_BACKGROUND[]  = "&";
const char  P_PIPE[]        = "|";
const char  P_IREDIRECT[]   = "<";
const char  P_OREDIRECT[]   = ">";

char *builtin_cmd[] = {
    "exit",
    "cd",
    "pwd",
    "set"
};


#endif /* p_globals_h */
