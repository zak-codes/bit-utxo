# Implementation

This repository contains the C implementation of the `bit_UTXO` algorithm.

## Files

- `bit_utxo.c` — Contains the implementation of all functions.
- `bit_utxo.h` — Contains the `BitArray` structure and public function declarations.

## Functions

| Function | Description |
|---|---|
| `init()` | Initializes an empty bit array. |
| `append_bit()` | Appends a `0` or `1` to the end of the array. |
| `remove_bit()` | Removes the final bit from the array. |
| `read_bit()` | Reads a bit at a specified position. |
| `save_bits()` | Saves the bit array to a binary file. |
| `load_bits()` | Loads a bit array from a binary file. |
| `destroy()` | Releases the memory allocated to the array. |

## Usage

Include `bit_utxo.h` in your C source file:

```c
#include "bit_utxo.h"
```

Compile your program alongside `bit_utxo.c`:

```bash
gcc main.c bit_utxo.c -o program
```

The module uses packed binary storage, with eight UTXO status bits stored in each byte.

## Disclaimer

This implementation is provided as-is, without warranties or guarantees. Use at your own risk. The author is not responsible for any damages, data loss, or other issues arising from its use.
