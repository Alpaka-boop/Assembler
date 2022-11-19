#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "assembler.h"
#include "tree.h"

#define input_filename "test.asm"
#define output_filename "test_code.txt"

int read_arg (char **arg, int *max_arg_lenth, const int *buffer, int *curr_char)
{
    int arg_lenth = 0;

    for (arg_lenth = 0; 
          buffer[*curr_char] != ' ' && buffer[*curr_char] != '\n' && buffer[*curr_char] != EOF; 
          arg_lenth++, (*curr_char)++)

    {
        (*arg)[arg_lenth] = buffer[*curr_char];

        if (arg_lenth < *max_arg_lenth - 2)
        {
            *arg = (char *) realloc(*arg, *max_arg_lenth + 2);
            *max_arg_lenth *= 2;
        }

    }

    (*arg)[arg_lenth] = '\0';

    return arg_lenth;
}

int read_file (int **buffer)
{
    FILE   *input_file = NULL;

    input_file = fopen(input_filename, "r");

    assert(input_file);

    struct stat st = {};

    stat(input_filename, &st);

    int number_of_characters = st.st_size / sizeof(char);
    int i = 0;

    *buffer = (int *) calloc(number_of_characters + 1, sizeof(int));

    while (((*buffer)[i++] = fgetc(input_file)) != EOF) {;}

    fclose(input_file);

    return number_of_characters;
}

void file_printer (const Tree *tree, const int *buffer, const int number_of_characters)
{
    int  curr_arg_lenth = 30;
    char *curr_arg = (char *) calloc(curr_arg_lenth, sizeof(char));
    FILE *opfile = fopen(output_filename, "w");

    for (int curr_char = 0; curr_char < number_of_characters; )
    {
        while (curr_char < number_of_characters && !isalpha(buffer[curr_char]))
        {
            fprintf(opfile, "%c", buffer[curr_char]);
            curr_char++;
        }

        read_arg(&curr_arg, &curr_arg_lenth, buffer, &curr_char);

        const Tree *cmd = exist(tree, curr_arg);

        if (cmd != nullptr)
        {
            fprintf(opfile, "%lu", cmd->cmd_code);
        }

        else 
        {
            fprintf(opfile, "Error: No such command: %s", curr_arg);
        }

        if (buffer[curr_char] == EOF)
        {
            break;
        }

        if (buffer[curr_char + 1] == EOF)
        {
            fprintf(opfile, "%c", buffer[curr_char]);

            break;
        }
    } 

    free(curr_arg);
}

int main ()
{
    Tree *tree = nullptr;
    int *buffer = nullptr;

    tree_ctor(&tree);

    int num_of_char = read_file(&buffer);

    file_printer(tree, buffer, num_of_char);
    tree_dtor(&tree);

    return 0;
}
