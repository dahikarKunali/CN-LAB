#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT (10000 + 229) // Port 10229
#define BUF_SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUF_SIZE];
    int addrlen = sizeof(address);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("TCP Echo Server listening on port %d...\n", PORT);

    // Accept client connection
    new_socket = accept(
        server_fd,
        (struct sockaddr *)&address,
        (socklen_t *)&addrlen
    );
    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        memset(buffer, 0, BUF_SIZE);

        int bytes = read(new_socket, buffer, BUF_SIZE);

        if (bytes <= 0)
            break;

        printf("Received: %s", buffer);

        // Echo back to client
        send(new_socket, buffer, bytes, 0);
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
