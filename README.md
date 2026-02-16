# Patricia Trie (Radix Tree) in C++

## Overview
This project is a C++ implementation of a **Patricia Trie** (Practical Algorithm to Retrieve Information Coded in Alphanumeric), also known as a **Radix Tree**.

Unlike a standard Trie which stores one character per node, this implementation compresses the tree by storing **strings as edges**. This optimization significantly reduces memory usage and tree height, making lookups faster for sparse datasets.

## Key Features
* **Path Compression:** Collapses single-branch paths into single edges (e.g., storing "car" in one node instead of `c` -> `a` -> `r`).
* **Space Optimization:** Uses `std::map<string, Node*>` to dynamically store variable-length edge labels.
* **Core Operations:** Supports `Insertion`, `Search` (Recursive & Iterative), and `Deletion`.
* **Visual Display:** Includes a utility to print the tree structure to the console for easy debugging and visualization.

## How It Works
The core logic relies on the **Longest Common Prefix (LCP)**.

1.  **Nodes & Edges:** Instead of an array of 26 characters, every Node contains a Map where the keys are **strings**.
2.  **Insertion Logic:**
    * When inserting a word (e.g., "card"), the algorithm compares it against existing edges (e.g., "car").
    * It finds the **LCP** ("car").
    * If the edge matches fully, it continues down.
    * If the edge matches *partially*, it **splits** the existing edge into two branches (e.g., "car" stays, and "d" becomes a new child).

## Code Structure
* **`struct Node`:** Represents a node in the tree. Contains:
    * `children`: A map mapping edge strings to child nodes.
    * `isEnd`: A boolean flag marking the end of a valid word.
* **`getLCP(string a, string b)`:** Utility function to find the common starting characters between two strings.
* **`insert(string word)`:** Handles the logic of splitting edges and creating new branches.
* **`search(string word)`:** Traverses the compressed path to find a word.
* **`display()`:** Recursively prints the tree structure with indentation.

## Compilation & Usage

### 1. Compile the Code
You can compile this using any standard C++ compiler (like g++).

```bash
g++ -o patricia_trie main.cpp