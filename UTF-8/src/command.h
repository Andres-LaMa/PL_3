

typedef struct
{
    uint8_t code[MaxCodeLenght];
    size_t lenght;
} CodeUnits;

int encode_file(const char *in_file_name, const char *out_file_name);

int decode_file(const char *in_file_name, const char *out_file_name);

