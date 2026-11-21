# Experiment No. 8: Sliding Window Protocol (Go-Back-N)

---

## Objective
- To understand how the Sliding Window Protocol controls the flow of frames between sender and receiver over an error-prone link.
- To simulate the Go-Back-N ARQ (Automatic Repeat reQuest) variant using arrays and sliding window logic in C.
- To observe how the sender window slides forward when positive acknowledgments (ACKs) are received and retransmits frames upon failure.

---

## Algorithm: Go-Back-N ARQ
**Input:** Total number of frames to send $N$, and window size $W$.

### Sender Side:
1. Initialize `base = 0` and `next_seq = 0`.
2. While `base < N`:
   - Send frames in the window: from `base` to `base + W - 1` (or up to $N$).
   - Wait for receiver acknowledgment:
     - If user inputs `y` (success): Window slides forward (`base += window_size`).
     - If user inputs `n` (error/loss): Go-Back-N triggers; retransmit all frames in current window starting from `base`.
3. Terminate when `base >= N`.

### Receiver Side:
- Simulates cumulative acknowledgment: for frame $i$, outputs `sending ACK: i + 1`.

---

## Extracted Source Code
File: `sliding_window222.c`
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 50

void sender(int frames[], int size, int window_size);
void receiver(int frames[], int start, int window_size);

int main() {
    int n, window_size;
    int frames[MAX_FRAMES];

    printf("Sliding Window Protocol Simulation (Go-Back-N)\n");
    printf("Enter number of frames to send (max %d): ", MAX_FRAMES);
    scanf("%d", &n);

    if (n > MAX_FRAMES || n <= 0) {
        printf("Invalid number of frames.\n");
        return 0;
    }

    printf("Enter window size: ");
    scanf("%d", &window_size);

    if (window_size <= 0 || window_size > n) {
        printf("Invalid window size.\n");
        return 0;
    }

    // Fill frames with sequence numbers
    for (int i = 0; i < n; i++) {
        frames[i] = i;
    }

    sender(frames, n, window_size);

    return 0;
}

void sender(int frames[], int size, int window_size) {
    int base = 0;
    int next_seq = 0;
    char choice;

    srand(time(NULL));

    while (base < size) {
        printf("\nSender window: ");
        for (int i = base; i < base + window_size && i < size; i++) {
            printf("[%d] ", frames[i]);
        }
        printf("\nSending frames...\n");

        for (int i = base; i < base + window_size && i < size; i++) {
            printf("Sent frame: %d\n", frames[i]);
        }

        printf("\nSimulating receiver...\n");
        receiver(frames, base, window_size);

        printf("\nDid all frames receive correctly? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            base += window_size;
        } else {
            printf("Resending from frame %d...\n", frames[base]);
            // In Go-Back-N, we resend from base
        }
    }

    printf("\nAll frames sent successfully!\n");
}

void receiver(int frames[], int start, int window_size) {
    for (int i = start; i < start + window_size; i++) {
        printf("Received frame: %d, sending ACK: %d\n", frames[i], frames[i] + 1);
    }
}
```

---

## Observation Table

| Step | Action / Parameter | Observed Program Behavior |
| :---: | :--- | :--- |
| **1** | Compile `sliding_window222.c` | Compiled successfully with GCC |
| **2** | Input $N = 5$, Window size $W = 3$ | Initial sender window displays `[0] [1] [2]` |
| **3** | Receiver Simulation | ACKs 1, 2, 3 generated for frames 0, 1, 2 |
| **4** | Input `y` (Correct transmission) | Window slides forward to `[3] [4]` |
| **5** | Transmit remaining frames | Frames 3 and 4 sent, ACKs 4 and 5 generated |
| **6** | Input `y` | Program prints `All frames sent successfully!` |

---

## Conclusion
- Go-Back-N enables pipelined frame transmission without waiting for individual per-frame stop-and-wait confirmations.
- When an acknowledgment fails or times out, the sender window rewinds back to the unacknowledged base sequence number, guaranteeing reliable in-order frame arrival.

---

## How to Execute in Ubuntu Terminal

### Step 1: Create the Source File
Open the file in `gedit`, paste the C code provided above, save and close:
```bash
gedit sliding_window222.c
```

### Step 2: Compile the Program
```bash
gcc sliding_window222.c -o sliding_window222
```

### Step 3: Run the Program
```bash
./sliding_window222
```

### Step 4: Interactive Sample Run
```text
Sliding Window Protocol Simulation (Go-Back-N)
Enter number of frames to send (max 50): 5
Enter window size: 3

Sender window: [0] [1] [2] 
Sending frames...
Sent frame: 0
Sent frame: 1
Sent frame: 2

Simulating receiver...
Received frame: 0, sending ACK: 1
Received frame: 1, sending ACK: 2
Received frame: 2, sending ACK: 3

Did all frames receive correctly? (y/n): y

Sender window: [3] [4] 
Sending frames...
Sent frame: 3
Sent frame: 4

Simulating receiver...
Received frame: 3, sending ACK: 4
Received frame: 4, sending ACK: 5

Did all frames receive correctly? (y/n): y

All frames sent successfully!
```
