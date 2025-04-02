#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Function to compute checksum for binary data
uint8_t checksum_binary(uint32_t data, int bits) {
    uint8_t sum = 0;
    for (int i = bits - 1; i >= 0; i--) {
        sum += (data >> i) & 1;  // Sum the bits
    }
    return sum;
}

// Function to print binary representation of a number
void print_binary(uint8_t num, int bits) {
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

    uint8_t sum = checksum_binary(data, bits);

    printf("Checksum for Binary Data (in Binary): ");
    print_binary(sum, 8);  // Checksum output in 8-bit binary
    return 0;
}

