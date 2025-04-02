#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#define PORT 5000 
#define BUFFER_SIZE 1024 
// Function to receive and print messages 
void receive_message(int client_fd) { 
char message[BUFFER_SIZE]; 
int bytes_received = recv(client_fd, message, sizeof(message), 0); 
if (bytes_received > 0) { 
message[bytes_received] = '\0';  // Null-terminate the received message 
printf("Message received: %s\n", message); 
} 
} 
// Function to receive and save files 
void receive_file(int client_fd) { 
char filename[BUFFER_SIZE]; 
int bytes_received = recv(client_fd, filename, sizeof(filename), 0); 
if (bytes_received <= 0) { 
printf("Failed to receive filename.\n"); 
return; 
} 
filename[bytes_received] = '\0';  // Null-terminate the filename 
// Open the file for writing 
FILE *file = fopen(filename, "wb"); 
if (file == NULL) { 
perror("Error opening file for writing"); 
return; 
} 
char buffer[BUFFER_SIZE]; 
while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) { 
fwrite(buffer, 1, bytes_received, file);  // Write the received data to the file 
} 
printf("File %s received successfully.\n", filename); 
fclose(file); 
} 
int main() { 
int server_fd, client_fd; 
struct sockaddr_in server_addr, client_addr; 
socklen_t addr_size = sizeof(client_addr); 
 
    // Create the socket 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd < 0) { 
        perror("Socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
 
    // Set up server address 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
 
    // Bind the socket to the address and port 
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
        perror("Binding failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    // Start listening for connections 
    if (listen(server_fd, 1) < 0) { 
        perror("Listen failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Server listening on port %d...\n", PORT); 
 
    // Accept client connection 
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size); 
    if (client_fd < 0) { 
        perror("Connection failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Connection established with client\n"); 
 
    // Main server loop 
    while (1) { 
        char choice; 
        int bytes_received = recv(client_fd, &choice, sizeof(choice), 0); 
        if (bytes_received <= 0) break; 
 
        // Handle the received choice from the client 
        if (choice == '1') { 
            receive_message(client_fd);  // Receive and display a message 
        } else if (choice == '2') { 
            receive_file(client_fd);  // Receive and save a file 
        } else if (choice == '0') { 
            printf("Client has disconnected.\n"); 
            break;  // Exit the server loop 
        } else { 
            printf("Invalid option received: %c\n", choice); 
        } 
    } 
 
    close(client_fd); 
    close(server_fd); 
    return 0; 
}