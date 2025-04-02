#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void caesarCipher(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];
        if (ch >= 'a' && ch <= 'z') {
            text[i] = (ch - shift - 'a' + 26) % 26 + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            text[i] = (ch - shift - 'A' + 26) % 26 + 'A';
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Enter a message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    send(sock, buffer, strlen(buffer), 0);
    printf("Message sent\n");

    read(sock, buffer, BUFFER_SIZE);
    printf("Encrypted message received: %s\n", buffer);

    caesarCipher(buffer, 3); // Decrypt with a shift of 3
    printf("Decrypted message: %s\n", buffer);

    close(sock);
    return 0;
}

