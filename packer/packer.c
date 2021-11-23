#include "packer.h"
#include <stdint.h>

const size_t COL_NUM = 16;

void pack(const char* src_path, const char* src_name, FILE* c_file, FILE* h_file)
{
    if(!(src_path || src_name || c_file || h_file)) {
        printf("Nullpointer error!\n");
        return;
    }

    fprintf(h_file, "extern const uint8_t %s[];\n", src_name);
    fprintf(h_file, "extern const size_t %s_size;\n\n", src_name);

    FILE* src_file = fopen(src_path, "rb");

    if(src_file == NULL) {
        printf("Error while opening source file \"%s\"!\n", src_path);
        return;
    }

    uint8_t buf[1];
    size_t size = 0;
    size_t col = 0;

    fprintf(c_file, "const uint8_t %s[] = {\n", src_name);

    while(fread(buf, 1, 1, src_file)) {
        if(col == 0) {
            fprintf(c_file, "    ");
        }

        fprintf(c_file, "0x%02X, ", buf[0]);

        if(col == COL_NUM - 1) {
            fprintf(c_file, "\n");
        }

        col = (col + 1) % COL_NUM;
        size++;
    }

    if(col != 0) {
        fprintf(c_file, "\n");
    }
    fprintf(c_file, "};\n\n");
    fprintf(c_file, "const size_t %s_size = %lu;\n\n", src_name, size);

    fclose(src_file);
}
