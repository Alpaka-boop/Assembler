#ifndef READWRITE_H
#define READWRITE_H

#include <stdio.h>
#include "tree.h"

int read_word (char **arg, int *max_arg_lenth, int *buffer, size_t *curr_char);
size_t read_file (int **buffer);
void print_arg (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num);
void print_lbl (int *buf, size_t *ch, FILE *file, int *labels, int max_lbl_num, int *res_buf_num);
int char_to_int (char *lbl, int lbl_lenth);
void int_to_char (int int_num, char *char_num, int lenth);
int deg_of_num (int number);

#endif
