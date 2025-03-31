// Author: 8891689
#include "bigint.h" // Include the header file we just defined
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // For isdigit
#include <errno.h>
#include <limits.h>


// --- BigInt Function Definitions ---

void initBigInt(BigInt *num) {
    memset(num->digits, 0, sizeof(num->digits));
    num->size = 1;
    num->digits[0] = 0;
    num->negative = false;
}

bool isBigIntZero(const BigInt *num) {
    return (num->size == 1 && num->digits[0] == 0);
}

void setBigIntFromInt(BigInt *num, long long val) {
    initBigInt(num);
    if (val < 0) {
        num->negative = true;
        val = -val; // Work with positive value
    }
    if (val == 0) {
        return; // Already initialized to 0
    }
    int i = 0;
    while (val > 0) {
        if (i >= MAX_DIGITS) {
             fprintf(stderr, "Error: Integer too large for BigInt capacity in setBigIntFromInt.\n");
             initBigInt(num); // Reset to 0 on error
             return;
        }
        num->digits[i++] = (int)(val % 10); // Cast to int
        val /= 10;
    }
    num->size = i;
}

void setBigIntFromString(BigInt *num, const char *str) {
    initBigInt(num);
    size_t len = strlen(str); // Use size_t
    if (len == 0) {
        return; // Handle empty string
    }

    // Handle potential sign
    size_t start = 0; // Use size_t
    if (str[0] == '-') {
        num->negative = true;
        start = 1;
        if (len == 1) { // Just a '-' sign
             fprintf(stderr, "Error: Invalid number string '-'.\n");
             initBigInt(num); // Reset to 0
             return;
        }
    } else if (str[0] == '+') {
        start = 1;
         if (len == 1) { // Just a '+' sign
             fprintf(stderr, "Error: Invalid number string '+'.\n");
             initBigInt(num); // Reset to 0
             return;
         }
    }


    // Find the start of the actual digits (skip leading zeros after sign)
    size_t digit_start = start; // Use size_t
    while (digit_start < len - 1 && str[digit_start] == '0' ) {
        digit_start++;
    }

    int effective_len = (int)(len - digit_start); // Calculate effective length
    if (effective_len == 0) {
         if (digit_start > start && str[len-1] == '0') { // Handle "0", "+0", "-0", "000" etc.
             initBigInt(num); // Result is 0
             // Keep sign if it was '-' ? Usually -0 is just 0.
             // if (num->negative) {}
             return;
         } else { // Invalid if only sign or empty after trimming 0s
             fprintf(stderr, "Error: Invalid number string format '%s'.\n", str);
             initBigInt(num);
             return;
         }
    }


    // Basic validation (allow only digits in the effective part)
    for(size_t k = digit_start; k < len; k++) { // Use size_t
        if (!isdigit((unsigned char)str[k])) {
             fprintf(stderr, "Error: Invalid character '%c' in number string '%s'.\n", str[k], str);
             initBigInt(num); // Reset to 0 on error
             return;
        }
    }

    if (effective_len > MAX_DIGITS) {
        fprintf(stderr, "Error: String representation (length %d) too long for BigInt capacity (%d).\n", effective_len, MAX_DIGITS);
        initBigInt(num); // Reset to 0 on error
        return;
    }

    num->size = effective_len;
    for (int i = 0; i < effective_len; i++) {
        // Store digits in reverse order from the effective part of the string
        num->digits[i] = str[len - 1 - i] - '0';
    }
    // Keep sign from earlier check
}


void printBigInt(const BigInt *num) {
    if (num->negative && !isBigIntZero(num)) printf("-"); // Avoid printing "-0"
    if (num->size == 0) { // Should not happen with proper init, but safe check
        printf("0");
        return;
    }
    for (int i = num->size - 1; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
}

// Modified to accept buffer size
void bigIntToString(const BigInt *num, char *str, size_t str_size) {
    if (str_size == 0) return; // No space

    if (isBigIntZero(num)) {
        if (str_size >= 2) { // Need space for '0' and '\0'
            str[0] = '0';
            str[1] = '\0';
        } else {
            str[0] = '\0'; // Not enough space even for "0"
        }
        return;
    }

    size_t k = 0; // Use size_t for index comparison with str_size
    if (num->negative) {
        if (k < str_size - 1) { // Check space for '-' and '\0'
            str[k++] = '-';
        } else {
            str[0] = '\0'; // Not enough space
            return;
        }
    }

    // Calculate required length first
    // Need space for sign (if any), all digits, and null terminator
    size_t required_len = (num->negative ? 1 : 0) + (size_t)num->size + 1;
    if (required_len > str_size) {
         fprintf(stderr, "Warning: bigIntToString buffer too small (needed %zu, got %zu).\n", required_len, str_size);
         // Truncate safely - determine max digits we can write
         size_t max_write_index = str_size - 1; // Last index for non-null char
         size_t available_for_digits = max_write_index - k; // Space left after sign
         int num_digits_to_write = (num->size < (int)available_for_digits) ? num->size : (int)available_for_digits;

         for (int i = num->size - 1; i >= num->size - num_digits_to_write; --i) {
             str[k++] = num->digits[i] + '0';
         }
         str[k] = '\0'; // Null terminate truncated string
         return; // Stop here after truncation
    }


    // If buffer is large enough, write all digits
    for (int i = num->size - 1; i >= 0; i--) {
        // We already checked required_len, so k < str_size - 1 is guaranteed here
        str[k++] = num->digits[i] + '0';
    }
    str[k] = '\0'; // Null terminate
}

// Compare two BigInts a and b.
// Returns: >0 if a > b, <0 if a < b, 0 if a == b
// Handles signs.
int compareBigInt(const BigInt *a, const BigInt *b) {
    // Different signs (handle zero explicitly)
    bool a_is_zero = isBigIntZero(a);
    bool b_is_zero = isBigIntZero(b);

    // If a is negative (and not zero) and b is non-negative
    if (a->negative && !a_is_zero && !b->negative) return -1;
    // If a is non-negative and b is negative (and not zero)
    if (!a->negative && b->negative && !b_is_zero) return 1;

    // Now they have the same sign OR one or both are zero.
    // If signs are the same and negative, comparison result is reversed.
    int sign_factor = (a->negative && !a_is_zero) ? -1 : 1;

    // Compare sizes (magnitudes)
    if (a->size > b->size) return 1 * sign_factor;
    if (a->size < b->size) return -1 * sign_factor;

    // Same size, compare digit by digit from most significant
    for (int i = a->size - 1; i >= 0; i--) {
        if (a->digits[i] > b->digits[i]) return 1 * sign_factor;
        if (a->digits[i] < b->digits[i]) return -1 * sign_factor;
    }

    return 0; // They are equal
}


// NOTE: This addBigInt is simplified and assumes non-negative inputs for base conversion context.
void addBigInt(const BigInt *a, const BigInt *b, BigInt *result) {
    if (a->negative || b->negative) {
        fprintf(stderr, "Warning: addBigInt called with negative numbers; simplified version used (treats inputs as positive).\n");
    }

    BigInt temp_result; // Use temporary storage
    initBigInt(&temp_result);

    int carry = 0;
    int maxSize = (a->size > b->size) ? a->size : b->size;

    int i;
    for (i = 0; i < maxSize; i++) {
         if (i >= MAX_DIGITS) {
             fprintf(stderr, "Error: BigInt capacity exceeded during addition.\n");
             initBigInt(result); return;
         }
        int sum = (i < a->size ? a->digits[i] : 0) + (i < b->size ? b->digits[i] : 0) + carry;
        temp_result.digits[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry) {
        if (i >= MAX_DIGITS) {
             fprintf(stderr, "Error: BigInt capacity exceeded during addition carry.\n");
             initBigInt(result); return;
        }
        temp_result.digits[i] = carry;
        temp_result.size = i + 1;
    } else {
        temp_result.size = (i == 0) ? 1 : i; // Ensure size is at least 1
    }

    memcpy(result, &temp_result, sizeof(BigInt));
    result->negative = false; // Result is non-negative in this simplified context
}


void multiplyBigIntByInt(const BigInt *a, int b, BigInt *result) {
     if (b < 0) {
         fprintf(stderr, "Error: multiplyBigIntByInt doesn't support negative multiplier b in this version.\n");
         initBigInt(result); return;
     }
     if (b == 0 || isBigIntZero(a)) {
        initBigInt(result); return; // Result is 0
     }
     if (b == 1) {
        if (result != a) {
             memcpy(result, a, sizeof(BigInt));
        }
        return;
     }

    BigInt temp_result;
    initBigInt(&temp_result);
    long long carry = 0;
    int i;

    for (i = 0; i < a->size; i++) {
        if (i >= MAX_DIGITS) {
             fprintf(stderr, "Error: BigInt capacity exceeded during multiplication (digit processing).\n");
             initBigInt(result); return;
        }
        long long product = (long long)a->digits[i] * b + carry;
        temp_result.digits[i] = (int)(product % 10); // Cast to int
        carry = product / 10;
    }

    while (carry > 0) {
        if (i >= MAX_DIGITS) {
            fprintf(stderr, "Error: BigInt capacity exceeded during multiplication (carry processing).\n");
            initBigInt(result); return;
        }
        temp_result.digits[i++] = (int)(carry % 10); // Cast to int
        carry /= 10;
    }

    temp_result.size = i > 0 ? i : 1;

    memcpy(result, &temp_result, sizeof(BigInt));
    result->negative = a->negative;
}


int divideBigIntByInt(const BigInt *n, int divisor, BigInt *quotient) {
    if (divisor == 0) {
        fprintf(stderr, "Error: Division by zero.\n");
        initBigInt(quotient);
        return -1;
    }
     if (divisor < 0) {
        fprintf(stderr, "Error: Negative divisor not supported in this simple version.\n");
        initBigInt(quotient);
        return -1;
    }
    if (isBigIntZero(n)) {
        initBigInt(quotient); // Quotient is 0
        return 0; // Remainder is 0
    }

    int temp_quotient_digits[MAX_DIGITS];
    int quotient_len = 0;

    long long current_segment = 0;
    bool leading_zeros = true;

    for (int i = n->size - 1; i >= 0; i--) {
        current_segment = current_segment * 10 + n->digits[i];
        int digit_quotient = 0;

        if (current_segment >= divisor) {
            digit_quotient = (int)(current_segment / divisor); // Cast to int
            current_segment %= divisor;
        }

        if (!leading_zeros || digit_quotient > 0 || n->size == 1) { // Handle single digit numerator correctly
             if (quotient_len >= MAX_DIGITS) {
                  fprintf(stderr, "Error: BigInt capacity exceeded during division quotient storage.\n");
                  initBigInt(quotient);
                  return -1;
             }
             // Ensure we add the digit even if it's zero, if it's the only digit (e.g. 3/5 -> Q=0)
             if (!leading_zeros || digit_quotient > 0 || (quotient_len == 0 && n->size -1 == i) ) {
                temp_quotient_digits[quotient_len++] = digit_quotient;
             }
             if (digit_quotient > 0) { // Start recording after first non-zero
                 leading_zeros = false;
             }
        }
    }

    int remainder = (int)current_segment;

    initBigInt(quotient);
    if (quotient_len == 0) {
        quotient->size = 1;
        quotient->digits[0] = 0;
    } else {
        quotient->size = quotient_len;
        for (int i = 0; i < quotient->size; ++i) {
            quotient->digits[i] = temp_quotient_digits[quotient_len - 1 - i];
        }
    }
    quotient->negative = n->negative && !isBigIntZero(quotient);

    return remainder;
}
