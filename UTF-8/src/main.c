#include "libs.h"
#include "coder.h"
#include "command.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Аргументов не 4!\n");
        return -1;
    }
    const char *command = argv[1];
    const char *in_file_name = argv[2];
    const char *out_file_name = argv[3];

    if (strcmp(command, "encode") == 0)
    {
        encode_file(in_file_name, out_file_name);
    }
    else if (strcmp(command, "decode") == 0)
    {
        decode_file(in_file_name, out_file_name);
    }
    else
    {
        printf("Команда не распознан!\n");
        return -1;
    }
    
    
}