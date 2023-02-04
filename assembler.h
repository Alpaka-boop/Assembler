#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "tree.h"

extern int ax_reg;
extern int bx_reg;
extern int cx_reg;

enum STAT {
    ARG, 
    CMD, 
    LBL,
    REG
};


char is_num (char *arg, int len);
char is_cmd (char *arg, int len);
char is_lbl (char *arg, int len);
void is_correct (char *arg, int len, int stat, int line_num);
void assembler (const Tree *tree, int *buffer, const size_t number_of_characters, int max_lbl_num, int *labels);

#endif
