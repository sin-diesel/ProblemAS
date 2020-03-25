// problem mk common types

#pragma once

enum {
    MOVMASK = (1 << 7),
    INOUTMASK = (1 << 6),
    IOSHIFT = 2,
    INOPC = 0x30,
    OUTPC = 0x31,
    INOUTOP = 0x3,
    RDSHIFT = 0,
    RSHIFT = 2,
    RMASK = 0x3,
    OPSCHIFT = 4,
    COMMANDMASK = 0xFF
};

enum reg_t {
    A = 0, B, C, D, RLAST
};

enum opcode_t {
    MOVI = 0, ADD = 8, SUB = 9, MUL = 10, DIV, IN, OUT, OPLAST
};

union operand_t {
    struct { int rd, rs; } ops; // pair of registers: destination and source
    int rop; // single register
    int imm; // move I
};

struct instr_t {
    enum opcode_t opcode;
    union operand_t opnd;
};

struct reg_state_t {
    unsigned char data;
    unsigned char init;
};

unsigned char exec_binop(enum opcode_t opc, unsigned char rd, unsigned char rs);

void execute_instr(struct reg_state_t *st, struct instr_t* instr);

void decode_arith(struct instr_t* instr, unsigned char cmd);

struct instr_t decode_instr(unsigned char cmd);

void execute_program(FILE *prog);

