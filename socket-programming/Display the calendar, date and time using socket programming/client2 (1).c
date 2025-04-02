#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_fd);
        exit(1);
    }

    // Receive the current date and time from the server
    recv(client_fd, buffer, sizeof(buffer), 0);

    // Display the received date and time
    printf("Current Date and Time: %s\n", buffer);

    // Close the client socket
    close(client_fd);

    return 0;
}

