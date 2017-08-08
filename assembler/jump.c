#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "jump.h"
#include "preprocessor.h"

jump_pair_t* jump_list;
int jump_list_size;

void initialize_jump_list() {
    jump_list = (jump_pair_t*)malloc(sizeof(jump_pair_t)); // allocate one jump
    jump_list[0].key = alloc_string("start");
    jump_list[0].pointer = 0;
    jump_list_size = 1;
}

void free_jump_pair(jump_pair_t j) {
    free(j.key);
}

void free_jump_list() {
    int i = 0;
    for (; i < jump_list_size; i++)
        free_jump_pair(jump_list[i]);
    free(jump_list);
}

int add_jump(char* key, int pointer) {
    jump_list = realloc(jump_list, sizeof(jump_pair_t)*(jump_list_size + 1));
    if (jump_list) {
        jump_list[jump_list_size].key = key;
        jump_list[jump_list_size].pointer = pointer;
        jump_list_size++;
        return 1;
    }
    return 0;
}

int get_jump_pointer(char* key) {
    int i = 0;
    for (; i < jump_list_size; i++)
        if (strcmp(key, jump_list[i].key) == 0)
            return jump_list[i].pointer;
    return -1;
}

int is_jump_instruction(char* line) {
    int i = 0;
    int len = strlen(line);
    if (line[0] == '_') {
        for (; i < len; i++) {
            if (line[i] == ':')
                return 1;
            if (isspace(line[i]))
                return 0;
        }
    }
    return 0;
}

char* get_key_from_line(char* line) {
    char str[1024];
    int i = 1;
    int len = strlen(line);
    for (; i < len; i++) {
        if (line[i] != ':')
            str[i-1] = line[i];
        else
            break;
    }
    str[i-1] = '\0';
    return alloc_string(str);
}

int get_pointer_from_line(char* line) {
    return get_jump_pointer(get_key_from_line(line));
}

#ifdef TEST
void print_jump_list() {
    int i = 0;
    for (; i < jump_list_size; i++)
        printf("%s = %d\n", jump_list[i].key, jump_list[i].pointer);
}
void print_jump_list() {
    int i = 0;
    for (; i < jump_list_size; i++)
        printf("%s = %d\n", jump_list[i].key, jump_list[i].pointer);
}

int main() {
    initialize_jump_list();
    printf("%s\n", get_key_from_line("_:"));
}
#endif
