#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define POLYNOMIAL 0xEDB88320  // CRC-32 polynomial
#define INITIAL_VALUE 0xFFFFFFFF
#define FINAL_XOR_VALUE 0xFFFFFFFF

// Function to compute CRC-32 on binary data
uint32_t crc32_binary(uint32_t data, int bits) {
    uint32_t crc = INITIAL_VALUE;
    for (int i = bits - 1; i >= 0; i--) {
        crc ^= (data >> i) & 1;
        for (int j = 7; j >= 0; j--) {
            if (crc & 1)
                crc = (crc >> 1) ^ POLYNOMIAL;
            else
                crc >>= 1;
        }
    }
    return crc ^ FINAL_XOR_VALUE;
}

// Function to print binary representation of a number
void print_binary(uint32_t num, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

// Function to convert binary string to integer
uint32_t binary_string_to_int(char *binary_str) {
    uint32_t num = 0;
    while (*binary_str) {
        num = num << 1;
        if (*binary_str == '1') num |= 1;
        binary_str++;
    }
    return num;
}

int main() {
    char binary_str[65];  // To store binary input (up to 64 bits)
    printf("Enter a binary number (up to 64 bits): ");
    scanf("%s", binary_str);

    int bits = strlen(binary_str);
    if (bits > 64) {
        printf("Input exceeds 64 bits!\n");
        return 1;
    }

    uint32_t data = binary_string_to_int(binary_str);

    uint32_t crc = crc32_binary(data, bits);

    printf("CRC32 for Binary Data (in Binary): ");
    print_binary(crc, 32);  // CRC32 output in 32-bit binary
    return 0;
}

