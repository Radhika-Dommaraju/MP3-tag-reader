#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"file.h"
#include"menu.h"

int main(int argc, char *argv[])
{
    if (argc <= 2) 
    {
        menu();
        return 1;
    }

    if (!strcmp(argv[1], "-h")) 
    {
        menu();
        return 1;
    }

    FILE *ptr = fopen("sample.mp3", "r");
    if (ptr == NULL) {
        printf("Error: Could not open file 'sample.mp3'\n");
        return 1;
    }

    if (!strcmp(argv[1], "-v") && !strcmp(argv[2], "sample.mp3")) {
        view(ptr);
        return 0;
    }

    if (!strcmp(argv[1], "-e")) 
    {
        if (argc < 5) 
        {
            menu();
            return 1;
        }

        char flag[6] = { 't', 'a', 'A', 'y', 'g', 'c' };
        char ch = argv[2][1];

        // ✅ Check for valid tag flag
        if (strchr(flag, ch) == NULL) 
        {
            printf("Error: Invalid flag '%c'\n", ch);
            menu();
            return 1;
        }

        if (!strcmp(argv[3], "sample.mp3")) 
        {
            edit(ptr, argv[4], ch);
            return 0;
        } else {
            printf("Error: Expected 'sample.mp3' as argument\n");
            menu();
            return 1;
        }
    }

    menu();
    return 1;
}
