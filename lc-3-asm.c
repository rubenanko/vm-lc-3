#include <stdio.h>

int main(int argc, char ** argv)
{
    if(argc > 1)
    {
        FILE * source = fopen(argv[1], "r");
        return 0;
    }
}