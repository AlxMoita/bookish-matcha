/*
 * Name: <Adissem Moita>
 * Date: <2/04/2025>
 * Title: Lab5 - Stop and Wait Client
 * Description: Stop-and-Wait over UDP with protocol 3.0 Client side
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define BUFFER_SIZE 10
#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"

// Protocol Header
typedef struct {
    int seq_ack;
    int len;
    int checksum;
} Header;

// Packet Structure
typedef struct {
    Header header;
    char data[BUFFER_SIZE];
} Packet;

// Checksum calculation
int compute_checksum(Packet *packet) {
    int checksum = 0;
    char *ptr = (char *)packet;
    char *end = ptr + sizeof(Header) + packet->header.len;
    while (ptr < end)
        checksum ^= *ptr++;
    return checksum;
}

// Function to send packets with retransmission
void send_packet(int sockfd, struct sockaddr *address, socklen_t addrlen, Packet *packet) {
    struct timeval tv;
    fd_set readfds;
    int attempts = 0;

    while (attempts < 3) {
        printf("Sending packet with SEQ: %d\n", packet->header.seq_ack);
        sendto(sockfd, packet, sizeof(*packet), 0, address, addrlen);

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int rv = select(sockfd + 1, &readfds, NULL, NULL, &tv);
        if (rv == 0) {
            printf("Timeout, resending packet %d\n", packet->header.seq_ack);
            attempts++;
        } else {
            Packet ack_packet;
            recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, NULL, NULL);
            if (ack_packet.header.seq_ack == packet->header.seq_ack) {
                printf("Received ACK: %d\n", ack_packet.header.seq_ack);
                return;
            } else {
                printf("Incorrect ACK received, retrying packet %d\n", packet->header.seq_ack);
                attempts++;
            }
        }
    }
    printf("Packet %d failed to send after 3 attempts.\n", packet->header.seq_ack);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <srcfile>\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addrlen = sizeof(serverAddr);
    int seq = 0;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // Open file to read
    int fp = open(argv[1], O_RDONLY);
    if (fp < 0) {
        perror("Failed to open file");
        close(sockfd);
        return 1;
    }

    Packet packet;
    ssize_t bytes_read;
    while ((bytes_read = read(fp, packet.data, BUFFER_SIZE)) > 0) {
        packet.header.len = bytes_read;
        packet.header.seq_ack = seq;
        packet.header.checksum = compute_checksum(&packet);
        send_packet(sockfd, (struct sockaddr *)&serverAddr, addrlen, &packet);
        seq = 1 - seq;
    }

    // Send final zero-length packet for end of file
    Packet final_packet = {0};
    send_packet(sockfd, (struct sockaddr *)&serverAddr, addrlen, &final_packet);

    printf("File sent successfully.\n");
    close(fp);
    close(sockfd);
    return 0;
}
