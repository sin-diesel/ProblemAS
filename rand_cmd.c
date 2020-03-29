#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "problem_as.h"
#include "problem_mk.h"
#include "rand_cmd.h"

unsigned check_instr(struct instr_t* instr, struct reg_state_t* regs) {
    //check division by zero
    if (instr->opcode == DIV && regs[instr->opnd.ops.rs].data == 0) {
        return 0;
    }

    //check printing uninitialized reg
    if (instr->opcode == OUT && regs[instr->opnd.rop].init == 0) {
        return 0;
    }

    if ((instr->opcode == ADD || instr->opcode == SUB || instr->opcode == MUL || instr->opcode == DIV) && ((regs[instr->opnd.ops.rd].init == 0) || (regs[instr->opnd.ops.rs].init) == 0)) {
        return 0;
    }



    return 1;
}

unsigned generate_random(int l, int r) {//this will generate random number in range l and r

    unsigned rand_num = 0;

    rand_num = (rand() % (r - l + 1)) + l;

    return rand_num;
}

void print_rand_cmd(FILE* stream, unsigned char cmd, struct reg_state_t* regs, unsigned cmd_cnt) {

    assert(regs);
    int i = 0;

    srand(time(0));

    for (i = 0; i < cmd_cnt; ++i) {
        cmd = generate_random(MIN_CMD, MAX_CMD);
        struct instr_t instr = decode_instr(cmd);
        if (check_instr(&instr, regs) == 1) {
            fprintf(stream, "0x%x ", cmd);
            exec_command_dec_wregs(instr, stream, regs);
        }
    }

    fprintf(stream, "\n");

}