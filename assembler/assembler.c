#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"
#include "processor.h"

void print_usage() {
    printf("Usage: asm [filename] [output name]\n");
}

int main(int argc, char** argv) {
    if (argc <= 2) {
        print_usage();
        return 0;
    }
    char str[1024];
    char* preprocessed_lines[1024];
    int maxindex;
    FILE* infile = fopen(argv[1], "r");
    if (infile != NULL) {
        int i = 0;
        while (fgets(str, 1024, infile) != NULL) {
            char* preproc = preprocess_line(str);
            if (strlen(preproc) != 0) {
                preprocessed_lines[i] = preproc;
                printf("%s\n", preprocessed_lines[i]);
                i++;
            }
        }
        maxindex = i;
    }
    int i;
    for (i = 0; i < maxindex; i++) {
        int inst = assemble(preprocessed_lines[i]);
        printf("[%s] -> 0x%02x\n", preprocessed_lines[i], inst & 0xff);
    }
}
