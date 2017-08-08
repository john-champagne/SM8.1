#ifndef JUMP_H
#define JUMP_H

typedef struct jump_pair {
    char* key;
    int   pointer;
} jump_pair_t;

extern jump_pair_t* jump_list;
extern int jump_list_size;

void initialize_jump_list();
void free_jump_pair(jump_pair_t j);
void free_jump_list();
int add_jump(char* key, int pointer);
int get_jump_pointer(char* key);
int is_jump_instruction(char* line);
char* get_key_from_line(char* line);
int get_pointer_from_line(char* line);

#endif
