#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

enum
{
    MaxCodeLenght = 4
};

typedef struct
{
    uint8_t code[MaxCodeLenght];
    size_t lenght;
} CodeUnit;