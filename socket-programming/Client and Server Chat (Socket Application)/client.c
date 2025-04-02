#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#define PORT 5000 
#define BUFFER_SIZE 1024 
int main() { 
int socket_fd; 
struct sockaddr_in server_addr; 
char buffer[BUFFER_SIZE]; 
// Create socket 
socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_fd < 0) { 
        perror("Client socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT);  // Server port 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost 
 
    // Connect to the server 
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
        perror("Connection to server failed"); 
        close(socket_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Connected to server\n"); 
 
    // Client Chat Loop 
    while (1) { 
        printf("You: "); 
        fgets(buffer, sizeof(buffer), stdin);  // Get message from client 
        send(socket_fd, buffer, strlen(buffer), 0);  // Send message to server 
 
        int bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0); 
        if (bytes_received <= 0) { 
            printf("Server disconnected.\n"); 
            break; 
        } 
 
        buffer[bytes_received] = '\0';  // Null-terminate the received message 
        printf("Server: %s\n", buffer);  // Display server's response 
    } 
 
    // Close the socket 
    close(socket_fd); 
    return 0; 
}