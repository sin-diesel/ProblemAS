// problem as common types

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "problem_mk.h"

extern const int MAX_LEN;

enum cmd_codes_t {
    MOVE_CODE = (0 << 7),
    ADD_CODE = (1 << 7),
    SUB_CODE = 0x90,
    MUL_CODE = 0xA0,
    DIV_CODE = 0xB0,
    IN_CODE = 0xC0,
    OUT_CODE = 0xC4
};

enum shifts_t {
    MOVE_SHIFT = 1,
    ARITH_SHIFT = 1,
    IN_SHIFT = 1,
    OUT_SHIFT = 1,
};

enum cmd_bin_t {
    MOVE_COM = 0,
    ADD_COM = 1000,
    SUB_COM = 1001,
    MUL_COM = 1010,
    DIV_COM = 1011,
    IN_COM = 110000,
    OUT_COM = 110001,
};

enum data_types_t {
    DECODER = 1,
    ENCODER = 2,
    COMMAND = 11,
    FILE_T = 12
};

union cmd_t {
    unsigned cmd_bin;
    char* cmd_asm;
};

struct files_t {
    FILE* in_file;
    FILE* out_file;
};

union data_type_t {
    union cmd_t cmd;
    struct files_t files;
};

struct in_data_t {
    int dec_type;
    int comm_file;
    union data_type_t data_type;
};



struct in_data_t get_data(int argc, char **argv);

void exec_command_dec(struct instr_t instr, FILE* stream); // executing single command (instr)

void exec_command_enc(struct instr_t instr, FILE* stream);

struct instr_t decode_instr_asm(char* cmd_asm);

void exec_file_enc(struct in_data_t data);

void exec_file_dec(struct in_data_t data);

void get_arith(int cmd_type, struct instr_t* instr, char const *beginning, char* p);

void free_all();