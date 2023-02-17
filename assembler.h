#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "tree.h"

enum CMD
{
    CMD_HLT,
    CMD_MUL,
    CMD_ADD,
    CMD_SUB,
    CMD_DIV,
    CMD_OUT,
    CMD_PUSH,
    CMD_POP,
    CMD_JMP,
    CMD_JB,
    CMD_JBE,
    CMD_JA, 
    CMD_JAE,
    CMD_JE,
    CMD_JNE,
    CMD_CALL
};

enum STAT {
    ARG, 
    CMD, 
    LBL,
    REG
};

char is_num (char *arg, int len);
char is_cmd (char *arg, int len);
char is_lbl (char *arg, int len);
int is_correct (char *arg, int len, int stat, int line_num);
int assembler (const Tree *tree, int *buffer, const size_t number_of_characters, int max_lbl_num, int *labels);
int find_arg (int *buffer, size_t curr_char);

#endif
