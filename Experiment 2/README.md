# Experiment No. 2: Socket Programming for Transport Layer Packets (TCP Server)

---
## Aim
Socket Programming for Transport Layer Packets (TCP Server).

---

## Theory
A socket is a software abstraction representing an endpoint of a two-way communication link between two programs running on a network. It is uniquely identified by the combination of an **IP Address** and a **Port Number**.

At the Transport Layer of the OSI model, **TCP (Transmission Control Protocol)** provides connection-oriented, reliable, ordered, and error-checked data delivery. TCP Socket Programming on the server side operates as a **Passive Open** process, waiting for incoming connection requests from client nodes.

### Server-Side Execution Steps:
1. **Socket Creation (`socket()`):** The server creates an endpoint using domain `AF_INET` (IPv4) and socket type `SOCK_STREAM` (TCP).
2. **Binding (`bind()`):** Binds the socket descriptor to a designated local IP address (`INADDR_ANY`) and port number (`PORT = 10000 + 222 = 10222`).
3. **Listening (`listen()`):** Transitions the socket into passive listening mode with a client backlog queue.
4. **Accepting Connection (`accept()`):** Blocks execution until an incoming connection arrives, completing the TCP Three-Way Handshake (SYN, SYN-ACK, ACK) and returning a dedicated active socket descriptor (`new_socket`).
5. **Data Transfer (`read()` / `send()`):** Receives data buffers from client and echoes them back reliably.
6. **Closing Connection (`close()`):** Terminates client and listening sockets via the TCP four-wave termination sequence.

---

## Extracted Source Code
File: `server1222.c`
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT (10000 + 222) // Port 10222
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
```

---

## Conclusion
The TCP Server socket was successfully implemented at the Transport Layer. The passive open execution sequence (`socket`, `bind`, `listen`, and `accept`) was executed, establishing a reliable, connection-oriented channel capable of processing incoming client data streams.

---

## How to Execute in Ubuntu Terminal

### Step 1: Create the Source File
Open the file in `gedit`, paste the C code provided above, save and close:
```bash
gedit server1222.c
```

### Step 2: Compile the Program
```bash
gcc server1222.c -o server
```

### Step 3: Run the TCP Server (Terminal 1)
```bash
./server
```
*Expected Output:*
```text
TCP Echo Server listening on port 10222...
```

### Step 4: Test with a Client (Terminal 2)
In a second terminal window, connect via the TCP client or Netcat:
```bash
nc 127.0.0.1 10222
```
*Observed Server Output:*
```text
Received: hello
Received: hiii
Received: test
```

### Troubleshooting
If port 10222 is busy:
```bash
sudo fuser -k 10222/tcp
```
