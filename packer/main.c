#include "packer.h"

int main()
{
    FILE* c_file = fopen("build/resource.c", "w");
    FILE* h_file = fopen("build/resource.h", "w");

    if(c_file == NULL || h_file == NULL) {
        printf("Error while opening target files!\n");
        return 0;
    }

    fprintf(h_file, "#pragma once\n\n");
    fprintf(h_file, "#include <stdint.h>\n");
    fprintf(h_file, "#include <stdlib.h>\n\n");

    fprintf(c_file, "#include \"resource.h\"\n\n");

    pack("res/death.png", "halalfej_bytes", c_file, h_file);
    pack("res/logo.png", "logo_bytes", c_file, h_file);
    pack("res/font_hun.ttf", "font_bytes", c_file, h_file);
    pack("res/font_bold_hun.ttf", "font_bold_bytes", c_file, h_file);

    fclose(c_file);
    fclose(h_file);

    return 0;
}
