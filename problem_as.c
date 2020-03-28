#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "problem_as.h"
#include "problem_mk.h"

extern const int MAX_LEN = 20;

struct in_data_t get_data(int argc, char **argv) { // acquiring data

    assert(argc > 0);
    assert(argv);

    struct in_data_t* data = (struct in_data_t*) calloc(1, sizeof(struct in_data_t));
    assert(data);

    if (argc == 3) {// single commands

        data->comm_file = COMMAND;

        if (strcmp(argv[1], "-d") == 0) {
            data->dec_type = DECODER;
            data->data_type.cmd.cmd_bin = *argv[2];
        }

        else if (strcmp(argv[1], "-c") == 0) {
            data->dec_type = ENCODER;
            data->data_type.cmd.cmd_asm = argv[2];
        }
        else {
            fprintf(stderr, "Command not recognized, aborting\n");
            abort();
        }
    } else if (argc == 6) { // managing files

        data->comm_file = FILE_T;

        if (strcmp(argv[1], "-d") == 0) {

            data->dec_type = DECODER;

            if (strcmp(argv[2], "-f" )!= 0) {
                fprintf(stderr, "No matching symbol -f for file, aborting\n");
                abort();
            }

            assert(argv[3]);
            data->data_type.files.in_file = fopen(argv[3], "r");
            assert(data->data_type.files.in_file != NULL);

            if (strcmp(argv[4], "-o" )!= 0) {
                fprintf(stderr, "No matching symbol -o for file, aborting\n");
                abort();
            }

            assert(argv[5]);
            data->data_type.files.out_file = fopen(argv[5], "w");
            assert(data->data_type.files.out_file != NULL);

        }

        else if (strcmp(argv[1], "-c") == 0) {

            data->dec_type = ENCODER;

            if (strcmp(argv[2], "-f" )!= 0) {
                fprintf(stderr, "No matching symbol -f for file, aborting\n");
                abort();
            }

            assert(argv[3]);
            data->data_type.files.in_file = fopen(argv[3], "r");
            assert(data->data_type.files.in_file != NULL);

            if (strcmp(argv[4], "-o" )!= 0) {
                fprintf(stderr, "No matching symbol -o for file, aborting\n");
                abort();
            }

            assert(argv[5]);
            data->data_type.files.out_file = fopen(argv[5], "w");
            assert(data->data_type.files.out_file != NULL);


        }
    } else {
        fprintf(stderr, "Arguments reading error, aborting");
        abort();
    }

    return *data;
}

void exec_command_dec(struct instr_t instr, FILE* stream) { // executing single command (instr)

    struct reg_state_t regs[RLAST] = {0};

    char reg1 = 0;
    char reg2 = 0;
    char reg3 = 0;

    reg1 = instr.opnd.ops.rs + 'A';
    reg2 = instr.opnd.ops.rd + 'A';
    reg3 = instr.opnd.rop + 'A';

    switch (instr.opcode) {

        case MOVI: {
            fprintf(stream, "MOVI %d\n", instr.opnd.imm);
            break;
        }

        case ADD: {
            fprintf(stream, "ADD %c, %c\n", reg1, reg2);
            break;
        }

        case SUB: {
            fprintf(stream, "SUB %c, %c\n", reg1, reg2);
            break;
        }
        case MUL: {
            fprintf(stream, "MUL %c, %c\n", reg1, reg2);
            break;
        }
        case DIV: {
            fprintf(stream, "DIV %c, %c\n", reg1, reg2);
            break;
        }

        case IN: {
            fprintf(stream, "IN %c\n", reg3);
            break;
        }

        case OUT: {
            fprintf(stream, "OUT %c\n", reg3);
            break;
        }

        default: {
            fprintf(stderr, "Decoding opcode error, aborting\n");
            abort();
        }
    }

    return;
}

void exec_command_enc(struct instr_t instr, FILE* stream) {

    unsigned res = 0;

    switch(instr.opcode) {

        case MOVI: {
            res = MOVE_CODE | instr.opnd.imm;
            fprintf(stream, "0x%x ", res);
            break;
        }

        case ADD: {
            res = ADD_CODE | (instr.opnd.ops.rd << 2) | (instr.opnd.ops.rs);
            fprintf(stream, "0x%x ", res);
            break;
        }

        case SUB: {
            res = SUB_CODE | (instr.opnd.ops.rd << 2) | (instr.opnd.ops.rs);
            fprintf(stream, "0x%x ", res);
            break;
        }

        case MUL: {
            res = MUL_CODE | (instr.opnd.ops.rd << 2) | (instr.opnd.ops.rs);
            fprintf(stream, "0x%x ", res);
            break;
        }

        case DIV: {
            res = DIV_CODE | (instr.opnd.ops.rd << 2) | (instr.opnd.ops.rs);
            fprintf(stream, "0x%x ", res);
            break;
        }

        case IN: {
            res = IN_CODE | instr.opnd.rop;
            fprintf(stream, "0x%x ", res);
            break;
        }

        case OUT: {
            res = OUT_CODE | instr.opnd.rop;
            fprintf(stream, "0x%x ", res);
            break;
        }

        default: {
            fprintf(stderr, "No matching command found, aborting");
            abort();
        }

    }


}



void exec_file_dec(struct in_data_t data) {

    unsigned cmd = 0;

    while (fscanf(data.data_type.files.in_file, "%x", &cmd) == 1) {

        struct instr_t instr = {0};
        unsigned char command = cmd & COMMANDMASK;
        assert(cmd == command);
        instr = decode_instr(cmd);
        exec_command_dec(instr, data.data_type.files.out_file);
    }

    return;
}

void get_arith(int cmd_type, struct instr_t* instr, char const *beginning, char* p) {

    instr->opcode = cmd_type;
    p++;
    beginning = p;
    p++;
    *p = '\0';

    int reg = *beginning - 'A';
    instr->opnd.ops.rd = reg;

    p += ARITH_SHIFT;
    p++;
    beginning = p;
    p++;
    *p = '\0';

    reg = *beginning - 'A';

    instr->opnd.ops.rs = reg;

    return;
}

struct instr_t decode_instr_asm(char* cmd_asm) {

    assert(cmd_asm);

    struct instr_t* instr = (struct instr_t*) calloc(1, sizeof(struct instr_t));
    assert(instr);

    int cmd_len = strlen(cmd_asm);
    char* beginning = cmd_asm;
    char* p = cmd_asm;

    while (*p != ' ') { //opcode
        ++p;
    }
    *p = '\0';

    if (strcmp(beginning, "MOVI") == 0) {
        instr->opcode = MOVI;
        p += MOVE_SHIFT;
        instr->opnd.imm = atoi(p);
    }
    else if (strcmp(beginning, "ADD") == 0) { //get_arith()
      get_arith(ADD, instr, beginning, p);
    }
    else if (strcmp(beginning, "SUB") == 0) {
        get_arith(SUB, instr, beginning, p);
    }

    if (strcmp(beginning, "MUL") == 0) {
        get_arith(MUL, instr, beginning, p);
    }

    if (strcmp(beginning, "DIV") == 0) {
        get_arith(DIV, instr, beginning, p);
    }

    if (strcmp(beginning, "IN") == 0) {
        instr->opcode = IN;
        p += IN_SHIFT;
        beginning = p;

        int reg = *beginning - 'A';

        instr->opnd.rop = reg;
    }

    if (strcmp(beginning, "OUT") == 0) {
        instr->opcode = OUT;
        p += OUT_SHIFT;
        beginning = p;

        int reg = *beginning - 'A';

        instr->opnd.rop = reg;
    }

    return *instr;
}

void exec_file_enc(struct in_data_t data) {

    char* command = (char*) calloc(MAX_LEN, sizeof(char));
    assert(command);



    while (fgets(command, MAX_LEN, data.data_type.files.in_file) != NULL) {

        int len = strlen(command);

        command[len - 1] = '\0';
        command[len - 2] = '\0'; // ONLY TO SKIP /r
        struct instr_t instr = decode_instr_asm(command);
        exec_command_enc(instr, data.data_type.files.out_file);

    }

    free(command);

}

