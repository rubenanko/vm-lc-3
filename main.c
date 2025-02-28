#include <stdio.h>
#include <stdint.h>

#define MEMORY_MAX 1 << 16
#define PROGRAM_COUNTER_START 0x3000

typedef enum
{
    REGISTER_R0 = 0,
    REGISTER_R1,
    REGISTER_R2,
    REGISTER_R3,
    REGISTER_R4,
    REGISTER_R5,
    REGISTER_R6,
    REGISTER_R7,
    REGISTER_PROGRAM_COUNTER,
    REGISTER_CONDITION,
    REGISTER_COUNT,
} REGISTER_CODE;

typedef enum 
{
    OP_CODE_BR = 0,
    OP_CODE_ADD,   
    OP_CODE_LD,    
    OP_CODE_ST,    
    OP_CODE_JSR,   
    OP_CODE_AND,   
    OP_CODE_LDR,   
    OP_CODE_STR,   
    OP_CODE_RTI,   
    OP_CODE_NOT,   
    OP_CODE_LDI,   
    OP_CODE_STI,   
    OP_CODE_JMP,   
    OP_CODE_RES,   
    OP_CODE_LEA,   
    OP_CODE_TRAP   
} OPERATION_CODE;


typedef enum
{
    CFLAG_POSITIVE = 1,
    CFLAG_ZERO = 2,
    CFLAG_NEGATIVE = 4,
} CONDITION_FLAG;

uint16_t MEMORY[MEMORY_MAX];
int REGISTERS[REGISTER_COUNT];

uint16_t read_memory_block(uint16_t offset)
{
    if(-1 < offset && offset < MEMORY_MAX)
        return MEMORY[offset];
}

uint16_t write_memory_block(uint16_t offset,uint16_t data)
{
    if(-1 < offset && offset < MEMORY_MAX)
    {
        MEMORY[offset] = data;
        return 1;
    }
    else
        return 0;
}

uint16_t write_memory(uint16_t offset,uint16_t * data, uint16_t size)
{
    if(-1 < offset && offset < MEMORY_MAX+size)
    {
        while(size > 0)
        {
            MEMORY[offset++] = *(data++);
            size--;
        }
        return 1;
    }
    else
        return 0;
}

uint16_t read_bit(uint16_t block,uint16_t offset)
{
    return (block>>offset) & 1;
}


uint16_t read_subint(uint16_t block,uint16_t offset,uint16_t size)
{
    if(size == 0)
        return (block>>offset);
    else
        return (block>>offset) & (0xFFFF>>(16-size));
}


int initialisation()
{
    REGISTERS[REGISTER_CONDITION] = CFLAG_ZERO;
    REGISTERS[REGISTER_PROGRAM_COUNTER] = PROGRAM_COUNTER_START;
    return 1;
}

int main()
{
    int run = initialisation();
    uint16_t instruction = 0;
    uint16_t opcode;
    
    write_memory_block(PROGRAM_COUNTER_START,(OP_CODE_ADD<<12) + (REGISTER_R0<<9) + (REGISTER_R1<<6) + (1<<5) + 2); //immediate
    write_memory_block(PROGRAM_COUNTER_START + 1,(OP_CODE_ADD<<12) + (REGISTER_R0<<9) + (REGISTER_R1<<6) + REGISTER_R0); //register

    REGISTERS[REGISTER_R1] = 5;
    REGISTERS[REGISTER_R0] = 4;

    // while(run)
    for(int i=0;i<2;i++)
    {
        instruction = read_memory_block(REGISTERS[REGISTER_PROGRAM_COUNTER]++);
        opcode = instruction >> 12;

        switch(opcode)
        {
            case OP_CODE_ADD:
                if(read_bit(instruction,5))
                    REGISTERS[read_subint(instruction,9,3)] = read_subint(instruction,0,5) + REGISTERS[read_subint(instruction,6,3)];
                else
                    REGISTERS[read_subint(instruction,9,3)] = REGISTERS[read_subint(instruction,0,3)] + REGISTERS[read_subint(instruction,6,3)];
                break;

            default:
                printf("ERROR : Unknown opcode\n");
                break;            
        }
        printf("%d\n",REGISTERS[REGISTER_R0]);
    }

    return 0;
}

