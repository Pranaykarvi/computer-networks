#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#define PORT 5000 
#define BUFFER_SIZE 1024 
void send_message(int socket_fd) { 
char message[BUFFER_SIZE]; 
printf("Enter message: "); 
fgets(message, sizeof(message), stdin); 
send(socket_fd, message, strlen(message), 0); 
} 
void send_file(int socket_fd) { 
char filename[BUFFER_SIZE]; 
printf("Enter the filename to send: "); 
fgets(filename, sizeof(filename), stdin); 
filename[strcspn(filename, "\n")] = 0;  // Remove the newline character 
FILE *file = fopen(filename, "rb"); 
if (file == NULL) { 
perror("File not found"); 
return; 
} 
// Send the filename to the server 
send(socket_fd, filename, strlen(filename), 0); 
// Send file content in chunks 
char buffer[BUFFER_SIZE]; 
size_t n; 
while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) { 
send(socket_fd, buffer, n, 0); 
} 
printf("File sent successfully.\n"); 
fclose(file); 
} 
 
int main() { 
    int socket_fd; 
    struct sockaddr_in server_addr; 
 
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_fd < 0) { 
        perror("Socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
 
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
        perror("Connection failed"); 
        close(socket_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Connected to server\n"); 
 
    while (1) { 
        printf("Enter 1 to send message, 2 to send file, or 0 to quit: "); 
        int choice; 
        scanf("%d", &choice); 
        getchar();  // Consume newline after the choice input 
 
        // Send the choice as a character ('1', '2', or '0') 
        if (choice == 1) { 
            send(socket_fd, "1", 1, 0);  // Send '1' to indicate message sending 
            send_message(socket_fd); 
        } else if (choice == 2) { 
            send(socket_fd, "2", 1, 0);  // Send '2' to indicate file sending 
            send_file(socket_fd); 
        } else if (choice == 0) { 
            send(socket_fd, "0", 1, 0);  // Send '0' to indicate exit 
            break; 
        } else { 
            printf("Invalid choice. Please try again.\n"); 
        } 
    } 
 
    close(socket_fd); 
    return 0; 
} 
