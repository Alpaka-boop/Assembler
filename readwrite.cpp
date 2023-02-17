#include <assert.h>
#include <sys/stat.h>
#include <ctype.h>

#include "tree.h"
#include "assembler.h"
#include "readwrite.h"
#include "color.h"

#define input_filename "factorial.asm"

int read_word (char *arg, int max_arg_lenth, int *buffer, size_t *curr_char)
{
    int arg_lenth = 0;

    for (arg_lenth = 0; arg_lenth < max_arg_lenth; arg_lenth++, (*curr_char)++)
    {
        arg[arg_lenth] = (char) buffer[*curr_char];
    }

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

int lenth_of_token (const int *buffer, int ch)
{
    int start_ch = ch;

    while (buffer[ch] != '\0' && buffer[ch] != '\n' && buffer[ch] != ' '  && buffer[ch] != EOF && buffer[ch] != ']') {ch++;}
    
    return ch - start_ch;
}

void print_arg (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num)
{   
    char first     = (char) (buf[*ch]);
    int  arg_lenth = lenth_of_token(buf, *ch);
    char *curr_arg = (char *) calloc(arg_lenth, sizeof(char));

    read_word(curr_arg, arg_lenth, buf, ch);
    is_correct(curr_arg, arg_lenth, ARG * (first != 'r') + REG * (first == 'r'), line_num);
    
    if (first != 'r')
    {
        fwrite(curr_arg, sizeof(char), arg_lenth, file);
        *res_buf_num += arg_lenth;
    }

    else
    {
        fwrite(&(buf[*ch - 2]), sizeof(char), 1, file);
        *res_buf_num += 1;
    }

    free(curr_arg);
}

int error_dump (int ch, int line_num, char recom_sym)
{
    printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, ch);

    if (recom_sym != '\0')
    {
        printf(GREY "note: " RESET "it may be you wanted to print \'%c'\\n", recom_sym);
    }

    return -1;
}

int print_reg (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num)
{
    if (buf[*ch] == 'r')
    {
        print_arg(buf, ch, file, line_num, res_buf_num);
    }
    else 
    {
        int error = error_dump(buf[*ch], line_num, 'r');

        if (error == -1)
        {
            return error;
        }
    }

    return 0;
}

int print_reg_num (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num)
{
    if (buf[*ch] == ' ') 
    {
        (*ch)++; 
    }
    else 
    {
        int error = error_dump(buf[*ch], line_num, ' ');

        if (error == -1)
        {
            return error;
        }
    }

    if (buf[*ch] == '+') 
    {
        (*ch)++;
    }
    else
    { 
        int error = error_dump(buf[*ch], line_num, '+');

        if (error == -1)
        {
            return error;
        }
    }

    if (buf[*ch] == ' ') 
    {
        (*ch)++;
    }
    else
    {
        int error = error_dump(buf[*ch], line_num, ' ');

        if (error == -1)
        {
            return error;
        }
    }

    if (isdigit(buf[*ch]))
    {
        print_arg(buf, ch, file, line_num, res_buf_num);
    }
    else 
    {
        int error = error_dump(buf[*ch], line_num, 'r');

        if (error == -1)
        {
            return error;
        }
    }

    return 0;
}

int print_num (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num)
{
    if (isdigit(buf[*ch]))
    {
        print_arg(buf, ch, file, line_num, res_buf_num);
    }
    else 
    {
        int error = error_dump(buf[*ch], line_num, 'r');

        if (error == -1)
        {
            return error;
        }
    }

    return 0;
}

int recognize_args (int *buf, size_t *ch, FILE *file, int line_num, int *res_buf_num, char cmd_code)
{
    cmd_code = (cmd_code << 1) + (buf[*ch] == '[');

    size_t curr_ch = *ch;
    char   regs_fl = 0;
    char   nums_fl = 0;

    while (buf[curr_ch] != '\n')
    {
        if (buf[curr_ch] == 'r')
        {
            regs_fl = 1;
        }

        if (isdigit(buf[curr_ch]))
        {
            nums_fl = 1;
        }
        
        curr_ch++;
    }
    
    cmd_code = (cmd_code << 1) + nums_fl;
    cmd_code = (cmd_code << 1) + regs_fl;

    fwrite(&cmd_code, sizeof(char), 1, file);

    if (isdigit(buf[*ch]) || buf[*ch] == 'r' || (buf[*ch] == '-' && isdigit(buf[*ch + 1])))
    {
        print_arg(buf, ch, file, line_num, res_buf_num);
        return 0;
    }

    else if (buf[*ch] == '[')
    {
        (*ch)++;

        if (regs_fl)
        {
            int error = print_reg(buf, ch, file, line_num, res_buf_num);

            if (error == -1)
            {
                return error;
            }
        }
        if (regs_fl && nums_fl)
        {
            int error = print_reg_num(buf, ch, file, line_num, res_buf_num);

            if (error == -1)
            {
                return error;
            }
        }

        if (!regs_fl && nums_fl)
        {
            int error = print_num(buf, ch, file, line_num, res_buf_num);

            if (error == -1)
            {
                return error;
            }
        }
        
        if (buf[*ch] == ']')
        {
            (*ch)++;
            return 0;  
        }
    }

    else 
    {
        int error = error_dump(buf[*ch], line_num, '\0');

        if (error == -1)
        {
            return error;
        }
    }

    return 0;
}

void print_lbl (int *buf, size_t *ch, FILE *file, int *labels, int max_lbl_num, int *res_buf_num)
{
    int  arg_lenth = lenth_of_token(buf, *ch);
    char *curr_arg = (char *) calloc(arg_lenth, sizeof(char));

    read_word(curr_arg, arg_lenth, buf, ch);

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
        char_num[lenth - i - 1] = (char) (int_num % 10 + '0');
        int_num /= 10;
    }
}
