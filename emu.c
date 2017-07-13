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

unsigned int iptr;

void print_state() {
    printf("reg A = %d\n", reg[0]);
    printf("reg B = %d\n", reg[1]);
    printf("reg C = %d\n", reg[2]);
    printf("reg D = %d\n", reg[3]);
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
    int rx = reg[rn];
    switch (op) {
        case 0: 
            reg[0] = mem[rx];
            break;
        case 1:
            mem[rx] = reg[0];
            break;
        case 2:
            reg[0] = rx;
            break;
        case 3:
            reg[rn] = reg[0];
            break;
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

int main() {
    init();
    print_state();
    prgm[0] = 0x3 | (1 << 7);
    prgm[1] = 1;
    prgm[2] = 0xff;
    while(iptr != 0xff)
    {
        uint8_t inst = prgm[iptr];
        if (BIT(inst, 7)) // ld x;
        {
            reg[0] = 0x7f & inst;
            iptr++;
        }
        else if (OP(inst) == 0) // arithmetic operation
        {
            process_arithmetic(inst);
            iptr++;
        }
        else if (OP(inst) == 1) // memory operation
        {
            process_memory(inst);
            iptr++;
        }
        else if (OP(inst) == 2) // jump operation
            process_jump(inst);
        else {
            printf("INVALID INSTRUCTION #%d.\n", iptr);
            iptr++;
        }
    }
    print_state();
}

