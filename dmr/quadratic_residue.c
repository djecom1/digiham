#include "quadratic_residue.h"

/* generator matrix from B.3.2 of the etsi specification:
1 0 0 0 0 0 0 0 0 1 0 0 1 1 1 1
0 1 0 0 0 0 0 1 0 0 0 1 1 1 1 0
0 0 1 0 0 0 0 1 1 0 1 1 0 1 1 1
0 0 0 1 0 0 0 1 1 1 1 0 0 0 1 0
0 0 0 0 1 0 0 1 1 1 0 0 1 0 0 1
0 0 0 0 0 1 0 0 1 1 1 0 0 1 0 1
0 0 0 0 0 0 1 0 0 1 1 1 0 0 1 1
*/

/* parity matrix derived from the above:
0 1 1 1 1 0 0 1 0 0 0 0 0 0 0 0
0 0 1 1 1 1 0 0 1 0 0 0 0 0 0 0
1 0 0 1 1 1 1 0 0 1 0 0 0 0 0 0
0 0 1 1 0 1 1 0 0 0 1 0 0 0 0 0
0 1 1 0 0 0 1 0 0 0 0 1 0 0 0 0
1 1 0 0 1 0 0 0 0 0 0 0 1 0 0 0
1 1 1 0 0 1 0 0 0 0 0 0 0 1 0 0
1 1 1 1 0 0 1 0 0 0 0 0 0 0 1 0
1 0 1 0 1 1 1 0 0 0 0 0 0 0 0 1
*/
// parity matrix as code:
uint16_t qr_parity_matrix[9] = {
    0b0111100100000000,
    0b0011110010000000,
    0b1001111001000000,
    0b0011011000100000,
    0b0110001000010000,
    0b1100100000001000,
    0b1110010000000100,
    0b1111001000000010,
    0b1010111000000001
};

struct correction {
    uint16_t syndrome;
    uint16_t error_pattern;
};

// generated by applying possible combinations of 1- and 2-bit errors
static struct correction corrections[] = {
    { 1, 1 },
    { 3, 3 },
    { 5, 5 },
    { 9, 9 },
    { 17, 17 },
    { 33, 33 },
    { 65, 65 },
    { 129, 129 },
    { 257, 257 },
    { 114, 513 },
    { 228, 1025 },
    { 456, 2049 },
    { 483, 4097 },
    { 438, 8193 },
    { 287, 16385 },
    { 78, 32769 },
    { 2, 2 },
    { 3, 3 },
    { 6, 6 },
    { 10, 10 },
    { 18, 18 },
    { 34, 34 },
    { 66, 66 },
    { 130, 130 },
    { 258, 258 },
    { 113, 514 },
    { 231, 1026 },
    { 459, 2050 },
    { 480, 4098 },
    { 437, 8194 },
    { 284, 16386 },
    { 77, 32770 },
    { 4, 4 },
    { 5, 5 },
    { 6, 6 },
    { 12, 12 },
    { 20, 20 },
    { 36, 36 },
    { 68, 68 },
    { 132, 132 },
    { 260, 260 },
    { 119, 516 },
    { 225, 1028 },
    { 461, 2052 },
    { 486, 4100 },
    { 435, 8196 },
    { 282, 16388 },
    { 75, 32772 },
    { 8, 8 },
    { 9, 9 },
    { 10, 10 },
    { 12, 12 },
    { 24, 24 },
    { 40, 40 },
    { 72, 72 },
    { 136, 136 },
    { 264, 264 },
    { 123, 520 },
    { 237, 1032 },
    { 449, 2056 },
    { 490, 4104 },
    { 447, 8200 },
    { 278, 16392 },
    { 71, 32776 },
    { 16, 16 },
    { 17, 17 },
    { 18, 18 },
    { 20, 20 },
    { 24, 24 },
    { 48, 48 },
    { 80, 80 },
    { 144, 144 },
    { 272, 272 },
    { 99, 528 },
    { 245, 1040 },
    { 473, 2064 },
    { 498, 4112 },
    { 423, 8208 },
    { 270, 16400 },
    { 95, 32784 },
    { 32, 32 },
    { 33, 33 },
    { 34, 34 },
    { 36, 36 },
    { 40, 40 },
    { 48, 48 },
    { 96, 96 },
    { 160, 160 },
    { 288, 288 },
    { 83, 544 },
    { 197, 1056 },
    { 489, 2080 },
    { 450, 4128 },
    { 407, 8224 },
    { 318, 16416 },
    { 111, 32800 },
    { 64, 64 },
    { 65, 65 },
    { 66, 66 },
    { 68, 68 },
    { 72, 72 },
    { 80, 80 },
    { 96, 96 },
    { 192, 192 },
    { 320, 320 },
    { 51, 576 },
    { 165, 1088 },
    { 393, 2112 },
    { 418, 4160 },
    { 503, 8256 },
    { 350, 16448 },
    { 15, 32832 },
    { 128, 128 },
    { 129, 129 },
    { 130, 130 },
    { 132, 132 },
    { 136, 136 },
    { 144, 144 },
    { 160, 160 },
    { 192, 192 },
    { 384, 384 },
    { 243, 640 },
    { 101, 1152 },
    { 329, 2176 },
    { 354, 4224 },
    { 311, 8320 },
    { 414, 16512 },
    { 207, 32896 },
    { 256, 256 },
    { 257, 257 },
    { 258, 258 },
    { 260, 260 },
    { 264, 264 },
    { 272, 272 },
    { 288, 288 },
    { 320, 320 },
    { 384, 384 },
    { 371, 768 },
    { 485, 1280 },
    { 201, 2304 },
    { 226, 4352 },
    { 183, 8448 },
    { 30, 16640 },
    { 335, 33024 },
    { 115, 512 },
    { 114, 513 },
    { 113, 514 },
    { 119, 516 },
    { 123, 520 },
    { 99, 528 },
    { 83, 544 },
    { 51, 576 },
    { 243, 640 },
    { 371, 768 },
    { 150, 1536 },
    { 442, 2560 },
    { 401, 4608 },
    { 452, 8704 },
    { 365, 16896 },
    { 60, 33280 },
    { 229, 1024 },
    { 228, 1025 },
    { 231, 1026 },
    { 225, 1028 },
    { 237, 1032 },
    { 245, 1040 },
    { 197, 1056 },
    { 165, 1088 },
    { 101, 1152 },
    { 485, 1280 },
    { 150, 1536 },
    { 300, 3072 },
    { 263, 5120 },
    { 338, 9216 },
    { 507, 17408 },
    { 170, 33792 },
    { 457, 2048 },
    { 456, 2049 },
    { 459, 2050 },
    { 461, 2052 },
    { 449, 2056 },
    { 473, 2064 },
    { 489, 2080 },
    { 393, 2112 },
    { 329, 2176 },
    { 201, 2304 },
    { 442, 2560 },
    { 300, 3072 },
    { 43, 6144 },
    { 126, 10240 },
    { 215, 18432 },
    { 390, 34816 },
    { 482, 4096 },
    { 483, 4097 },
    { 480, 4098 },
    { 486, 4100 },
    { 490, 4104 },
    { 498, 4112 },
    { 450, 4128 },
    { 418, 4160 },
    { 354, 4224 },
    { 226, 4352 },
    { 401, 4608 },
    { 263, 5120 },
    { 43, 6144 },
    { 85, 12288 },
    { 252, 20480 },
    { 429, 36864 },
    { 439, 8192 },
    { 438, 8193 },
    { 437, 8194 },
    { 435, 8196 },
    { 447, 8200 },
    { 423, 8208 },
    { 407, 8224 },
    { 503, 8256 },
    { 311, 8320 },
    { 183, 8448 },
    { 452, 8704 },
    { 338, 9216 },
    { 126, 10240 },
    { 85, 12288 },
    { 169, 24576 },
    { 504, 40960 },
    { 286, 16384 },
    { 287, 16385 },
    { 284, 16386 },
    { 282, 16388 },
    { 278, 16392 },
    { 270, 16400 },
    { 318, 16416 },
    { 350, 16448 },
    { 414, 16512 },
    { 30, 16640 },
    { 365, 16896 },
    { 507, 17408 },
    { 215, 18432 },
    { 252, 20480 },
    { 169, 24576 },
    { 337, 49152 },
    { 79, 32768 },
    { 78, 32769 },
    { 77, 32770 },
    { 75, 32772 },
    { 71, 32776 },
    { 95, 32784 },
    { 111, 32800 },
    { 15, 32832 },
    { 207, 32896 },
    { 335, 33024 },
    { 60, 33280 },
    { 170, 33792 },
    { 390, 34816 },
    { 429, 36864 },
    { 504, 40960 },
    { 337, 49152 }
};

uint16_t quadratic_residue_parity(uint16_t* data) {
    // check emb header
    uint16_t parity = 0;

    uint8_t k;
    for (k = 0; k < 9; k++) {
        uint8_t bit = 0, l;
        for (l = 0; l < 16; l++) {
            if ((qr_parity_matrix[k] >> l) & 1) {
                bit ^= ((*data >> l) & 1);
            }
        }

        parity = (parity << 1) | (bit & 1);
    }

    return parity;
}

bool quadratic_residue(uint16_t* data) {
    uint16_t parity = quadratic_residue_parity(data);

    if (parity == 0) return true;

    int num_corrections = sizeof(corrections)/sizeof(corrections[0]);
    for (int i = 0; i < num_corrections; i++) {
        if (corrections[i].syndrome == parity) {
            *data = *data ^ corrections[i].error_pattern;
            return true;
        }
    }

    return false;
}
