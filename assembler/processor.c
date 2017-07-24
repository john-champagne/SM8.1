#ifdef TEST
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "processor.h"
#include "constant.h"

#define KEYWORDS 17

// A list of keywords with simple syntax
// (i.e. a keyword followed by a register)
// mov, ld, and throw are handled differently.
char* keywords[KEYWORDS] = {"add", "sub", "or", "and",
                            "xor", "rsh", "lsh",
                            "rd", "wr", "jgeu",
                            "jge", "jgu", "jg", "je", "jle",
                            "jl", "jmp", "uld"};

// table of opcodes that corespond to the simple keywords
// in the table above.
// e.g. keyword[7] = rd, keyword_opcode[7] = 001 000 00
uint8_t keyword_opcode[KEYWORDS] = {    0x00, 0x04, 0x08,
                                        0x0c, 0x10, 0x14,
                                        0x18, 0x20, 0x24,
                                        0x58, 0x44, 0x54,
                                        0x40, 0x48, 0x50,
                                        0x4c, 0x5c, 0x60 };

// checks if the string 'key' is at the current
// position in source.
int string_match(char* source, char* key){
    int i;
    int key_len = strlen(key);
    int src_len = strlen(source);
    if (key_len > src_len)
        return 0;
    for (i = 0; i < key_len; i++)
        if (*(key + i) != *(source + i))
            return 0;
    return 1;
}

// returns a pointer to the position immediately following the first
// whitespace.
// e.g. "mov A,B" -> "A,B"      "hello world" -> "world"
char* next_word(char* line) {
    int i = 0;
    int len = strlen(line);
    while (i < len && !isspace(line[i])) // read until you hit whitespace.
        i++;
    while (i < len && isspace(line[i]))  // read until you get out of
        i++;                             // whitespace.
    return (line + i);
}

// returns the index of the instruction on the line.
// if there is no instruction, return -1.
int get_instruction(char* line) {
    int i;
    for (i = 0; i < KEYWORDS; i++)
        if (string_match(line, keywords[i]))
            return i;
    return -1;
}

// returns the number of the register on the line.
// e.g. "rd A" -> 00, "wr C" -> 10 etc.
int get_register(char* line) {
    char* reg = next_word(line);
    if (strlen(reg) != 0)
        return reg[0] - 'A';
    return -1;
}

// assembles a line with a mov instruction on it.
int assemble_mov(char* line){
    char* regs = next_word(line);
    int lreg, rreg;
    if (strlen(regs) >= 3 && regs[1] == ',') {
        lreg = regs[0] - 'A';
        rreg = regs[2] - 'A';
    }
    else
        return -1;
    if (lreg == 0)
        return 0x2c | rreg;
    else if (rreg == 0)
        return 0x28 | lreg;
    else
        return -1;
}

// assembles a line of code into machine code.
int assemble(char* line) {
    int instruction = get_instruction(line);
    if (instruction == -1) {
       if (string_match(line, "mov"))
            return assemble_mov(line);
    }
    int reg = get_register(line);
    if (reg != -1 && instruction != -1)
        return keyword_opcode[instruction] | reg;
}

#ifdef TEST

int main(){
    printf("0x%02x", assemble("mov D,A"));
}        

#endif 
