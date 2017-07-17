// run gcc with flag -D TEST to compile a test case.
#ifdef TEST
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>

// allocates a string.
char* alloc_string(char* str) {
    char* out = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(out, str);
    return out;
}

// processes a line, removing whitespace and comments.
char* preprocess_line(char* line) {
    char out[1024];
    int len = strlen(line);
    int i, j = 0;
    int whitespace_running = 0;
    for (i = 0; i < len; i++)
    {
        if (isspace(line[i])){  // If a character is whitespace.
            if (i == 0)
                continue;   // If its the first character, don't place it.
            if (whitespace_running) 
                continue;   // If the whitespace is running 
                            // (i.e. 2 spaces in a row) don't place it
            whitespace_running = 1;
        }
        else
            whitespace_running = 0;
        if (line[i] == ';') // If you find a comment, end the loop.
            break;
        out[j] = line[i];
        j++;
    }
    if (isspace(out[j-1]))  // Check for and remove any whitespace
        out[j-1] = '\0';    // at the end of the line.
    else
        out[j] = '\0';
    return alloc_string(out);
}

#ifdef TEST

int main() {
    char* s = "; mov A ; move this ";
    char* processed = preprocess_line(s);
    printf("Unprocessed:\n%s\nProcessed:\n%s", s, processed);
}

#endif




