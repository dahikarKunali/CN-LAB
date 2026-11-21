# Experiment No. 3: Socket Programming for Transport Layer Packets (TCP Client)

---

## Aim
Socket Programming for Transport Layer Packets (TCP Client).

---

## Theory
In a client-server architecture, the **client** acts as the active process that initiates communication by sending a request to a passive server running on a known IP address and port number.

At the Transport Layer, the TCP Client provides **Active Open** socket functionality. It guarantees data delivery by using sequence numbers, acknowledgments, and checksums over a virtual connection stream.

### Client-Side Execution Steps:
1. **Socket Creation (`socket()`):** Initializes an endpoint socket specifying address family (`AF_INET` for IPv4) and protocol type (`SOCK_STREAM` for TCP).
2. **Server Addressing:** Configures a socket address structure (`sockaddr_in`) containing target server's destination IP address and port number (`PORT = 10000 + 222 = 10222`).
3. **Initiating Connection (`connect()`):** Triggers the TCP Three-Way Handshake with the listening server:
   - **SYN:** Client sends synchronization packet.
   - **SYN-ACK:** Server responds with synchronization and acknowledgment.
   - **ACK:** Client confirms connection.
4. **Data Transmission (`send()` / `read()`):** Sends strings from user input through the full-duplex socket and prints echoed responses.
5. **Termination (`close()`):** Closes the connection gracefully releasing kernel descriptors.

---

## Extracted Source Code
File: `client1222.c`
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT (10000 + 222) // Port 10222
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
    // Connect to localhost (127.0.0.1) or remote server IP
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
```

---

## Conclusion
The TCP Client socket was successfully constructed at the Transport Layer. Using the active open mechanism (`socket`, setting server parameters, and `connect`), the client successfully established a connection with the server via the TCP Three-Way Handshake and enabled reliable stream-based data transmission.

---

## How to Execute in Ubuntu Terminal

### Step 1: Create the Source File
Open the file in `gedit`, paste the C code provided above, save and close:
```bash
gedit client1222.c
```

### Step 2: Compile the Program
```bash
gcc client1222.c -o client
```

### Step 3: Execute Client-Server Communication

#### Terminal 1: Run Server
Start the TCP server (from Experiment 2):
```bash
./server
```

#### Terminal 2: Run Client
Start the TCP client:
```bash
./client
```

### Step 4: Interactive Sample Run (Terminal 2)
```text
Connected to TCP Echo Server on port 10222.
Enter message: hello
Echo from server: hello
Enter message: hiii
Echo from server: hiii
Enter message: test
Echo from server: test
```
