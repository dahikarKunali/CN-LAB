# Experiment No. 4: Socket Programming for Transport Layer Packet (UDP Server)

---

## Aim
Socket programming for transport layer packet (UDP server).

---

## Theory
At the Transport Layer of the OSI model, **UDP (User Datagram Protocol)** is a connectionless, unreliable, and lightweight protocol. Unlike TCP, UDP does not establish a virtual circuit or perform a three-way handshake prior to data transfer. Each transmitted data unit is handled as an independent **datagram**.

UDP Socket Programming on the server side listens for incoming datagrams from any client without maintaining state or active connections.

### UDP Server Execution Steps:
1. **Socket Creation (`socket()`):** Initializes a datagram socket using address family `AF_INET` (IPv4) and socket type `SOCK_DGRAM` (UDP).
2. **Binding (`bind()`):** Binds the socket to local IP (`INADDR_ANY`) and port (`PORT = 8080`).
3. **Receiving Data (`recvfrom()`):** Blocks and waits for incoming packets. Captures client's data payload along with sender's `sockaddr_in` structure.
4. **Sending Data (`sendto()`):** Transmits an acknowledgment message back directly to the captured client address.
5. **Closing Socket (`close()`):** Releases socket descriptor resources upon termination.

---

## Extracted Source Code
File: `server_udp222.c`
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main()
{
    int server_socket;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    printf("UDP Server is running on port %d...\n", PORT);

    // Receive message from client
    int n = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client_addr, &client_len);
    if (n < 0)
    {
        perror("Receive failed");
        close(server_socket);
        return 1;
    }

    buffer[n] = '\0';
    printf("Message from Client: %s\n", buffer);

    // Send response to client
    char *response = "Hello from UDP Server";
    sendto(server_socket, response, strlen(response), 0,
           (struct sockaddr *)&client_addr, client_len);

    // Close socket
    close(server_socket);
    return 0;
}
```

---

## Conclusion
The UDP Server socket was successfully implemented at the Transport Layer. By utilizing `SOCK_DGRAM` with `recvfrom()` and `sendto()`, the server successfully bound to its designated port and processed incoming connectionless datagrams without establishing a prior network handshake.

---

## How to Execute in Ubuntu Terminal

### Step 1: Create the Source File
Open the file in `gedit`, paste the C code provided above, save and close:
```bash
gedit server_udp222.c
```

### Step 2: Compile the Program
```bash
gcc server_udp222.c -o server
```

### Step 3: Run the UDP Server (Terminal 1)
```bash
./server
```
*Expected Output:*
```text
UDP Server is running on port 8080...
```

### Step 4: Test with UDP Client (Terminal 2)
In a second terminal window, run the UDP client from Experiment 5:
```bash
./client
```
Or test using Netcat:
```bash
echo "hello" | nc -u 127.0.0.1 8080
```
*Observed Server Output:*
```text
Message from Client: hello
```
