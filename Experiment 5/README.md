# Experiment No. 5: Socket Programming for Transport Layer Packet (UDP Client)

---

## Aim
Socket programming for transport layer packet (UDP client).

---

## Theory
The **UDP Client** operates as an active sender in a connectionless architecture. Because UDP does not maintain a persistent connection or require handshaking, the client can immediately transmit datagrams to a target server once the destination IP address and port number are configured.

### UDP Client Execution Steps:
1. **Socket Creation (`socket()`):** Initializes a datagram socket using `AF_INET` (IPv4) and `SOCK_DGRAM` (UDP).
2. **Addressing Setup:** Configures `sockaddr_in` with destination IP (`127.0.0.1`) and port (`8080`).
3. **Data Transmission (`sendto()`):** Transmits datagram directly to destination without calling `connect()`.
4. **Data Reception (`recvfrom()`):** Waits for incoming response datagram from server.
5. **Closing Socket (`close()`):** Closes descriptor and releases kernel buffer.

---

## Extracted Source Code
File: `client_udp_222.c`
```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd;
    char message[100], buffer[100];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    // Send message to server
    sendto(sockfd, message, strlen(message), 0,
           (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Receive response from server
    addr_size = sizeof(serverAddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&serverAddr, &addr_size);
    if (n >= 0)
    {
        buffer[n] = '\0';
        printf("Server Response: %s\n", buffer);
    }

    // Close socket
    close(sockfd);
    return 0;
}
```

---

## Conclusion
The UDP Client socket was successfully constructed at the Transport Layer. Using `SOCK_DGRAM` and the `sendto()` system call, the client demonstrated connectionless communication by sending datagram packets directly to the server without needing to perform a TCP-style connection handshake.

---

## How to Execute in Ubuntu Terminal

### Step 1: Create the Source File
Open the file in `gedit`, paste the C code provided above, save and close:
```bash
gedit client_udp_222.c
```

### Step 2: Compile the Program
```bash
gcc client_udp_222.c -o client
```

### Step 3: Execute Client-Server Communication

#### Terminal 1: Run UDP Server (from Experiment 4)
```bash
./server
```

#### Terminal 2: Run UDP Client
```bash
./client
```

### Step 4: Sample Interactive Session (Terminal 2)
```text
Enter message: test 222
Server Response: Hello from UDP Server
```
