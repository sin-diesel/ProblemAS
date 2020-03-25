// problem MK sample solution

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "problem_mk.h"
#include "problem_as.h"

char *strrev(char *str)
{
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}


int get_bin_comm(char* command) {

    int mask = 0x1;
    unsigned char res = 0;



    strrev(command);

    int temp = atoi(command);
    int len = strlen(command);

    for (int i = 0; i < len; ++i) {

        int remainder = temp % 10;
        res += remainder;
        if (i < len - 1) {
            res = res << 1;
        }
        temp = temp / 10;
    }


    return res;
}

struct in_data_t get_data(int argc, char **argv) {

    assert(argc > 0);
    assert(argv);

    struct in_data_t* data = (struct in_data_t*) calloc(1, sizeof(struct in_data_t));
    assert(data);

    if (argc == 3) {// single commands

        data->comm_file = COMMAND;

        if (strcmp(argv[1], "-d") == 0) {
            data->dec_type = DECODER;
            data->data_type.cmd.cmd_bin = get_bin_comm(argv[2]);
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

            if (strcmp(argv[2], "-o" )!= 0) {
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

void exec_command(struct in_data_t data) {

    struct reg_state_t regs[RLAST] = {0};

    if (data.dec_type == DECODER) {

        unsigned char cmd = data.data_type.cmd.cmd_bin;

        struct instr_t instr = decode_instr(cmd);

        char* reg1 = (char*) calloc(5, sizeof(char));
        char* reg2 = (char*) calloc(5, sizeof(char));
        char* reg3 = (char*) calloc(5, sizeof(char));

        switch(instr.opnd.ops.rd) { // determining which regs to write
            case A: {
                strcpy(reg1, "A");
                break;
            }

            case B: {
                strcpy(reg1, "B");
                break;
            }

            case C: {
                strcpy(reg1, "C");
                break;
            }

            case D: {
                strcpy(reg1, "D");
                break;
            }

        }

        switch(instr.opnd.ops.rs) { // determining which regs to write
            case A: {
                strcpy(reg2, "A");
                break;
            }

            case B: {
                strcpy(reg2, "B");
                break;
            }

            case C: {
                strcpy(reg2, "C");
                break;
            }

            case D: {
                strcpy(reg2, "D");
                break;
            }

        }

        switch(instr.opnd.rop) { // determining which regs to write
            case A: {
                strcpy(reg3, "A");
                break;
            }

            case B: {
                strcpy(reg3, "B");
                break;
            }

            case C: {
                strcpy(reg3, "C");
                break;
            }

            case D: {
                strcpy(reg3, "D");
                break;
            }

        }

        switch (instr.opcode) {

            case MOVI: {
                fprintf(stdout, "MOVE D, %x\n", instr.opnd.imm);
                break;
            }

            case ADD: {
                fprintf(stdout, "ADD R%s, R%s\n", reg1, reg2);
                break;
            }

            case SUB: {
                fprintf(stdout, "SUB R%s, R%s\n", reg1, reg2);
                break;
            }
            case MUL: {
                fprintf(stdout, "MUL R%s, R%s\n", reg1, reg2);
                break;
            }
            case DIV: {
                fprintf(stdout, "DIV R%s, R%s\n", reg1, reg2);
                break;
            }

            case IN: {
                fprintf(stdout, "IN %s\n", reg3);
                break;
            }

            case OUT: {
                fprintf(stdout, "OUT %s\n", reg3);
                break;
            }

            default: {
                fprintf(stderr, "Decoding opcode error, aborting\n");
                abort();
            }
        }
    }
        //execute_instr(regs, &instr);

    return;
}

struct instr_t parse_command(char* command) {

//    assert(command);
//    struct instr_t* instr = (struct instr_t*) calloc(1, sizeof(struct instr_t));
//
//    char* p = command;
//    while (*p != ' ') { //opcode
//        ++p;
//    }
//    *p = '\0';
//
//    if (strcmp(p, "MOVI") == 0) {
//        instr->opcode = MOVI;
//        p += MOVE_SHIFT;
//        instr->opnd.imm = atoi(p);
//    }
//
//    if (strcmp(p, "ADD") == 0) {
//        instr->opcode = ADD;
//
//    }
//
//    if (strcmp(p, "SUB") == 0) {
//        instr->opcode = SUB;
//    }
//
//    if (strcmp(p, "MUL") == 0) {
//        instr->opcode = MUL;
//    }
//
//    if (strcmp(p, "DIV") == 0) {
//        instr->opcode = DIV;
//    }
//
//    if (strcmp(p, "IN") == 0) {
//        instr->opcode = IN;
//    }
//
//    if (strcmp(p, "OUT") == 0) {
//        instr->opcode = OUT;
//    }
//    ++p;




}

void exec_file(struct in_data_t data) {

    char* command = (char*) calloc(MAX_LEN, sizeof(char));

    while (fscanf(data.data_type.files.in_file, "%s", command) == 1) {

        struct instr_t instr = {0};

        int res = get_bin_comm(command);

        unsigned char cmd = res & COMMANDMASK;
        assert(cmd == res);
        instr = decode_instr(cmd);
        exec_command(
    }
}

int main(int argc, char **argv) {


    struct in_data_t data = get_data(argc, argv);

    if (data.comm_file == COMMAND) {
        exec_command(data);
    } else if (data.comm_file == FILE_T) {
        exec_file(data);
    }




//    FILE *prog = NULL;
//
//    if (argc < 2) {
//        fprintf(stderr, "Usage: %s <programm>\n", argv[0]);
//        abort();
//    }
//
//    prog = fopen(argv[1], "r");
//    if (NULL == prog) {
//        fprintf(stderr, "Usage: %s <programm>\n", argv[0]);
//        fprintf(stderr, "Where file with programm shall exist\n");
//        abort();
//    }
//
//    execute_program(prog);
//
//    fclose(prog);
    return 0;
}
