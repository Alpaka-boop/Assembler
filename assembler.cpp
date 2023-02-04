#include <stdio.h>
#include <ctype.h>

#include "assembler.h"
#include "readwrite.h"
#include "color.h"

#define output_filename "bin.code"

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

char is_lbl (char *arg, int len)
{
    if (arg[0] != ':')
        return arg[0];
    
    if (len < 2)
    {
        return -1;
    }

    for (int i = 1; i < len; i++)
    {
        if (arg[i] < '0' || arg[i] > '9')
        {
            return arg[i];
        }
    }

    return 0;
}

char is_reg (char *arg, int len)
{
    if (len != 2)
    {
        return -1;
    }
    
    if (arg[1] != 'x')
    {
        return arg[1];
    }

    if (arg[0] < 'a' || arg[0] > 'c')
    {
        return arg[0];
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

        else if (stat == ARG || stat == REG)
        {
            printf("%i: " RED "error:" RESET " few parameters: \n", line_num);
            abort();
        }
    }

    char (*func[]) (char *arg, int len) = {&is_num, &is_cmd, &is_lbl, &is_reg};
    int res = func[stat](arg, len);

    if (res)
    {
        switch (stat)
        {
        case ARG:
            printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
            abort();
        
        case CMD:
            printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
            abort();

        case LBL:
            if (res == -1)
            {
                printf("%i: " RED "error: " RESET "there must be a name of label written after \':\'\n", line_num);
                abort();
            }

            else 
            {
                printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
                printf(GREY "note: " RESET "label must start with \':\' and must have only numbers\n");
                abort();
            }
        
        case REG:
            if (res == -1)
            {
                printf("%i: " RED "error: " RESET "register must consist of two leters\n", line_num);
                abort();
            }

            else 
            {
                printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
                printf(GREY "note: " RESET "register must consist of two letters: thirst -- \'a\', \'b\', \'c\', second -- \'x\'");
                abort();
            }
    
        default:
            break;
        }
    }
}

void assembler (const Tree *tree, int *buffer, const size_t number_of_characters, int max_lbl_num, int *labels)
{
    size_t  curr_char      = 0;
    int     curr_cmd_lenth = 30;
    int     curr_arg_lenth = 30;
    int     res_buf_num    = 0;   /// rename 
    int     line_num       = 0;
    FILE    *opfile        = fopen(output_filename, "wb");
    int     curr_lbl_num   = 1;

    enum commands
    {
        CMD_HLT,
        CMD_MUL,
        CMD_ADD,
        CMD_SUB,
        CMD_DIV,
        CMD_OUT,
        CMD_PUSH,
        CMD_JMP,
        CMD_JB,
        CMD_JBE,
        CMD_JA, 
        CMD_JAE,
        CMD_JE,
        CMD_JNE
    };

    while (buffer[curr_char] != '\0' && curr_char < number_of_characters)
    {
        char *curr_cmd = (char *) calloc(30, sizeof(char));

        read_word(&curr_cmd, &curr_cmd_lenth, buffer, &curr_char);

        if (curr_cmd[0] == ':')
        {
            is_correct(curr_cmd, curr_cmd_lenth, LBL, line_num);
            labels[curr_lbl_num++] = res_buf_num;
        }

        else 
        {    
            is_correct(curr_cmd, curr_arg_lenth, CMD, line_num);

            const Tree *cmd = exist(tree, curr_cmd);

            fwrite(&cmd->cmd_code, sizeof(char), 1, opfile);
            res_buf_num++;

            for (int arg_num = 0; arg_num < cmd->arg_num; arg_num++)
            {
                if (buffer[curr_char] != ' ')
                {
                    printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, buffer[curr_char]);
                    printf(GREY "note: " RESET "cmd and args & arg and arg must be devided by space\n");
                    abort();
                }

                fwrite(&buffer[curr_char++], sizeof(char), 1, opfile);
                res_buf_num++;

                if (cmd->cmd_code < CMD_JMP) 
                {
                    print_arg(buffer, &curr_char, opfile, line_num, &res_buf_num);
                }
                else 
                {
                    print_lbl(buffer, &curr_char, opfile, labels, max_lbl_num, &res_buf_num);
                }
            }
        }

        if (buffer[curr_char] != '\n')
        {
            printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, buffer[curr_char]);
            printf(GREY "note: " RESET "line must be ended with the \'\\n\' symbol\n");
            abort();
        }
        
        if (curr_cmd[0] == ':')
        {
            curr_char++;
        }
        else
        {
            fwrite(&buffer[curr_char++], sizeof(char), 1, opfile);
            res_buf_num++;
        }

        free(curr_cmd);
        line_num++;
    }    
    
    fclose(opfile);
}
