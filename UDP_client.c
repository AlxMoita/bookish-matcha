/*
 * Name: <Adissem Moita>
 * Date: <2/04/2025>
 * Title: Lab4 - Part 1
 * Description: This program allows a UDP server to receives a file from a client.
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
    struct sockaddr_in serverAddr, clientAddr;
    unsigned int clientAddrLen = sizeof(clientAddr); 
    char buffer[BUFFER_SIZE];

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Step 2: Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // Step 3: Bind socket
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // Step 4: Open file
    FILE *file = fopen("received_file.txt", "wb");
    if (!file) {
        perror("Failed to open file");
        close(sockfd);
        return 1;
    }

    int bytesReceived;
    while ((bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen)) > 0) {
        fwrite(buffer, 1, bytesReceived, file);
        if (bytesReceived < BUFFER_SIZE) break; 
    }

    printf("File received successfully.\n");
    fclose(file);
    close(sockfd);

    return 0;
}
