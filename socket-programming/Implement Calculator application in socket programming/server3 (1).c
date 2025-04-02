#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

// Function to perform the arithmetic operation
double calculate(double num1, double num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': 
            if (num2 != 0) {
                return num1 / num2;
            } else {
                return 0;  // Handle division by zero
            }
        default:
            return 0;  // Invalid operator
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];
    double num1, num2, result;
    char operator;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server is waiting for a connection...\n");

    // Accept client connection
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Connection established with client\n");

    // Receive the operation (num1 operator num2) from the client
    recv(client_fd, buffer, sizeof(buffer), 0);

    // Parse the received message
    sscanf(buffer, "%lf %c %lf", &num1, &operator, &num2);

    // Perform the calculation
    result = calculate(num1, num2, operator);

    // Send the result back to the client
    sprintf(buffer, "Result: %.2f", result);
    send(client_fd, buffer, strlen(buffer), 0);

    // Close client connection
    close(client_fd);
    close(server_fd);

    return 0;
}

