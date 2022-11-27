#include "tree.h"
#include "assembler.h"

int main ()
{
    Tree *tree = nullptr;
    int *buffer = nullptr;

    tree_ctor(&tree);

    size_t num_of_char = read_file(&buffer);

    file_printer(tree, buffer, num_of_char);
    tree_dtor(&tree);

    return 0;
}
