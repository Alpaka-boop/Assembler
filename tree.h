#ifndef TREEE_H
#define TREEE_H

#include <stdlib.h>

struct Tree {
    char *str;
    int str_lenth;
    int arg_num;
    int cmd_code;

    char height;
    Tree *left;
    Tree *right;
};

char height (Tree* tree);
char balance_fact (Tree *tree);
Tree* rotate_right (Tree **tree);
Tree* rotate_left (Tree **tree);
void balance (Tree **tree);
int comparator (const char *l1, const char *l2);
void insert (Tree **tree, const char *str, const int lenth, const int cmd_code, int arg_num);
void tree_dtor (Tree **tree);
const Tree *exist (const Tree *tree, char *str);
void tree_ctor (Tree **tree);

#endif
