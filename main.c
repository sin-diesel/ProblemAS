
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "problem_mk.h"
#include "problem_as.h"



int main(int argc, char **argv) {

    struct in_data_t data = get_data(argc, argv);

    if (data.comm_file == COMMAND) {
        if (data.dec_type == DECODER) {
            struct instr_t instr = decode_instr(data.data_type.cmd.cmd_bin);
            exec_command_dec(instr, stdout);
        } else if (data.dec_type == ENCODER) {
            struct instr_t instr = decode_instr_asm(data.data_type.cmd.cmd_asm);
            exec_command_enc(instr, stdout);
        }
    } else if (data.comm_file == FILE_T) {
        if (data.dec_type == DECODER) {
            exec_file_dec(data);
        }
        else if (data.dec_type == ENCODER) {
            exec_file_enc(data);
        }
    }


    return 0;
}
