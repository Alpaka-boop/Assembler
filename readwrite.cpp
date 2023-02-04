#include <assert.h>
#include <sys/stat.h>

#include "tree.h"
#include "assembler.h"
#include "readwrite.h"
#include "color.h"

#define input_filename "test.asm"
#define output_filename "test.code"

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

void reg_to_int (char **curr_arg, int *arg_lenth)
{
    switch ((*curr_arg)[0])
    {
    case 'a':
        *arg_lenth = deg_of_num(ax_reg) + 1 * (ax_reg < 0);
        *curr_arg = (char *) realloc(*curr_arg, *arg_lenth);
        int_to_char(ax_reg, *curr_arg, *arg_lenth);
        break;
    
    case 'b':
        *arg_lenth = deg_of_num(bx_reg) + 1 * (bx_reg < 0);
        *curr_arg = (char *) realloc(*curr_arg, *arg_lenth);
        int_to_char(bx_reg, *curr_arg, *arg_lenth);
        break;

    case 'c':
        *arg_lenth = deg_of_num(cx_reg) + 1 * (cx_reg < 0);
        *curr_arg = (char *) realloc(*curr_arg, *arg_lenth);
        int_to_char(cx_reg, *curr_arg, *arg_lenth);
        break;
    
    default:
        break;
    }
}

void print_arg (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num)
{
    int  arg_lenth = 30;
    char *curr_arg = (char *) calloc(arg_lenth, sizeof(char));

    read_word(&curr_arg, &arg_lenth, buf, ch);
    
    if (curr_arg[0] <= '9')
    {  
        is_correct(curr_arg, arg_lenth, ARG, line_num);
    }
    else 
    {
        is_correct(curr_arg, arg_lenth, REG, line_num);
        reg_to_int(&curr_arg, &arg_lenth);
    }

    fwrite(curr_arg, sizeof(char), arg_lenth, file);
    *res_buf_num += arg_lenth;

    free(curr_arg);
}

void print_lbl (int *buf, size_t *ch, FILE *file, int *labels, int max_lbl_num, int *res_buf_num)
{
    int  arg_lenth = 30;
    char *curr_arg = (char *) calloc(arg_lenth, sizeof(char));

    read_word(&curr_arg, &arg_lenth, buf, ch);

    int int_curr_arg = char_to_int(curr_arg, arg_lenth);

    if (int_curr_arg < max_lbl_num && labels[int_curr_arg] != -1)
    {
        int  int_addr   = labels[int_curr_arg];
        char *char_addr = (char *) calloc(deg_of_num(int_addr), sizeof(char));

        int_to_char(int_addr, char_addr, deg_of_num(int_addr));
        fwrite(char_addr, sizeof(char), deg_of_num(int_addr), file);
        free(char_addr);
    }

    *res_buf_num += arg_lenth;

    free(curr_arg);
}

int deg_of_num (int number)
{
    number = number * (number >= 0) + (-1) * number * (number < 0);

    if (number == 0)
    {
        return 1;
    }

    int res = 0;

    while (number > 0)
    {
        number /= 10; 
        res++;
    }

    return res;
}

int char_to_int (char *lbl, int lbl_lenth)
{
    int res = 0;

    for (int i = 0; i < lbl_lenth; i++)
    {
        res = res * 10 + lbl[i] - '0';
    }

    return res;
}

void int_to_char (int int_num, char *char_num, int lenth)
{
    if (int_num < 0)
    {
        int_num *= -1;
        char_num[0] = '-';
        lenth--;
    }
    
    for (int i = 0; i < lenth; i++)
    {
        char_num[lenth - i] = (char) (int_num % 10 + '0');
        int_num /= 10;
    }
}
