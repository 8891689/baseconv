// Author: 8891689
#ifndef BIGINT_H
#define BIGINT_H

#include <stdbool.h> // For bool type
#include <stddef.h>  // For size_t type

// --- Big Integer Implementation ---

// Define the maximum size for the big integer (adjust if needed)
#define MAX_DIGITS 1024

// Structure to hold big integer
typedef struct {
    int digits[MAX_DIGITS]; // Stores digits in reverse order (digits[0] is least significant)
    int size;               // Number of digits used
    bool negative;          // Sign
} BigInt;

// --- Forward declarations for BigInt functions ---

// Initialization and Setup
void initBigInt(BigInt *num);
void setBigIntFromString(BigInt *num, const char *str);
void setBigIntFromInt(BigInt *num, long long val);

// Input/Output
void printBigInt(const BigInt *num);
void bigIntToString(const BigInt *num, char *str, size_t str_size); // Added size parameter

// Comparison and Checks
int compareBigInt(const BigInt *a, const BigInt *b);
bool isBigIntZero(const BigInt *num);

// Arithmetic Operations
void addBigInt(const BigInt *a, const BigInt *b, BigInt *result); // Simplified version (handles non-negatives mainly)
void multiplyBigIntByInt(const BigInt *a, int b, BigInt *result);
int divideBigIntByInt(const BigInt *n, int divisor, BigInt *quotient); // Returns remainder

#endif // BIGINT_H
