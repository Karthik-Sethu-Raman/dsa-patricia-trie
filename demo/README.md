# 🚀 Firewall Using Patricia Trie (Compressed Radix Tree)

A C++ implementation of a **Patricia Trie (Practical Algorithm to Retrieve Information Coded in Alphanumeric)** demonstrating **Longest Prefix Match (LPM)** for firewall rule processing and IP routing.

This project simulates how routers and firewalls efficiently match IP prefixes using path compression instead of storing every bit individually.

---

## 📌 Problem Statement

Firewalls and routers must quickly decide:

> Given an IP address, which rule applies?

Rules are typically stored as prefixes:

192.0.0.0/8       → ALLOW  
192.168.1.55/32   → BLOCK  

The system must:
- Match prefixes efficiently
- Return the most specific rule
- Minimize memory usage
- Scale for large rule sets

---

## 🧠 Why Patricia Trie?

A normal Trie:
- Stores one node per bit
- Consumes large memory for shared prefixes
- Slower due to deep traversal

A **Patricia Trie**:
- Compresses consecutive bits into a single edge
- Reduces memory consumption
- Speeds up lookup
- Enables efficient Longest Prefix Matching

---

## 🔍 Concepts Demonstrated

### 1️⃣ Path Compression

Instead of storing:

1 → 1 → 0 → 0 → 0 → 0 → 0 → 0  

We store:

"11000000"

This drastically reduces node count.

---

### 2️⃣ Longest Prefix Match (LPM)

If both rules match:

192.0.0.0/8        → ALLOW  
192.168.1.55/32    → BLOCK  

Packet:
192.168.1.55  

The /32 rule wins because it is more specific.

---

## 🏗 Project Structure

### PatriciaNode
- key → Compressed bit string
- is_terminal → Marks rule endpoint
- data → Action (ALLOW / BLOCK)
- children → Branches (0 or 1)

### FirewallRadixTree
- insert() → Adds rule with compression
- search() → Performs Longest Prefix Match
- printTree() → Visualizes structure

---

## ⚙️ How It Works

### Insertion Algorithm

1. Compare binary string with existing edges.
2. If full match → traverse down.
3. If partial match → split node.
4. If no match → create new compressed edge.

### Search Algorithm (LPM)

1. Traverse matching edges.
2. Track the last terminal node encountered.
3. Return the most specific matching rule.

---

## ▶️ Build and Run

### Compile

```bash
g++ patricia.cpp -o patricia
