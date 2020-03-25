#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "problem_mk.h"

unsigned char exec_binop(enum opcode_t opc, unsigned char rd, unsigned char rs) {

    switch(opc) {
        case ADD: return rd + rs;
        case SUB: return rd - rs;
        case MUL: return rd * rs;
        case DIV:  {
            if (rs == 0) {
                fprintf(stderr, "Division by 0, error\n");
                abort();
            } else {
                return rd / rs;
            }
        }
        default:
            fprintf(stderr, "Unexpected error\n");
            abort();
    }
}

void execute_instr(struct reg_state_t *st, struct instr_t* instr) {

    int rd = instr->opnd.ops.rd;
    int rs = instr->opnd.ops.rs;

    switch(instr->opcode) {

        case MOVI: {
            st[D].data = instr->opnd.imm;
            break;
        }

        case ADD: {
            st[rd].data = exec_binop(instr->opcode, st[rd].data, st[rs].data);
            break;
        }
        case SUB: {
            st[rd].data = exec_binop(instr->opcode, st[rd].data, st[rs].data);
            break;
        }
        case MUL: {
            st[rd].data = exec_binop(instr->opcode, st[rd].data, st[rs].data);
            break;
        }
        case DIV: {
            st[rd].data = exec_binop(instr->opcode, st[rd].data, st[rs].data);
            break;
        }

        case IN: {
            int d = 0;
            int res = 0;
            res = scanf("%d", &d);
            assert(res == 1);
            st[instr->opnd.rop].data = d & 0xFF; // placing data in register

            break;
        }

        case OUT: {
            fprintf(stdout, "%d ", st[instr->opnd.rop].data);
            break;
        }
        default: {
            fprintf(stderr, "Execution Opcode error: %x\n", instr->opcode);
            abort();
        }
    }
}


void decode_arith(struct instr_t* instr, unsigned char cmd) {

    instr->opnd.ops.rs = (cmd >> RSHIFT) & RMASK;
    instr->opnd.ops.rd = (cmd >> RDSHIFT) & RMASK;
    instr->opcode = (cmd >> OPSCHIFT);

    if (instr->opcode > DIV) {
        fprintf(stderr, "Decode OPcode error");
        abort();
    }

    return;
}

struct instr_t decode_instr(unsigned char cmd) {

    struct instr_t instr = {0};

    //may be mov
    if ((cmd & MOVMASK) == 0) {
        instr.opcode = MOVI;
        instr.opnd.imm = cmd;
    }
        // in or out
    else if ((cmd & INOUTMASK) == INOUTMASK) {
        if ((cmd >> IOSHIFT) == INOPC) {
            instr.opcode = IN;
        } else {
            instr.opcode = OUT;
        }
        instr.opnd.rop = cmd & INOUTOP;
    } else {
        // arithmetics
        decode_arith(&instr, cmd);
    }

    return instr;
}

void execute_program(FILE *prog) {

    unsigned command = 0;
    struct reg_state_t regs[RLAST] = {0};

    while (fscanf(prog, "%x", &command) == 1) {

        struct instr_t instr = {0};
        unsigned char cmd = command & COMMANDMASK;
        assert(cmd == command);
        instr = decode_instr(cmd);
        execute_instr(regs, &instr);

    }

    return;
}