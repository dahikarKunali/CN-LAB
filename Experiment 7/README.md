# Experiment No. 7: Application Layer Packet Analysis using Packet Tracer / Wireshark (DNS & HTTP)

## Aim
To analyze Application Layer packets (**DNS** & **HTTP**) using a packet analysis tool (Wireshark) and understand their protocol structures, header fields, and client-server request-response lifecycles.

---

## Tools Required
- Wireshark (Packet Analyzer) / `tshark`
- Web Browser or CLI HTTP Client (`curl`, `wget`)
- DNS query utility (`nslookup`, `dig`)
- Internet Connection

---

## Procedure

### Part A: DNS Packet Analysis
1. Start Wireshark and begin capturing on active interface.
2. Open terminal and issue a DNS query:
   ```bash
   nslookup www.google.com
   ```
3. Stop capture in Wireshark and apply filter:
   ```text
   dns
   ```
4. Observe:
   - **Transaction ID** (Query matching Response)
   - **Flags:** Standard query (0x0100) vs Standard response (0x8180)
   - **Query Name:** `www.google.com`
   - **Query Type:** A (IPv4)
   - **Resolved IP Address and TTL** in Answers section.

### Part B: HTTP Packet Analysis
1. Start Wireshark capture.
2. Generate an unencrypted HTTP/1.1 request (plain HTTP port 80):
   ```bash
   curl -I http://example.com
   ```
   *(Or browse to `http://neverssl.com`)*
3. Stop capture and apply filter:
   ```text
   http
   ```
4. Inspect:
   - **HTTP GET Request:** Method (`GET`), Request URI (`/`), Request Version (`HTTP/1.1`), Host header (`example.com`), User-Agent header.
   - **HTTP Response:** Status Code (`200 OK`), Server header, Content-Type (`text/html`), Content-Length.

---

## Comparative Observation Table

| Protocol Field | DNS Query | DNS Response | HTTP Request | HTTP Response |
| :---: | :---: | :---: | :---: | :---: |
| **Transport Protocol** | UDP | UDP | TCP | TCP |
| **Destination Port** | 53 | Ephemeral Client Port | 80 | Ephemeral Client Port |
| **Method / Type** | Standard Query (A) | Response (Answers) | GET / | 200 OK |
| **Host / Target** | `www.google.com` | e.g., `142.250.77.68` | `example.com` | IP of `example.com` |
| **TTL / Cache** | - | 300 seconds | - | Cache-Control / Age |
| **Content Type** | - | - | - | `text/html; charset=UTF-8` |

---

## Conclusion
- DNS translates domain names into routable IP addresses over **UDP port 53** with minimal latency.
- HTTP operates over **TCP port 80**, utilizing full 3-way handshakes to deliver guaranteed, structured application content with explicit status headers (e.g., `200 OK`).
- Wireshark captures enable comprehensive verification of protocol boundaries between Transport and Application layers.

---

## How to Execute in Ubuntu Terminal

### Method A: Terminal Packet Capture with `tshark`
Open two terminal windows:

**Terminal 1 (Capture DNS and HTTP traffic):**
```bash
sudo tshark -i any -Y "dns || http" -T fields -e frame.number -e _ws.col.Protocol -e ip.src -e ip.dst -e _ws.col.Info
```

**Terminal 2 (Generate DNS and HTTP Traffic):**
```bash
# 1. DNS Query
nslookup www.google.com

# 2. HTTP Request (Plain HTTP port 80)
curl -I http://example.com
```

---

### Method B: Graphical Packet Analysis with Wireshark
1. Launch Wireshark:
   ```bash
   sudo wireshark &
   ```
2. Select active network interface and begin capturing packets.
3. In terminal, trigger traffic:
   ```bash
   nslookup www.google.com
   curl -I http://example.com
   ```
4. Stop capture.
5. In the filter bar:
   - Type `dns` to inspect DNS query and response packets on UDP port 53.
   - Type `http` to inspect HTTP GET requests and `200 OK` responses on TCP port 80.
