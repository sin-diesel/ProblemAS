#pragma once

#include <stdlib.h>

enum {
    MAX_CMD = 0xFF,
    MIN_CMD = 0,
};


unsigned check_instr(struct instr_t* instr, struct reg_state_t* regs);

unsigned generate_random(int l, int r); //this will generate random number in range l and r

void print_rand_cmd(FILE* stream, unsigned char cmd, struct reg_state_t* regs, unsigned cmd_cnt);