#include <stdio.h>
#include <ctype.h>
#include <string>

#include "assembler.h"
#include "readwrite.h"
#include "color.h"

#define output_filename "factor.bin"

char is_num (char *arg, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(arg[i]) && arg[i] != '-')
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
    {
        return arg[0];
    }
    
    if (len < 2)
    {
        return -1;
    }

    for (int i = 1; i < len; i++)
    {
        if (!isdigit(arg[i]))
        {
            return arg[i];
        }
    }

    return 0;
}

char is_reg (char *arg, int len)
{
    if (len != 3)
    {
        return -1;
    }
    
    if (arg[0] != 'r')
    {
        return arg[0];
    }

    if (arg[2] != 'x')
    {
        return arg[1];
    }

    if (arg[1] < 'a' || arg[1] > 'f')
    {
        return arg[1];
    }

    return 0;
}

int is_correct (char *arg, int len, int stat, int line_num)
{
    if (arg == nullptr)
    {
        if (stat == CMD)
        {
            printf("%i: " RED "error:" RESET " no such command: \'%s\'\n", line_num, arg);
            return -1;
        }

        else if (stat == ARG || stat == REG)
        {
            printf("%i: " RED "error:" RESET " few parameters: \n", line_num);
            return -1;
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
            return -1;
        
        case CMD:
            printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
            return -1;  
        case LBL:
            if (res == -1)
            {
                printf("%i: " RED "error: " RESET "there must be a name of label written after \':\'\n", line_num);
                return -1;
            }

            else 
            {
                printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
                printf(GREY "note: " RESET "label must start with \':\' and must have only numbers\n");
                return -1;
            }
        
        case REG:
            if (res == -1)
            {
                printf("%i: " RED "error: " RESET "register must consist of 3 leters\n", line_num);
                printf(GREY "note: " RESET "register must consist of 3 letters: first -- \'r\' second -- \'a\', \'b\', \'c\', \'e\', \'f\', \'d\', third -- \'x\'\n");
                return -1;
            }
            

            else 
            {
                printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, res);
                printf(GREY "note: " RESET "register must consist of 3 letters: first -- \'r\' second -- \'a\', \'b\', \'c\', \'e\', \'f\', \'d\', third -- \'x\'\n");
                return -1;
            }
    
        default:
            break;
        }
    }

    return 0;
}

int assembler (const Tree *tree, int *buffer, const size_t number_of_characters, int max_lbl_num, int *labels)
{
    size_t  curr_char      = 0;
    int     curr_cmd_lenth = 0;
    int     res_buf_num    = 0; 
    int     line_num       = 1;
    FILE    *opfile        = fopen(output_filename, "wb");
    int     curr_lbl_num   = 1;

    while (buffer[curr_char] != '\0' && curr_char < number_of_characters)
    {
        while (buffer[curr_char] == '\n')
        {
            curr_char++;
            line_num++;
        }

        curr_cmd_lenth = lenth_of_token(buffer, curr_char);
        char *curr_cmd = (char *) calloc(curr_cmd_lenth, sizeof(char)); 
        read_word(curr_cmd, curr_cmd_lenth, buffer, &curr_char);

        if (curr_cmd[0] == ':')
        {
            int error = is_correct(curr_cmd, curr_cmd_lenth, LBL, line_num);

            if (error == -1)
            {
                return error;
            }

            labels[curr_lbl_num++] = res_buf_num;
        }

        else if (curr_cmd[0] == '#')
        {
            while (curr_char < number_of_characters && buffer[curr_char] != '\n')
            {
                curr_char++;
            }
        }

        else 
        {    
            int error = is_correct(curr_cmd, curr_cmd_lenth, CMD, line_num);

            if (error == -1)
            {
                return error;
            }

            const Tree *cmd = exist(tree, curr_cmd);

            if (!cmd)
            {
                printf("%i: " RED "error: " RESET "no such command \'%s\'\n", line_num, curr_cmd);
                return 1;
            }

            char cmd_code = cmd->cmd_code;

            if (cmd->arg_num == 0)
            {
                cmd_code <<= 3;
                fwrite(&cmd_code, sizeof(char), 1, opfile);
            }

            for (int arg_num = 0; arg_num < cmd->arg_num; arg_num++)
            {
                if (cmd->cmd_code == CMD_POP && buffer[curr_char] == '\n')
                {
                    cmd_code <<= 3;
                    fwrite(&cmd_code, sizeof(char), 1, opfile);
                    res_buf_num++;
                    break;
                }

                if (buffer[curr_char] != ' ')
                {
                    printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, buffer[curr_char]);
                    printf(GREY "note: " RESET "cmd and args & arg and arg must be devided by space\n");
                    return 1;
                }

                curr_char++; // skip space 

                if (cmd->cmd_code < CMD_JMP) 
                {
                    int error = recognize_args(buffer, &curr_char, opfile, line_num, &res_buf_num, cmd_code);

                    if (error == - 1)
                    {
                        return error;
                    }
                }

                else 
                {
                    cmd_code <<= 3;

                    fwrite(&cmd_code, sizeof(char), 1, opfile);
                    res_buf_num++;
                    print_lbl(buffer, &curr_char, opfile, labels, max_lbl_num, &res_buf_num);
                }
            }
        }

        if (buffer[curr_char] != '\n')
        {
            printf("%i: " RED "error: " RESET "unrecognised token \'%c\'\n", line_num, buffer[curr_char]);
            printf(GREY "note: " RESET "line must be ended with the \'\\n\' symbol\n");
            return 1;
        }

        if (curr_cmd[0] != ':' && curr_cmd[0] != '#')
        {
            fwrite(&buffer[curr_char], sizeof(char), 1, opfile);
            res_buf_num++;
        }

        free(curr_cmd);
        curr_char++;
        line_num++;
    }    
    
    fclose(opfile);
    return 0;
}

int find_arg (int *buffer, size_t curr_char)
{
    int res = 0;

    while (buffer[curr_char] != '\n' && buffer != '\0' && buffer[curr_char] != EOF)
    {
        if (buffer[curr_char] == ' ')
        {
            res += 1;
        }

        curr_char++;
    }

    return res > 0;
}
