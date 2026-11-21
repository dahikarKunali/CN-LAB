# Experiment No. 9: Traffic Shaping Algorithms (Leaky Bucket & Token Bucket)

---

## Objective
- To understand the principles of network traffic shaping and congestion control at router interfaces.
- To implement the **Leaky Bucket algorithm** in C, ensuring a constant, uniform output transmission rate regardless of bursty packet arrival.
- To implement the **Token Bucket algorithm** in C, allowing controlled bursty transmission based on accumulated token availability.

---

## Algorithms & Comparative Theory

### Part A: Leaky Bucket Algorithm (`exp9_229.c`)
- **Concept:** Acts as a buffer queue with a rigid leak rate. Regardless of how rapidly packets arrive into the bucket, packets leave the bucket at a constant rate $R$. If incoming traffic exceeds the capacity $B$, the bucket overflows and excess packets are immediately discarded.
- **Algorithm:**
  1. Set `stored = 0`.
  2. For each time unit $i = 1 \dots N$:
     - Read incoming packets $P[i]$.
     - If $\text{incoming} + \text{stored} > B$:
       - Dropped packets $= \text{incoming} + \text{stored} - B$.
       - `stored = B`.
     - Else: $\text{stored} += \text{incoming}$.
     - If $\text{stored} \le R$:
       - Packets sent $= \text{stored}$; $\text{stored} = 0$.
     - Else:
       - Packets sent $= R$; $\text{stored} -= R$.
  3. Drain all remaining packets in buffer at rate $R$ until empty.

### Part B: Token Bucket Algorithm (`exp9_2_229.c`)
- **Concept:** Tokens are continuously generated at a constant rate $R$ up to bucket capacity $B$. A packet can only be transmitted if an equivalent token is available. Idle periods allow tokens to accumulate, permitting bursts of packets up to the available token limit.
- **Algorithm:**
  1. Set `tokens = 0`.
  2. For each time unit:
     - Generate $R$ tokens: $\text{tokens} = \min(\text{tokens} + R, B)$.
     - Read packets to send $P[i]$.
     - If $\text{tokens} \ge P[i]$:
       - Transmit all $P[i]$ packets; $\text{tokens} -= P[i]$.
     - Else:
       - Transmit `tokens` packets.
       - Discard excess: $\text{dropped} = P[i] - \text{tokens}$.
       - $\text{tokens} = 0$.

---

## Extracted Source Codes

### 1. Leaky Bucket Simulation (`exp9_229.c`)
```c
#include <stdio.h>

int main() {
    int bucket_size, n, i, incoming, outgoing, stored = 0;

    printf("Leaky Bucket Algorithm Simulation\n");
    printf("Enter bucket size (capacity): ");
    scanf("%d", &bucket_size);

    printf("Enter outgoing rate: ");
    scanf("%d", &outgoing);

    printf("Enter number of packets (time units): ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("\nTime unit %d:\n", i + 1);
        printf("Enter number of incoming packets: ");
        scanf("%d", &incoming);

        if (incoming + stored > bucket_size) {
            printf("Bucket overflow! Packet loss = %d\n", incoming + stored - bucket_size);
            stored = bucket_size;
        } else {
            stored += incoming;
        }

        if (stored <= outgoing) {
            printf("Packets sent: %d\n", stored);
            stored = 0;
        } else {
            printf("Packets sent: %d\n", outgoing);
            stored -= outgoing;
        }

        printf("Packets remaining in bucket: %d\n", stored);
    }

    // Drain remaining packets
    while (stored > 0) {
        printf("\nDraining remaining packets...\n");
        if (stored <= outgoing) {
            printf("Packets sent: %d\n", stored);
            stored = 0;
        } else {
            printf("Packets sent: %d\n", outgoing);
            stored -= outgoing;
        }
        printf("Packets left in bucket: %d\n", stored);
    }

    printf("\nSimulation complete.\n");
    return 0;
}
```

### 2. Token Bucket Simulation (`exp9_2_229.c`)
```c
#include <stdio.h>

int main() {
    int bucket_size, n, rate, tokens = 0, incoming, i;

    printf("Token Bucket Algorithm Simulation\n");
    printf("Enter bucket size (capacity): ");
    scanf("%d", &bucket_size);

    printf("Enter token generation rate: ");
    scanf("%d", &rate);

    printf("Enter number of packets (time units): ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("\nTime unit %d:\n", i + 1);

        tokens += rate;
        if (tokens > bucket_size) {
            tokens = bucket_size;
        }

        printf("Enter number of packets to send: ");
        scanf("%d", &incoming);

        if (tokens >= incoming) {
            printf("Packets sent: %d\n", incoming);
            tokens -= incoming;
        } else {
            printf("Only %d tokens available, packets sent: %d, packets dropped: %d\n",
                   tokens, tokens, incoming - tokens);
            tokens = 0;
        }

        printf("Tokens left: %d\n", tokens);
    }

    printf("\nSimulation complete.\n");
    return 0;
}
```

---

## Observation Table

| Step | Algorithm / Parameters | Observations |
| :---: | :--- | :--- |
| **1** | Leaky Bucket: Cap 10, Out rate 3, Inputs: [4, 3, 1] | Time 1: Sent 3, Rem 1<br>Time 2: Sent 3, Rem 1<br>Time 3: Sent 2, Rem 0 |
| **2** | Token Bucket: Cap 10, Token rate 4, Inputs: [3, 3, 9] | Time 1: Sent 3, Tokens 1<br>Time 2: Sent 3, Tokens 2<br>Time 3: Sent 6, Dropped 3, Tokens 0 |

---

## Conclusion
- Leaky bucket enforces an uncompromising constant transmission rate, discarding traffic peaks.
- Token bucket accommodates short-term traffic bursts without packet drops as long as stored tokens suffice.

---

## How to Execute in Ubuntu Terminal

### 1. Leaky Bucket Program

#### Step 1: Create the Source File
Open the file in `gedit`, paste the Leaky Bucket code, save and close:
```bash
gedit exp9_222.c
```

#### Step 2: Compile and Run
```bash
gcc exp9_229.c -o exp9_229
./exp9_229
```

#### Sample Output:
```text
Leaky Bucket Algorithm Simulation
Enter bucket size (capacity): 10
Enter outgoing rate: 3
Enter number of packets (time units): 3

Time unit 1:
Enter number of incoming packets: 4
Packets sent: 3
Packets remaining in bucket: 1

Time unit 2:
Enter number of incoming packets: 3
Packets sent: 3
Packets remaining in bucket: 1

Time unit 3:
Enter number of incoming packets: 1
Packets sent: 2
Packets remaining in bucket: 0

Simulation complete.
```

---

### 2. Token Bucket Program

#### Step 1: Create the Source File
Open the file in `gedit`, paste the Token Bucket code, save and close:
```bash
gedit exp9_2_222.c
```

#### Step 2: Compile and Run
```bash
gcc exp9_2_229.c -o exp9_2_229
./exp9_2_229
```

#### Sample Output:
```text
Token Bucket Algorithm Simulation
Enter bucket size (capacity): 10
Enter token generation rate: 4
Enter number of packets (time units): 3

Time unit 1:
Enter number of packets to send: 3
Packets sent: 3
Tokens left: 1

Time unit 2:
Enter number of packets to send: 3
Packets sent: 3
Tokens left: 2

Time unit 3:
Enter number of packets to send: 9
Only 6 tokens available, packets sent: 6, packets dropped: 3
Tokens left: 0

Simulation complete.
```
