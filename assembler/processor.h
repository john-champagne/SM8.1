#ifndef PROCESSOR_H
#define PROCESSOR_H

// string manipulation functions
int string_match(char*, char*);
char* next_word(char*);

int get_register(char*);
int get_instruction(char*);

int assemble_mov(char*);
int assemble(char*);

#endif
