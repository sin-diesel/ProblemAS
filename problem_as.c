#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "problem_as.h"
#include "problem_mk.h"

extern const int MAX_LEN = 20;


char *strrev(char *str) { // str reverse function
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


int get_bin_comm(char* command) { // conversion of binary num in string representation to int representation

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

char * int_to_bin(int i)
{
    size_t bits = sizeof(int);

    char * str = malloc(bits + 1);
    if (!str) return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
}

struct in_data_t get_data(int argc, char **argv) { // acquiring data

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

    char* reg1 = (char*) calloc(5, sizeof(char));
    char* reg2 = (char*) calloc(5, sizeof(char));
    char* reg3 = (char*) calloc(5, sizeof(char));

    switch(instr.opnd.ops.rs) { // determining which regs to write
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

    switch(instr.opnd.ops.rd) { // determining which regs to write
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
            fprintf(stream, "MOVI D, %d\n", instr.opnd.imm);
            break;
        }

        case ADD: {
            fprintf(stream, "ADD R%s, R%s\n", reg1, reg2);
            break;
        }

        case SUB: {
            fprintf(stream, "SUB R%s, R%s\n", reg1, reg2);
            break;
        }
        case MUL: {
            fprintf(stream, "MUL R%s, R%s\n", reg1, reg2);
            break;
        }
        case DIV: {
            fprintf(stream, "DIV R%s, R%s\n", reg1, reg2);
            break;
        }

        case IN: {
            fprintf(stream, "IN R%s\n", reg3);
            break;
        }

        case OUT: {
            fprintf(stream, "OUT R%s\n", reg3);
            break;
        }

        default: {
            fprintf(stderr, "Decoding opcode error, aborting\n");
            abort();
        }
    }
    //execute_instr(regs, &instr);

    return;
}

void exec_command_enc(struct instr_t instr, FILE* stream) {

    switch(instr.opcode) {

        case MOVI: {
            fprintf(stream, "%04d", MOVE_COM);
            fprintf(stream, "%s ", int_to_bin(instr.opnd.imm));
            break;
        }

        case ADD: {
            fprintf(stream, "%d", ADD_COM);
            fprintf(stream, "%s", (int_to_bin(instr.opnd.ops.rd) + 2));
            fprintf(stream, "%s ", (int_to_bin(instr.opnd.ops.rs) + 2));
            break;
        }

        case SUB: {
            fprintf(stream, "%d", SUB_COM);
            fprintf(stream, "%s", (int_to_bin(instr.opnd.ops.rd) + 2));
            fprintf(stream, "%s ", (int_to_bin(instr.opnd.ops.rs) + 2));
            break;
        }

        case MUL: {
            fprintf(stream, "%d", MUL_COM);
            fprintf(stream, "%s", (int_to_bin(instr.opnd.ops.rd) + 2));
            fprintf(stream, "%s ", (int_to_bin(instr.opnd.ops.rs) + 2));
            break;
        }

        case DIV: {
            fprintf(stream, "%d", DIV_COM);
            fprintf(stream, "%s", (int_to_bin(instr.opnd.ops.rd) + 2));
            fprintf(stream, "%s ", (int_to_bin(instr.opnd.ops.rs) + 2));
            break;
        }

        case IN: {
            fprintf(stream, "%d", IN_COM);
            fprintf(stream, "%s ", int_to_bin(instr.opnd.rop) + 2);
            break;
        }

        case OUT: {
            fprintf(stream, "%d", OUT_COM);
            fprintf(stream, "%s ", int_to_bin(instr.opnd.rop) + 2);
            break;
        }

        default: {
            fprintf(stderr, "No matching command found, aborting");
            abort();
        }

    }


}

void exec_file_dec(struct in_data_t data) {

    char* command = (char*) calloc(MAX_LEN, sizeof(char));

    while (fscanf(data.data_type.files.in_file, "%s", command) == 1) {

        struct instr_t instr = {0};

        int res = get_bin_comm(command);

        unsigned char cmd = res & COMMANDMASK;
        assert(cmd == res);
        instr = decode_instr(cmd);
        exec_command_dec(instr, data.data_type.files.out_file);
    }

    return;
}

struct instr_t decode_instr_asm(char* cmd_asm) {

    assert(cmd_asm);

    struct instr_t* instr = (struct instr_t*) calloc(1, sizeof(struct instr_t));
    assert(instr);

    int cmd_len = strlen(cmd_asm);

    //cmd_asm[cmd_len - 2] = '\0';//removing " " symbols from cmd
    //cmd_asm[0] = '\0';
    //cmd_asm++;

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
    else if (strcmp(beginning, "ADD") == 0) {
        instr->opcode = ADD;
        p += 2;
        beginning = p;
        p++;
        *p = '\0';
        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rd = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rd = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rd = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rd = D;
        }

        p += ADD_SHIFT;
        beginning = p;
        p++;
        *p = '\0';

        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rs = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rs = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rs = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rs = D;
        }
    }
    else if (strcmp(beginning, "SUB") == 0) {
        instr->opcode = SUB;
        p += 2;
        beginning = p;
        p++;
        *p = '\0';
        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rd = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rd = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rd = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rd = D;
        }

        p += SUB_SHIFT;
        beginning = p;
        p++;
        *p = '\0';

        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rs = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rs = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rs = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rs = D;
        }
    }

    if (strcmp(beginning, "MUL") == 0) {
        instr->opcode = MUL;
        p += 2;
        beginning = p;
        p++;
        *p = '\0';
        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rd = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rd = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rd = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rd = D;
        }

        p += MUL_SHIFT;
        beginning = p;
        p++;
        *p = '\0';

        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rs = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rs = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rs = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rs = D;
        }
    }

    if (strcmp(beginning, "DIV") == 0) {
        instr->opcode = DIV;
        p += 2;
        beginning = p;
        p++;
        *p = '\0';
        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rd = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rd = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rd = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rd = D;
        }

        p += DIV_SHIFT;
        beginning = p;
        p++;
        *p = '\0';

        if (strcmp(beginning, "A") == 0) {
            instr->opnd.ops.rs = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.ops.rs = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.ops.rs = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.ops.rs = D;
        }
    }

    if (strcmp(beginning, "IN") == 0) {
        instr->opcode = IN;
        p += IN_SHIFT;
        beginning = p;
        if (strcmp(beginning, "A") == 0) {
            instr->opnd.rop = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.rop = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.rop = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.rop = D;
        }
    }

    if (strcmp(beginning, "OUT") == 0) {
        instr->opcode = OUT;
        p += OUT_SHIFT;
        beginning = p;
        if (strcmp(beginning, "A") == 0) {
            instr->opnd.rop = A;
        }
        if (strcmp(beginning, "B") == 0) {
            instr->opnd.rop = B;
        }
        if (strcmp(beginning, "C") == 0) {
            instr->opnd.rop = C;
        }
        if (strcmp(beginning, "D") == 0) {
            instr->opnd.rop = D;
        }
    }

    return *instr;
}

void exec_file_enc(struct in_data_t data) {

    char* command = (char*) calloc(MAX_LEN, sizeof(char));
    assert(command);

    while (fgets(command, MAX_LEN, data.data_type.files.in_file) != NULL) {

        command[strlen(command) - 1] = '\0';
        struct instr_t instr = decode_instr_asm(command);
        exec_command_enc(instr, data.data_type.files.out_file);

    }

}