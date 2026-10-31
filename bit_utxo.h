
#ifndef BIT_UTXO_H
#define BIT_UTXO_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *data;
    size_t bit_count;
} BitArray;

// Initialize an empty bit array
void init(BitArray *array);

// Append a bit (0 or 1)
int append_bit(BitArray *array, int bit);

// Remove the final bit
int remove_bit(BitArray *array);

// Read a bit at a specified position
// Returns -1 if the position is invalid
int read_bit(const BitArray *array, size_t position);

// Save the bit array to a binary file
int save_bits(const BitArray *array, const char *filename);

// Load a bit array from a binary file
int load_bits(BitArray *array, const char *filename);

// Release allocated memory
void destroy(BitArray *array);

#endif
