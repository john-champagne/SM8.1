#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"
#include "processor.h"
#include "jump.h"

#define VERBOSE 0

jump_pair_t* jump_list;
int jump_list_size;

char* preprocessed_lines[1024];
int lines_length;

void print_usage() {
    printf("Usage: asm [input filename] [output filename]\n");
}

// reads a file, preprocesses it, and stores it in the array
// returns the length of the array, or -1 on failure.
int populate_preprocessed_lines(char* filename) {
    FILE* infile = fopen(filename, "r");
    char str[1024];
    if (infile == NULL)
        return -1;
    int i = 0;
    while (fgets(str, 1024, infile) != NULL) {
        char* preproc = preprocess_line(str);
        if (strlen(preproc) != 0) {
            preprocessed_lines[i] = preproc;
            i++;
        }
    }
    return i;
}

// assembles the preprocessed lines and outputs them to a file.
// if verbose is nonzero, prints debug information to the console.
int assemble_and_output(char* filename, int verbose) {
    FILE* outfile = fopen(filename, "w+");
    if (outfile == NULL){
        return -1; printf("JJJJ");}
    int i=0, l=0;
    initialize_jump_list();
    for (i = 0; i < lines_length; i++) {
        if (is_jump_instruction(preprocessed_lines[i])) {
            char* key = get_key_from_line(preprocessed_lines[i]);
            int pointer = l;
            if (verbose)
                printf("adding key %s at pos %d\n", key, pointer);
            add_jump(key,pointer);
            continue;
        }
        int inst = assemble(preprocessed_lines[i]);
        if (verbose)
            printf("[%s] -> 0x%02x\n", preprocessed_lines[i], inst & 0xff);
        fputc((char)(inst & 0xff), outfile);
        l++;
    }
    free_jump_list();
    fclose(outfile);
}

int main(int argc, char** argv) {
    if (argc <= 2) {
        print_usage();
        return 0;
    }
    lines_length = populate_preprocessed_lines(argv[1]); // preprocess
    if (lines_length != -1) {
        assemble_and_output(argv[2], VERBOSE);           // assemble
    } else {
        printf("ERROR: failed to open file %s\n", argv[1]);
    }
}
