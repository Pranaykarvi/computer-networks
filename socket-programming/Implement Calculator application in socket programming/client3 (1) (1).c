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
    double num1, num2;
    char operator;

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

    // Get the input expression from the user
    printf("Enter a mathematical expression (e.g., 5 + 3): ");
    scanf("%lf %c %lf", &num1, &operator, &num2);

    // Send the expression to the server
    sprintf(buffer, "%.2f %c %.2f", num1, operator, num2);
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive the result from the server
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("Server Response: %s\n", buffer);

    // Close the client socket
    close(client_fd);

    return 0;
}

