#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "tree.h"
#include "assembler.h"
#include "color.h"

#define input_filename "test.asm"
#define output_filename "test.code"

enum STAT {
    ARG, 
    CMD
};

int read_word (char **arg, int *max_arg_lenth, int *buffer, size_t *curr_char)
{
    int arg_lenth = 0;

    for (int i = 0; i < *max_arg_lenth; i++)
    {
        (*arg)[i] = 0;
    }

    for (arg_lenth = 0; 
          arg_lenth < *max_arg_lenth && buffer[*curr_char] != ' ' && buffer[*curr_char] != '\n' && buffer[*curr_char] != EOF; 
          arg_lenth++, (*curr_char)++)

    {
        (*arg)[arg_lenth] = (char) buffer[*curr_char];

        if (arg_lenth == *max_arg_lenth - 2)
        {
            *arg = (char *) realloc(*arg, *max_arg_lenth * 2);
            *max_arg_lenth *= 2;
        }

    }

    (*arg)[arg_lenth] = '\0';

    *arg = (char *) realloc(*arg, arg_lenth);
    *max_arg_lenth = arg_lenth;

    return arg_lenth;
}

size_t read_file (int **buffer)
{
    FILE *input_file = NULL;

    input_file = fopen(input_filename, "r");

    assert(input_file);

    struct stat st = {};

    stat(input_filename, &st);

    size_t number_of_characters = st.st_size / sizeof(char);
    int i = 0;

    *buffer = (int *) calloc(number_of_characters + 1, sizeof(int));

    while (((*buffer)[i++] = fgetc(input_file)) != EOF) {;}

    (*buffer)[i - 1] = '\0';

    fclose(input_file);

    return number_of_characters;
}

int decoder (char *num_arr, int num_len)
{
    int number = 0;

    for (int i = 0; i < num_len; i++)
    {
        number = number * 10 + num_arr[num_len - i - 1] - 48;
    }

    return number;
}

void print_arg (int *buf, size_t *ch, FILE *file, int line_num)
{
    int arg_lenth = 30;
    char *curr_arg = (char *) calloc(arg_lenth, sizeof(char));

    read_word(&curr_arg, &arg_lenth, buf, ch);
    is_correct(curr_arg, arg_lenth, ARG, line_num);

    int arg = decoder(curr_arg, arg_lenth);

    fwrite(&arg, sizeof(char), 1, file);

    free(curr_arg);
}

int degree (int cmd_code)
{
    int deg = 0;

    if (cmd_code == 0)
        return 1;

    while (cmd_code)
    {
        cmd_code /= 10;
        deg += 1;
    }

    return deg;
}

char is_num (char *arg, int len)
{
    for (int i = 0; i < len; i++)
    {
        if ('0' > arg[i] && arg[i] > '9')
        {
            return arg[i];
        }
    }

    return 0;
}

char is_cmd (char *arg, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (!isalpha(arg[i]))
        {
            return arg[i];
        }
    }

    return 0;
}

void is_correct (char *arg, int len, int stat, int line_num)
{
    if (arg == nullptr)
    {
        if (stat == CMD)
        {
            printf("%i: " RED "error:" RESET " no such command: \'%s\'\n", line_num, arg);
            abort();
        }

        else if (stat == ARG)
        {
            printf("%i: " RED "error:" RESET " few parameters: \n", line_num);
            abort();
        }
    }

    char (*func[2]) (char *arg, int len) = {&is_num, &is_cmd};
    int res = func[stat](arg, len);

    if (res)
    {
        switch (stat)
        {
        case ARG:
            printf("line: %i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
            abort();
        
        case CMD:
            printf("%i: " RED "error:" RESET "unrecognised token \'%c\'\n", line_num, res);
            abort();
    
        default:
            break;
        }
    }
}

void file_printer (const Tree *tree, int *buffer, const size_t number_of_characters)
{
    size_t  curr_char      = 0;
    int     curr_cmd_lenth = 30;
    int     curr_arg_lenth = 30;
    int     line_num       = 0;
    FILE    *opfile        = fopen(output_filename, "wb");

    if (!isalpha(buffer[0]))
    {
        printf("line: %i: " RED "error:" RESET "unrecognised token \'%c\'\n", line_num, buffer[0]);
        abort();
    }

    while (buffer[curr_char] != '\0' && curr_char < number_of_characters)
    {
        char *curr_cmd = (char *) calloc(30, sizeof(char));

        read_word(&curr_cmd, &curr_cmd_lenth, buffer, &curr_char);
        is_correct(curr_cmd, curr_arg_lenth, CMD, line_num);

        const Tree *cmd = exist(tree, curr_cmd);

        fwrite(&cmd->cmd_code, sizeof(char), 1, opfile);

        for (int arg_num = 0; arg_num < cmd->arg_num; arg_num++)
        {
            if (buffer[curr_char] != ' ')
            {
                printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, buffer[curr_char]);
                printf(GREY "note: " RESET "cmd and args & arg and arg must be devided by space\n");
                abort();
            }

            fwrite(&buffer[curr_char++], sizeof(char), 1, opfile);
            print_arg(buffer, &curr_char, opfile, line_num);
        }

        if (buffer[curr_char] != '\n')
        {
            printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, buffer[curr_char]);
            printf(GREY "note: " RESET "the cmd: \'%s\' must have %i args\n", curr_cmd, cmd->arg_num);
            printf(GREY "note: " RESET "line must be ended with the \'\\n\' symbol\n");
            abort();
        }

        fwrite(&buffer[curr_char++], sizeof(char), 1, opfile);
        free(curr_cmd);
    }    

    fclose(opfile);
}
