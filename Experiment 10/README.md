# Experiment No. 10: Distance Vector Routing Algorithm

---

## Objective
- To understand how routers cooperatively share routing updates and distance vectors to determine the shortest communication paths across a network topology.
- To implement the **Distance Vector Routing algorithm** based on the distributed **Bellman-Ford equation** in C.
- To compute and display the complete routing table (Destination, Next Hop, and Minimum Distance) for every router node upon convergence.

---

## Theory & Bellman-Ford Formulation
In distance vector routing, each router $i$ maintains a distance vector $DV_i$ where $DV_i[j]$ represents the currently estimated minimum distance to destination node $j$.
Routers iteratively exchange their distance vectors with directly connected neighbors. The shortest path is computed via relaxation:

$$DV_i[j] = \min_k \{ C[i][k] + DV_k[j] \}$$

where:
- $C[i][k]$ is the link cost from node $i$ to direct neighbor $k$.
- $DV_k[j]$ is the advertised distance from neighbor $k$ to destination $j$.
- If an updated path through neighbor $k$ offers lower cost, router $i$ sets $DV_i[j]$ to this new value and sets `next_hop = k`.
- The process repeats for at most $N-1$ iterations or until no table updates occur (convergence).

---

## Extracted Source Code
File: `exp10_229.c`
```c
#include <stdio.h>
#define MAX_NODES 10
#define INF 9999

struct Node {
    int dist[MAX_NODES];
    int from[MAX_NODES];
} rt[MAX_NODES];

int main() {
    int cost[MAX_NODES][MAX_NODES], nodes, i, j, k;
    int updated, iteration;

    printf("Enter number of nodes: ");
    scanf("%d", &nodes);

    printf("Enter cost matrix row by row (use 999 for INF/no connection):\n");
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            int v;
            scanf("%d", &v);
            if (v == 999) cost[i][j] = INF;
            else cost[i][j] = v;
        }
    }

    for (i = 0; i < nodes; i++) {
        cost[i][i] = 0;
        for (j = 0; j < nodes; j++) {
            rt[i].dist[j] = cost[i][j];
            if (cost[i][j] < INF && i != j) rt[i].from[j] = j; // next hop is neighbor j
            else rt[i].from[j] = -1; // unknown / unreachable
        }
    }

    /* Repeat relaxations at most nodes-1 times (Bellman-Ford style) */
    for (iteration = 0; iteration < nodes - 1; iteration++) {
        updated = 0;
        for (i = 0; i < nodes; i++) {
            for (j = 0; j < nodes; j++) {
                for (k = 0; k < nodes; k++) {
                    if (cost[i][k] >= INF) continue; // no direct link i->k
                    if (rt[k].dist[j] >= INF) continue; // k->j not known
                    int newdist = cost[i][k] + rt[k].dist[j];
                    if (newdist < rt[i].dist[j]) {
                        rt[i].dist[j] = newdist;
                        rt[i].from[j] = k;
                        updated = 1;
                    }
                }
            }
        }
        if (!updated) break;
    }

    // Display routing tables
    for (i = 0; i < nodes; i++) {
        printf("\nRouting table for Node %d:\n", i);
        printf("Destination\tNext Hop\tDistance\n");
        for (j = 0; j < nodes; j++) {
            if (rt[i].dist[j] >= INF)
                printf("%d\t\t%s\t\t%s\n", j, "-", "INF");
            else
                printf("%d\t\t%d\t\t%d\n", j, rt[i].from[j], rt[i].dist[j]);
        }
    }

    return 0;
}
```

---

## Observation Table

| Input Cost Matrix | Node 0 Routing Table | Node 1 Routing Table | Node 2 Routing Table |
| :---: | :---: | :---: | :---: |
| `0 2 7`<br>`2 0 1`<br>`7 1 0` | Dest 0: Hop -1, Cost 0<br>Dest 1: Hop 1, Cost 2<br>Dest 2: Hop 1, Cost 3 | Dest 0: Hop 0, Cost 2<br>Dest 1: Hop -1, Cost 0<br>Dest 2: Hop 2, Cost 1 | Dest 0: Hop 1, Cost 3<br>Dest 1: Hop 1, Cost 1<br>Dest 2: Hop -1, Cost 0 |

*Notice that direct link from Node 0 to Node 2 has cost 7, but via Node 1 the cost is $2 + 1 = 3$. The algorithm discovers this optimal indirect path.*

---

## Conclusion
The Distance Vector Routing algorithm correctly iterates through local neighbor exchanges to converge on globally optimal shortest paths without requiring any single router to possess full global network topology knowledge.

---

## How to Execute in Ubuntu Terminal

### Step 1: Create the Source File
Open the file in `gedit`, paste the Distance Vector Routing code, save and close:
```bash
gedit exp10_229.c
```

### Step 2: Compile the Program
```bash
gcc exp10_229.c -o exp10_229
```

### Step 3: Run the Program
```bash
./exp10_229
```

### Step 4: Interactive Sample Run
**Terminal Input:**
```text
Enter number of nodes: 3
Enter cost matrix row by row (use 999 for INF/no connection):
0 2 7
2 0 1
7 1 0
```

**Terminal Output:**
```text
Routing table for Node 0:
Destination     Next Hop        Distance
0               -1              0
1               1               2
2               1               3

Routing table for Node 1:
Destination     Next Hop        Distance
0               0               2
1               -1              0
2               2               1

Routing table for Node 2:
Destination     Next Hop        Distance
0               1               3
1               1               1
2               -1              0
```
