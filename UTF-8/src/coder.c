#include "libs.h"
#include "command.h"

int encode(uint32_t code_point, CodeUnit *code_units)
{
    if (code_point < 0x80)
    {
        code_units->lenght = 1;
        code_units->code[0] = code_point;
    }
    else if (code_point < 0x800)
    {
        code_units->lenght = 2;
        code_units->code[0] = 0xc0 | (code_point >> 6);
        code_units->code[1] = 0x80 | (code_point & 0x3f);
    }
    else if (code_point < 0x10000)
    {
        code_units->lenght = 3;
        code_units->code[0] = 0xe0 | (code_point >> 12);
        code_units->code[1] = 0x80 | ((code_point >> 6) & 0x3f);
        code_units->code[2] = 0x80 | (code_point & 0x3f);
    }
    else if (code_point < 2000000)
    {
        code_units->lenght = 4;
        code_units->code[0] = 0xf0 | (code_point >> 18);
        code_units->code[1] = 0x80 | ((code_point >> 12) & 0x3f);
        code_units->code[2] = 0x80 | ((code_point >> 6) & 0x3f);
        code_units->code[3] = 0x80 | (code_point & 0x3f);
    }
    else
    {
        printf("Странное число!\n");
        return -1;
    }
    return 0;
}

int write_code_unit(FILE *out, const CodeUnit *code_unit)
{
    if (code_unit == NULL)
        return -1;

    if (code_unit->lenght < 0 || code_unit->lenght > 4)
        return -1;

    if (fwrite(code_unit->code, sizeof(uint8_t), code_unit->lenght, out) != code_unit->lenght)
        return -1;
    return 0;
}

int read_next_code_unit(FILE *in, CodeUnit *code_units)
{
    uint8_t buf;
    int i, octet, check = 0, shift;
    while (check == 0)
    {
        octet = 0;
        shift = 0x80;
        code_units->lenght = 1;
        if (fread(&buf, sizeof(uint8_t), 1, in) == 1)
        {
            for (i = 7; i < ( (buf & shift) == shift ) && i > 2; i--)
            {
                octet++;
                shift = shift / 2;
            }

            if (octet != 1 && i != 2)
            {
                code_units->code[0] = buf;
                check = 1;
            }
            else
            {
                check = 0;
                continue;
            }
            for (int j = 1; octet > 1; j++)
            {
                if (fread(&buf, sizeof(uint8_t), 1, in) == 1)
                {
                    if ( ( (buf >> 7) != 1 ) || ( (buf & 0x40) == 0x40) ) //01000000
                    {
                        check = 0;
                        break;
                    }
                    else
                    {
                        code_units->code[0] = buf;
                        code_units->lenght++;
                        octet--;
                        check = 1;
                    }
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

uint32_t decode(const CodeUnit *code_unit)
{
    uint32_t res = 0;
    if (code_unit->lenght == 1)
    {
        res = code_unit->code[0];
    }
    else if (code_unit->lenght == 2)
    {
        res += code_unit->code[0] & 0x1F; // 00011111
        res <<= 6;
        res += code_unit->code[1] & 0x4F; // 00111111
    }
    else if (code_unit->lenght == 3)
    {
        res += code_unit->code[0] & 0xF; // 00001111
        res <<= 6;
        for (int i = 1; i < 3; i++)
        {
            res += code_unit->code[i] & 0x3F; // 00000111
            if (i + 1 != 3)
                res <<= 6;
        }
    }
    else if (code_unit->lenght == 4)
    {
        res += code_unit->code[0] & 0x7;
        res <<= 6;
        for (int i = 1; i < 4; i++)
        {
            res += code_unit->code[i] & 0x3F;
            if (i + 1 != 4)
                res <<= 6;
        }
    }
    return res;
}