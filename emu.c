#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Gets a single bit (y) from the variable (x).
#define BIT(x,y) ((0x1 << y) & x) != 0

// Extracts the op code (bits 6,5) from the instruction.
#define OP(x) 0x3 & (x >> 5)

int8_t mem[256];
uint8_t prgm[256];
int8_t reg[4];
int uload = -1;

unsigned int iptr;

// prints the values in the registers
void print_register_state() {
    printf("A: 0x%02x\n", reg[0]);
    printf("B: 0x%02x\n", reg[1]);
    printf("C: 0x%02x\n", reg[2]);
    printf("D: 0x%02x\n", reg[3]);
}

// prints the memory in a readable fashion.
void print_memory_state() {
    int i;
    for (i = 0; i < 256; i++)
        printf("0x%02x: %02x\n", i, 0xff & mem[i]);
}

// reads a program file into the prgm array
int read_program(char* filename) {
    FILE* in_file = fopen(filename, "rb");
    int i;
    if (in_file == NULL)
    {
        printf("ERROR opening program file. Exiting...\n");
        return 0;
    }
    for (i = 0; i < 256; i++){
        int c = getc(in_file);
        if (c == EOF)
            break;
        prgm[i] = c;
    }
    return 1; 
}

void init() {
    memset(mem, 0, 256);
    memset(prgm, 0, 256);
    iptr = 0;
}

void process_arithmetic(uint8_t inst){
    int op = 0x7 & (inst >> 2);
    int rx = reg[0x3 & inst];
    switch (op) {
        case 0:
            reg[0] = reg[0] + rx;
            break;
        case 1:
            reg[0] = reg[0] - rx;
            break;
        case 2:
            reg[0] = reg[0] | rx;
            break;
        case 3:
            reg[0] = reg[0] & rx;
            break;
        case 4:
            reg[0] = reg[0] ^ rx;
            break;
        case 5:
            reg[0] = reg[0] >> rx;
            break;
        case 6:
            reg[0] = reg[0] << rx;
            break;
    }
}

void process_memory(inst) {
    int op = 0x3 & (inst >> 2);
    int rn = 0x3 & inst;
    if (BIT(inst,4)) {      // checks for a mov instruction
        int ra = (0x3 << 2) & inst;
        reg[rn] = reg[ra];
    }
    else {
        int rx = reg[rn];
        switch (op) {
            case 0: 
                reg[0] = mem[rx];
                break;
            case 1:
                mem[rx] = reg[0];
                break;
        }
    }
}

void process_jump(uint8_t inst) {
    int op = 0x3 & (inst >> 2);
    int rx = reg[0x3 & inst];
    int au = (uint8_t)reg[0];
    switch (op) {
        case 0:
            iptr = (reg[0] > 0) ? rx : iptr + 1;
            break;
        case 1:
            iptr = (reg[0] >= 0) ? rx : iptr + 1;
            break;
        case 2:
            iptr = (reg[0] == 0) ? rx : iptr + 1;
            break;
        case 3:
            iptr = (reg[0] < 0) ? rx : iptr + 1;
            break;
        case 4:
            iptr = (reg[0] <= 0) ? rx : iptr + 1;
            break;
        case 5:
            iptr = (au > 0) ? rx : iptr + 1;
            break;
        case 6:
            iptr = (au >= 0) ? rx : iptr + 1;
            break;
        case 7:
            iptr = rx;
            break;
    }
}

void process_misc(uint8_t inst)
{
    int op = 0x3 & (inst >> 2);
    int rx = 0x3 & inst;
    if (op == 0)
        uload = rx;
}
int main(int argc, char** argv) {
    init();
    int file_read_successful = 1;
    if (argc > 1)
        file_read_successful = read_program(argv[1]);
    if (file_read_successful)
    {
        while(prgm[iptr] != 0x2c)   // program has hard stop at 
        {                           // 0x2c instruction
            uint8_t inst = prgm[iptr];
            if (uload != -1) {
                reg[uload] = inst;
                uload = -1;
            }
            else if (BIT(inst, 7))       // load operation
                reg[0] = 0x7f & inst;
            else if (OP(inst) == 0) // arithmetic operation
                process_arithmetic(inst);
            else if (OP(inst) == 1) // memory operation
                process_memory(inst);
            else if (OP(inst) == 2) // jump operation
            {
                process_jump(inst);
                continue;   // the loop is started from the top to avoid
            }               // the IP increment.
            else
                process_misc(inst);
            iptr++;
        }
        print_register_state();
        print_memory_state();
    }
}

