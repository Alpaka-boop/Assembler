#include <assert.h>
#include <string.h>

#include "tree.h"

int height (Tree *tree)
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

int balance_fact (Tree *tree)
{
    int res1 = height(tree->right);
    int res2 = height(tree->left);
    // return height(tree->right) - height(tree->left);
    return res1 - res2;
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

void insert (Tree **tree, const char *str, const size_t lenth, const size_t cmd_code)
{
    if (*tree == nullptr)
    {
        *tree = (Tree *) calloc(1, sizeof(Tree));
        (*tree)->str = (char *) calloc(lenth + 1, sizeof(char));
        strncpy((*tree)->str, str, lenth);
        (*tree)->str_lenth = lenth;
        (*tree)->right = (*tree)->left = nullptr;
        (*tree)->height = 1;
        (*tree)->cmd_code = cmd_code;
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
                    insert(&(*tree)->left, str, lenth, cmd_code);
                    
                    break;
                }

                else 
                {
                    (*tree)->left = (Tree *) calloc(1, sizeof(Tree));
                    (*tree)->left->right = nullptr;
                    (*tree)->left->left = nullptr;
                    (*tree)->left->str = (char *) calloc(lenth + 1, sizeof(char));
                    strncpy((*tree)->left->str, str, lenth);
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
                    insert(&(*tree)->right, str, lenth, cmd_code);
                    
                    break;
                }

                else 
                {
                    (*tree)->right = (Tree *) calloc(1, sizeof(Tree));
                    (*tree)->right->right = nullptr;
                    (*tree)->right->left = nullptr;
                    (*tree)->right->str = (char *) calloc(lenth + 1, sizeof(char));
                    strncpy((*tree)->right->str, str, lenth);
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
    insert(tree, "push", 4, 1);
    insert(tree, "mul", 3, 2);
    insert(tree, "add", 3, 3);
    insert(tree, "sub", 3, 4);
    insert(tree, "div", 3, 5);
    insert(tree, "out", 3, 6);
}
