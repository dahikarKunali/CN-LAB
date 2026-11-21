#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT (10000 + 229) // Port 10229
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    // Use loopback address (127.0.0.1) or the server's LAN IP address
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    printf("Connected to TCP Echo Server on port %d.\n", PORT);

    while (1)
    {
        printf("Enter message: ");
        if (fgets(buffer, BUF_SIZE, stdin) == NULL)
            break;

        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUF_SIZE);

        int bytes = read(sock, buffer, BUF_SIZE);
        if (bytes <= 0)
            break;

        printf("Echo from server: %s", buffer);
    }

    close(sock);

    return 0;
}
