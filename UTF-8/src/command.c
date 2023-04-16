#include "libs.h"
#include "coder.h"

int encode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;

    in = fopen(in_file_name, "r");
    out = fopen(out_file_name, "wb");

    if (!in || !out)
    {
        printf("Ошибка открытия файла/ов!\n");
        return -1;
    }

    uint32_t code_point;
    CodeUnit code_unit;

    while (fscanf(in, "%" SCNx32, &code_point) >= 0)
    {
        printf("%" PRIx32 "\n", code_point);
        if (encode(code_point, &code_unit) < 0)
        {
            printf("Ошибака кодирования\n");
            return -1;
        }
        write_code_unit(out, &code_unit);
    }

    fclose(in);
    fclose(out);

    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name)
{
    if (in_file_name == NULL || out_file_name == NULL)
        return -1;
    
    FILE *in;
    FILE *out;

    in = fopen(in_file_name, "r");
    out = fopen(out_file_name, "wb");

    if (!in || !out)
    {
        printf("Ошибка открытия файла/ов!\n");
        return -1;
    }

    CodeUnit code_unit;
    uint32_t code_point;
    while (read_next_code_unit(in, &code_unit) == 0)
    {
        code_point = decode(&code_unit);

        if (fprintf(out, "%x", code_point) < 1)
        {
            printf("Ошибка записи в файл\n");
            return -1;
        }
        fprintf(out, "\n");
    }
    printf("Программа завершена\n");
    fclose(in);
    fclose(out);

    return 0;
}