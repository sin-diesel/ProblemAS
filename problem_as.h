// problem as common types

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "problem_mk.h"

extern const int MAX_LEN;

enum {
    MOVE_SHIFT = 4,
    ADD_SHIFT = 3,
    SUB_SHIFT = 3,
    MUL_SHIFT = 3,
    DIV_SHIFT = 3,
    IN_SHIFT = 3,
    OUT_SHIFT = 2,
};

enum {
    MOVE_COM = 0,
    ADD_COM = 1000,
    SUB_COM = 1001,
    MUL_COM = 1010,
    DIV_COM = 1011,
    IN_COM = 110000,
    OUT_COM = 110001,
};

enum {
    DECODER = 1,
    ENCODER = 2,
    COMMAND = 11,
    FILE_T = 12
};

union cmd_t {
    int cmd_bin;
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



char *strrev(char *str); // str reverse function


int get_bin_comm(char* command); // conversion of binary num in string representation to int representation


char * int_to_bin(int i);

struct in_data_t get_data(int argc, char **argv);

void exec_command_dec(struct instr_t instr, FILE* stream); // executing single command (instr)

void exec_command_enc(struct instr_t instr, FILE* stream);

struct instr_t decode_instr_asm(char* cmd_asm);

void exec_file_enc(struct in_data_t data);

void exec_file_dec(struct in_data_t data);