
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "problem_mk.h"
#include "problem_as.h"
#include "rand_cmd.h"


int main(int argc, char **argv) {

#ifdef ENC_DEC
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
            execute_program(data.data_type.files.in_file);
        }
        else if (data.dec_type == ENCODER) {
            exec_file_enc(data);
        }
    }

#endif


    int cmd_cnt = 100;
    int i = 0;
    unsigned cmd = 0;

    struct reg_state_t regs[RLAST] = {0};
    //printf("%d ", regs[0].init);

    print_rand_cmd(stdout, cmd, regs, cmd_cnt);


    return 0;
}