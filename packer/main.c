#include <stddef.h>
#include <stdio.h>
#include "packer.h"

int process(FILE* c_file, FILE* h_file, int argc, const char* argv[]);

int main(int argc, const char* argv[])
{
    FILE* c_file = fopen("resource.c", "w");
    FILE* h_file = fopen("resource.h", "w");

    int ret = process(c_file, h_file, argc, argv);

    fclose(c_file);
    fclose(h_file);

    return ret;
}

int process(FILE* c_file, FILE* h_file, int argc, const char* argv[])
{
    if(c_file == NULL || h_file == NULL) {
        printf("Error while opening target files!\n");
        return 1;
    }

    if(argc != 5) {
        printf("This function waits for 5 args instead of %d!\n", argc);
        return 1;
    }

    fprintf(h_file, "#pragma once\n\n");
    fprintf(h_file, "#include <stdint.h>\n");
    fprintf(h_file, "#include <stdlib.h>\n\n");

    fprintf(c_file, "#include \"resource.h\"\n\n");

    int ret = 0;

    ret |= pack(argv[1], "halalfej_bytes", c_file, h_file);
    ret |= pack(argv[2], "logo_bytes", c_file, h_file);
    ret |= pack(argv[3], "font_bytes", c_file, h_file);
    ret |= pack(argv[4], "font_bold_bytes", c_file, h_file);

    return ret;
}