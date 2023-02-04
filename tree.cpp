#include <assert.h>
#include <string.h>

#include "tree.h"

enum ARG_NUM {
    NO_ARG,
    ONE_ARG
};

char height (Tree *tree)
{
    if (tree == nullptr)
        return 0; 

    if (tree->left == nullptr)
    {
        if (tree->right == nullptr)
            return 1;
        
        return tree->right->height + 1;
    }

    if (tree->right == nullptr)
    {
        if (tree->left == nullptr)
            return 1;
        
        return tree->left->height + 1;
    }

    return tree->left->height > tree->right->height ? tree->left->height + 1 : tree->right->height + 1;
}

char balance_fact (Tree *tree)
{
    return height(tree->right) - height(tree->left);
}

Tree *rotate_right (Tree *tree)
{
	Tree* tmp = tree->left;

	tree->left = tmp->right;
	tmp->right = tree;

	tree->height = height(tree);
	tmp->height = height(tmp);

	return tmp;
}

Tree *rotate_left (Tree *tree)
{
	Tree* tmp = tree->right;

	tree->right = tmp->left;
	tmp->left = tree;

	tree->height = height(tree);
	tmp->height = height(tmp);

	return tmp;
}

void balance (Tree **tree)
{
    (*tree)->height = height(*tree);

	if (balance_fact(*tree) == 2)
	{
		if (balance_fact((*tree)->right) < 0)
			(*tree)->right = rotate_right((*tree)->right);
            
        *tree = rotate_left(*tree);
	}

	else if (balance_fact(*tree) == -2)
	{
		if (balance_fact((*tree)->left) > 0)
			(*tree)->left = rotate_left((*tree)->left);

        *tree = rotate_right(*tree);
	}
}

int comparator (const char *l1, const char *l2)
{
    int curr_char = 0;
    enum cmp {
        less   = -1,
        equal  = 0,
        bigger = 1
    };
     
    while (l1[curr_char] == l2[curr_char])
    {
        if (l1[curr_char] == '\0')
            return equal;

        curr_char++;
    }
    
    if (l1[curr_char] > l2[curr_char])
        return bigger;

    return less;    
}

void insert (Tree **tree, const char *str, const int lenth, const int cmd_code, int arg_num)
{
    if (*tree == nullptr)
    {
        *tree        = (Tree *) calloc(1, sizeof(Tree));
        (*tree)->str = (char *) calloc((size_t) lenth + 1, sizeof(char));

        strncpy((*tree)->str, str, (size_t) lenth);
        (*tree)->str_lenth = lenth;
        (*tree)->arg_num   = arg_num;
        (*tree)->right     = (*tree)->left = nullptr;
        (*tree)->height    = 1;
        (*tree)->cmd_code  = cmd_code;
    } 

    else 
    {
        while (true)
        {
            int res = comparator((*tree)->str, str);

            if (res > 0)
            {
                if ((*tree)->left != nullptr)
                {
                    insert(&(*tree)->left, str, lenth, cmd_code, arg_num);
                    
                    break;
                }

                else 
                {
                    (*tree)->left = (Tree *) calloc(1, sizeof(Tree));
                    (*tree)->left->right = nullptr;
                    (*tree)->left->left = nullptr;
                    (*tree)->left->str = (char *) calloc((size_t) lenth + 1, sizeof(char));
                    strncpy((*tree)->left->str, str, (size_t) lenth);
                    (*tree)->left->arg_num = arg_num;
                    (*tree)->left->str_lenth = lenth;
                    (*tree)->left->height = 1;
                    (*tree)->left->cmd_code = cmd_code;
                    
                    break;
                }
            }

            else if (res < 0)
            {
                if ((*tree)->right != nullptr)
                {
                    insert(&(*tree)->right, str, lenth, cmd_code, arg_num);
                    
                    break;
                }

                else 
                {
                    (*tree)->right = (Tree *) calloc(1, sizeof(Tree));
                    (*tree)->right->right = nullptr;
                    (*tree)->right->left = nullptr;
                    (*tree)->right->str = (char *) calloc((size_t) lenth + 1, sizeof(char));
                    strncpy((*tree)->right->str, str, (size_t) lenth);
                    (*tree)->right->arg_num = arg_num;
                    (*tree)->right->str_lenth = lenth;
                    (*tree)->right->height = 1;
                    (*tree)->right->cmd_code = cmd_code;
                    
                    break;
                }
            }

            break;
        }
    }

    balance(tree);
}

void tree_dtor (Tree **tree)
{
    if (*tree == nullptr)
        return;

    tree_dtor(&(*tree)->left);
    tree_dtor(&(*tree)->right);

    free((*tree)->str);
    free(*tree);
}

const Tree *exist (const Tree *tree, char *str)
{
    if (tree == nullptr)
        return nullptr;

    int res = comparator(tree->str, str);

    if (res > 0)
    {
        return exist(tree->left, str);
    }

    if (res < 0)
    {
        return exist(tree->right, str);
    }

    return tree;
}

void tree_ctor (Tree **tree)
{
    const char  *no_arg_cmd[]    = {"hlt", "mul", "add", "sub", "div", "out", nullptr};
    const char  *one_arg_cmd[]   = {"push", "jmp", "jb", "jbe", "ja", "jae", "je", "jne", nullptr};
    int         no_arg_cmd_code  = 0;
    int         one_arg_cmd_code = sizeof(no_arg_cmd) / sizeof(no_arg_cmd[0]) - 1;
    int         curr_cmd_num     = 0;

    while (no_arg_cmd[curr_cmd_num])
    {
        insert(tree, no_arg_cmd[curr_cmd_num], (int) strlen(no_arg_cmd[curr_cmd_num]), no_arg_cmd_code, NO_ARG);
        curr_cmd_num++;
        no_arg_cmd_code++;
    }

    curr_cmd_num = 0;

    while (one_arg_cmd[curr_cmd_num])
    {
        insert(tree, one_arg_cmd[curr_cmd_num], (int) strlen(one_arg_cmd[curr_cmd_num]), one_arg_cmd_code, ONE_ARG);
        curr_cmd_num++;
        one_arg_cmd_code++;
    }
}
