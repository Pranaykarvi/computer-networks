#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
 
#define PORT 5000 
#define BUFFER_SIZE 1024 
 
int main() { 
    int server_fd, client_fd; 
    struct sockaddr_in server_addr, client_addr; 
    socklen_t addr_size; 
    char buffer[BUFFER_SIZE]; 
 
    // Create the socket 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd < 0) { 
        perror("Server socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT);  // Port number 
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP 
 
    // Bind the socket to the IP address and port 
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
        perror("Binding failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    // Listen for incoming connections 
    if (listen(server_fd, 10) == 0) { 
        printf("Server is listening on port %d...\n", PORT); 
    } else { 
        perror("Listen failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    addr_size = sizeof(client_addr); 
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size); 
    if (client_fd < 0) { 
        perror("Connection failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Client connected\n"); 
 
    // Server Chat Loop 
    while (1) { 
        // Receive message from client 
        int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0); 
        if (bytes_received <= 0) { 
            printf("Client disconnected.\n"); 
            break; 
        } 
 
        buffer[bytes_received] = '\0';  // Null-terminate the received message 
        printf("Client: %s\n", buffer); 
 
        // Send message back to client (Echoing the message) 
        printf("Server: "); 
        fgets(buffer, sizeof(buffer), stdin);  // Get input from the server 
        send(client_fd, buffer, strlen(buffer), 0); 
    } 
 
    // Close the connection 
close(client_fd); 
close(server_fd); 
return 0; 
} 
