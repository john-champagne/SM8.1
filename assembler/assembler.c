#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"
#include "processor.h"
#include "jump.h"

jump_pair_t* jump_list;
int jump_list_size;

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
                //printf("%s\n", preprocessed_lines[i]);
                i++;
            }
        }
        maxindex = i;
    }
    int i=0, l=0;
    initialize_jump_list();
    for (i = 0; i < maxindex; i++) {
        if (is_jump_instruction(preprocessed_lines[i])) {
            char* key = get_key_from_line(preprocessed_lines[i]);
            int pointer = l;
            printf("adding key %s at pos %d\n", key, pointer);
            add_jump(key,pointer);
            continue;
        }
        int inst = assemble(preprocessed_lines[i]);
        printf("[%s] -> 0x%02x\n", preprocessed_lines[i], inst & 0xff);
        l++;
    }
    free_jump_list();
}
