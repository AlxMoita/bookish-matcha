/*
 * Name: <Adissem Moita>
 * Date: <2/04/2025>
 * Title: Lab4 - Part 2
 * Description: This program allows a UDP client to send a file to a server.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Open file to read data
    FILE *file = fopen("file_to_send.txt", "rb");
    if (!file) {
        perror("Failed to open file");
        close(sockfd);
        return 1;
    }
   // Read data and Send over the socket
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (sendto(sockfd, buffer, bytesRead, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Failed to send data");
            fclose(file);
            close(sockfd);
            return 1;
        }
    }

    printf("File sent successfully.\n");
    fclose(file);
    close(sockfd);

    return 0;
}



