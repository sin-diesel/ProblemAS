// problem as common types

#pragma once

#include <stdlib.h>
#include <stdio.h>

const int MAX_LEN = 20;

enum {
    MOVE_SHIFT = 3,
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