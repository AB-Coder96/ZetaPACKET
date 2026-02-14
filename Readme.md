# Core

- C++20 (userspace ingest loop + generator)

- CMake

- Linux-only

- libbpf / bpftool (for XDP program build/loading)

- AF_XDP with UMEM + RX rings

# Tooling

- clang for eBPF/XDP program (if you include the kernel-side XDP prog)

- ip link, ethtool, tc, bpftool

- perf for profiling

- Provide a baseline kernel UDP socket ingest in same repo

- Packet generator with sequence numbers

- CSV export for p50/p99/p99.9 + pps

# ZetaXDP — Kernel-Bypass UDP Ingest (AF_XDP)

ZetaXDP is a focused systems project exploring **kernel-bypass UDP ingest** using **AF_XDP**. The goal is to demonstrate the exact performance work HFT teams care about:

- fast, predictable packet ingest (pps + tail latency)
- correctness under load (loss detection, gaps, counters)
- reproducible benchmarks (p99/p99.9, commit tracking)
- profiling-driven tuning (perf, CPU affinity, cache effects)

> **Scope note:** ZetaXDP is a transport/ingest prototype intended for learning and measurement. It is not represented as a production trading connector.

---

## Architecture

```
NIC RX
  ↓
XDP program (attach to interface)
  ↓
UMEM frames (copy vs zero-copy)
  ↓
AF_XDP RX ring → userspace poll loop
  ↓
Parser (UDP payload decode)
  ↓
Lock-free ring buffer
  ↓
Downstream pipeline (book builder / replay / recorder)
```

### Modes
- **Baseline:** kernel UDP socket recv loop (comparison point)
- **AF_XDP copy:** AF_XDP path with copy semantics
- **AF_XDP zero-copy:** target path (when supported by driver/NIC)

---

## What it measures

### Performance
- packets/sec (pps)
- p50/p99/p99.9 ingest latency (timestamped at receive + stage timing)
- CPU utilization per thread / core
- cache miss / branch stats (perf)

### Correctness / loss diagnostics
- sequence-gap detector (per flow) if payload includes sequence numbers
- ring saturation / UMEM pressure indicators
- drop counters across layers (NIC / XDP / socket)
- jitter + timestamp skew tracker
- optional PCAP mirror/debug mode

---

## Reproducible benchmarking discipline

Every benchmark run is stored with:
- config hash (mode, batch size, busy-poll, ring sizes)
- dataset generator parameters (packet size, rate, flows)
- commit SHA
- machine profile (CPU, kernel, governor)

Recommended run checklist:
- pin threads to cores (affinity)
- warm up before measurement
- fixed payload sizes / fixed generator seeds
- release builds only
- export CSV for dashboards

---

## Quickstart (example)

### Build
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
```

### Run (baseline vs AF_XDP)
```bash
# baseline socket ingest
./ZetaXDP --mode socket --iface eth0 --port 9000 --seconds 30 --out out/socket.csv

# AF_XDP copy
./ZetaXDP --mode xdp_copy --iface eth0 --queue 0 --seconds 30 --out out/xdp_copy.csv

# AF_XDP zero-copy (if supported)
./ZetaXDP --mode xdp_zc --iface eth0 --queue 0 --seconds 30 --out out/xdp_zc.csv
```

---

## What to publish on your portfolio page

Include:
1) **pps + p99/p99.9** for socket vs XDP copy vs XDP zero-copy
2) a one-paragraph explanation of **what changed** to move the tail (affinity, batching, ring sizes, busy-poll)
3) a screenshot or snippet of a **perf top** showing the hot path
4) a “loss dashboard” screenshot proving **0.00% drops** under your test load

---

## Roadmap (credible extensions)
- add AF_XDP TX path for loopback tests
- add AF_PACKET mmap baseline (another comparison)
- add an eBPF stats exporter (periodic counters)
- integrate results into **ZetaPulse** dashboard
- add CI perf regression (store last-known-good p99)

---
