#include <stdio.h>
#include "./../includes/gb.h"


int main(int argc, char *argv[]) {

    GB gb;


    if (argc < 2) {
        printf("Invalid input\n");
        return -1;
    }

    init_gb(&gb, argv[1]);
    free_gb(&gb);

    return 0;
}