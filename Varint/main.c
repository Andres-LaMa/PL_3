#include "varint.h"

#define SIZE 1000000

int main()
{
    FILE *fuc = fopen("uncompressed", "wb");
    FILE *fc = fopen("compressed", "wb");

    if (fuc == NULL || fc == NULL)
    {
        printf("Ошибка открытия файлов\n");
        return -1;
    }

    const uint8_t *bufp[SIZE];
    uint8_t buf[4];

    for (unsigned int i = 0; i < SIZE; i++)
    {
        uint32_t number;

        number = generate_number();

        size_t code_number = encode_varint(number, buf);

        fwrite(&number, sizeof(uint32_t), 1, fuc);
        fwrite(buf, sizeof(uint8_t), code_number, fc);
    }
    
    fclose(fuc);
    fclose(fc);

    fuc = fopen("uncompressed", "rb");
    fc = fopen("compressed", "rb");

    for (unsigned int i = 0; i < SIZE; i++)
    {
        uint32_t number;
        
        fread(&number, sizeof(uint32_t), 1, fuc);

        for (int j = 0; j < 4; j++)
        {
            fread(&buf[j], sizeof(uint8_t), 1, fc);
            if ((buf[j] >> 7) == 0)
                break;
            
        }
        bufp[0] = buf;

        assert(number == decode_varint(&bufp[0]) && "Числа не равны!\n");
        // printf("%d == %d\n", number, decode_varint(&bufp[0]) );
    }
    
}