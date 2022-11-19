#ifndef ASSEMBLER_H
#define ASSEMBLER_H

int read_arg (char **arg, int *max_arg_lenth, const int *buffer, int *curr_char);
int read_file (int **buffer);
void file_printer (const Tree *tree, const int *buffer, const int number_of_characters);

#endif
