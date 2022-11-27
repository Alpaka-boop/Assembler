#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "tree.h"

int read_word (char **arg, int *max_arg_lenth, int *buffer, size_t *curr_char);
size_t read_file (int **buffer);
int decoder (char *num_arr, int num_len);
void print_arg (int *buf, size_t *ch, FILE *file, int line_num);
int degree (int cmd_code);
char is_num (char *arg, int len);
char is_cmd (char *arg, int len);
void is_correct (char *arg, int len, int stat, int line_num);
void file_printer (const Tree *tree, int *buffer, const size_t number_of_characters);

#endif
