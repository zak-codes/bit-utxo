
#include "bit_utxo.h"

#include <stdlib.h>
#include <stdio.h>

// Initialize an empty bit array
void init(BitArray *array) {
    array->data = NULL;
    array->bit_count = 0;
}

// Append one bit (0 or 1)
int append_bit(BitArray *array, int bit) {
    if (bit != 0 && bit != 1)
        return 0;

    size_t byte_index = array->bit_count / 8;
    size_t bit_index = array->bit_count % 8;

    if (bit_index == 0) {
        uint8_t *new_data = realloc(
            array->data, byte_index + 1
        );

        if (new_data == NULL)
            return 0;

        array->data = new_data;
        array->data[byte_index] = 0;
    }

    if (bit == 1) {
        array->data[byte_index] |=
            (uint8_t)(1u << bit_index);
    }

    array->bit_count++;

    return 1;
}

// Remove the final bit
int remove_bit(BitArray *array) {
    if (array->bit_count == 0)
        return 0;

    array->bit_count--;

    if (array->bit_count % 8 == 0) {
        size_t new_size = array->bit_count / 8;

        if (new_size == 0) {
            free(array->data);
            array->data = NULL;
        } else {
            uint8_t *new_data = realloc(
                array->data, new_size
            );

            if (new_data != NULL)
                array->data = new_data;
        }
    } else {
        size_t byte_index = array->bit_count / 8;
        size_t bit_index = array->bit_count % 8;

        array->data[byte_index] &=
            (uint8_t)~(1u << bit_index);
    }

    return 1;
}

// Read a bit at a specific position
int read_bit(const BitArray *array, size_t position) {
    if (position >= array->bit_count)
        return -1;

    size_t byte_index = position / 8;
    size_t bit_index = position % 8;

    return (array->data[byte_index] >> bit_index) & 1;
}

// Save the bit array to a binary file
int save_bits(const BitArray *array, const char *filename) {
    FILE *file = fopen(filename, "wb");

    if (file == NULL)
        return 0;

    uint64_t count = (uint64_t)array->bit_count;

    // Write the bit count as 8 little-endian bytes
    for (int i = 0; i < 8; i++) {
        if (fputc((int)((count >> (i * 8)) & 0xFF), file) == EOF) {
            fclose(file);
            return 0;
        }
    }

    // Calculate the number of bytes required
    size_t byte_count = array->bit_count / 8 +
                        (array->bit_count % 8 != 0);

    // Write the packed binary data
    if (byte_count > 0 &&
        fwrite(array->data, 1, byte_count, file) != byte_count) {
        fclose(file);
        return 0;
    }

    return fclose(file) == 0;
}

// Load a bit array from a binary file
int load_bits(BitArray *array, const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
        return 0;

    uint64_t count = 0;

    // Read the 8-byte little-endian bit count
    for (int i = 0; i < 8; i++) {
        int byte = fgetc(file);

        if (byte == EOF) {
            fclose(file);
            return 0;
        }

        count |= (uint64_t)(uint8_t)byte << (i * 8);
    }

    // Check whether the bit count fits in size_t
    if (sizeof(size_t) < sizeof(uint64_t) &&
        count > (uint64_t)SIZE_MAX) {
        fclose(file);
        return 0;
    }

    size_t bit_count = (size_t)count;

    // Calculate the required number of bytes
    size_t byte_count = bit_count / 8 +
                        (bit_count % 8 != 0);

    uint8_t *new_data = NULL;

    // Allocate memory for the bit data
    if (byte_count > 0) {
        new_data = malloc(byte_count);

        if (new_data == NULL) {
            fclose(file);
            return 0;
        }

        // Read the packed binary data
        if (fread(new_data, 1, byte_count, file) != byte_count) {
            free(new_data);
            fclose(file);
            return 0;
        }
    }

    // Check for file read errors
    if (ferror(file)) {
        free(new_data);
        fclose(file);
        return 0;
    }

    fclose(file);

    // Replace the existing array
    free(array->data);

    array->data = new_data;
    array->bit_count = bit_count;

    return 1;
}

// Release allocated memory
void destroy(BitArray *array) {
    free(array->data);

    array->data = NULL;
    array->bit_count = 0;
}
