# bit_UTXO

**A compact, block-indexed UTXO status algorithm implementation using single-bit identifiers.**

[Implementation Documentation](IMPLEMENTATION.md)

## Overview

bit_UTXO uses a UTXO status storage algorithm designed to efficiently determine whether a specific UTXO, identified by its originating block number and output position within that block, has already been spent.

Every transaction output is represented by exactly **1 bit** in a continuously expanding array.

Instead of allocating a fixed amount of storage for every block, each block specifies how many outputs it creates and where their entries begin in the global array.

The array expands dynamically as new blocks are added.

The author developed this algorithm independently but makes no guarantee of its originality, uniqueness, or novelty.
## 1. Block allocation

Each block specifies two values:

- **UTXO Count:** The total number of outputs created by the block.
- **UTXO Start:** The starting position of those outputs in the global array.

For each block, the algorithm allocates exactly one bit for every output created.

**Allocation formula:**

`Allocation (bits) = UTXO Count`

The starting position is determined by the cumulative number of outputs created by all previous blocks.

**Starting position formula:**

`UTXO Start (current block) = UTXO Start (previous block) + UTXO Count (previous block)`

For the first block, the starting position is `0`.

### Example

| Block | UTXOs created | Starting position |
|---|---:|---:|
| 1 | 500 | 0 |
| 2 | 200 | 500 |
| 3 | 800 | 700 |
| 4 | 100 | 1,500 |

Each output receives its own consecutive bit position.

There is no fixed allocation per block. Blocks only consume the space required by their actual output count.

## 2. Status identifiers

Every output is assigned exactly 1 bit.

The two possible states are:

| Bit | Status |
|---|---|
| `0` | Created and unspent |
| `1` | Spent |

When an output is created, its corresponding bit is initialized to `0`.

When the output is spent by a confirmed transaction, its bit is changed to `1`.

Because the block allocation metadata already establishes which output positions exist, there is no need for a separate identifier for unallocated positions.

## 3. Locating an output

An output is identified by two values:

- Its originating block number.
- Its output position within that block.

Each block's `UTXO Start` value determines where its outputs begin in the global array.

The corresponding global position is calculated as:

`Global Index = UTXO Start + Output Index`

Because each entry occupies 1 bit:

`Bit Position = Global Index`

A node can use these values to directly access the corresponding status bit.

This allows a node to efficiently determine whether a specific output from a specific block has already been spent, without searching through the full transaction history.

## 4. Array expansion

The array expands as new blocks are accepted.

Each block adds exactly one bit for every output it creates.

Eight UTXO statuses fit into a single byte.

This variable-length allocation avoids reserving unused space for blocks that create fewer outputs.

### Storage efficiency

| Total outputs | Storage required |
|---:|---:|
| 1 million | 125 KB |
| 10 million | 1.25 MB |
| 100 million | 12.5 MB |
| 1 billion | 125 MB |

These figures represent the raw status array, excluding allocation metadata and other storage overhead.

## 5. Optional cryptographic commitment

A cryptographic hash can be calculated over the array, or over individual sections of it, to produce a commitment to the latest list of UTXO statuses.

For example, SHA-256 could be used to produce a 32-byte digest.

This commitment could be included in a block header or another authenticated structure, allowing nodes to verify that the status data corresponds to an expected state.

The exact commitment construction and update mechanism are implementation-dependent.

---

**Core principle:** Every transaction output receives exactly 1 bit in a global array. Each block specifies how many outputs it creates and where their entries begin. By combining the originating block number with the output's position within that block, nodes can efficiently determine whether that particular UTXO has already been spent.
