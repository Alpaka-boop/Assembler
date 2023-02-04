#include "tree.h"
#include "assembler.h"
#include "readwrite.h"

int ax_reg = -1;
int bx_reg = 0;
int cx_reg = 1;

int main ()
{
    int  max_lbl_num  = 10;
    int  *labels      = (int *) calloc(max_lbl_num, sizeof(int));
    Tree *tree        = nullptr;
    int  *buffer      = nullptr;

    for (int i = 0; i < max_lbl_num; i++)
    {
        labels[i] = -1;
    }

    tree_ctor(&tree);

    size_t num_of_char = read_file(&buffer);

    assembler(tree, buffer, num_of_char, max_lbl_num, labels);
    assembler(tree, buffer, num_of_char, max_lbl_num, labels);
    tree_dtor(&tree);
    free(labels);

    return 0;
}
