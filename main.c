//  gcc -o baseconv baseconv.c bigint.c   /*-Wall -Wextra -pedantic -std=c99 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h> // For isspace, toupper, isdigit, islower
#include <limits.h>
#include "bigint.h" // Include our BigInt library

// Author: 8891689

// --- Forward declarations for helper functions used in main ---
bool is_valid_for_base(const char* str, const char* digits);
bool read_line(char* buffer, size_t size);

// --- Base Conversion using BigInt ---
// (These functions use BigInt but are part of the application logic, not the core BigInt library)

/*
 * Converts BigInt to a string in the specified base.
 * Writes the result to 'out' buffer.
 */
void convert_base_bigint(const BigInt *value_ptr, const char* digits, char* out, size_t out_size) {
    int radix = (int)strlen(digits);
    if (out_size == 0) return;
    out[0] = '\0';

    if (radix < 2) {
        fprintf(stderr, "Error: Radix must be at least 2.\n");
        return;
    }

    char buf[MAX_DIGITS * 4 + 2];
    size_t i = 0;

    BigInt quotient;
    BigInt current_value;
    memcpy(&current_value, value_ptr, sizeof(BigInt)); // Copy the passed BigInt

    //bool was_negative = current_value.negative;
    current_value.negative = false;

    if (isBigIntZero(&current_value)) {
        if (i < sizeof(buf) - 1) {
            buf[i++] = digits[0];
        } else {
            fprintf(stderr, "Error: Temp buffer 'buf' too small for zero.\n");
            out[0] = '\0';
            return;
        }
    } else {
        while (!isBigIntZero(&current_value)) {
            if (i >= sizeof(buf) - 1) {
                fprintf(stderr, "Error: Temp buffer 'buf' overflow during conversion.\n");
                out[0] = '\0';
                return;
            }

            initBigInt(&quotient);
            int remainder = divideBigIntByInt(&current_value, radix, &quotient);
            buf[i++] = digits[remainder];
            memcpy(&current_value, &quotient, sizeof(BigInt));
        }
    }

    buf[i] = '\0';

    if (i + 1 > out_size) {
        fprintf(stderr, "Error: Output buffer too small.\n");
        out[0] = '\0';
        return;
    }

    size_t out_idx = 0;
    for (int j = (int)i - 1; j >= 0; j--) {
        out[out_idx++] = buf[j];
    }
    out[out_idx] = '\0';
}

void convert_from_base_bigint(const char* str, const char* digits, BigInt* result) {
    initBigInt(result);
    size_t base = strlen(digits);
    if (base < 2) {
        fprintf(stderr, "Error: Base must be at least 2 for conversion from string.\n");
        return; // Return 0
    }

    BigInt current_digit_bigint; // To hold the BigInt value of the current digit
    // BigInt base_bigint; // Not strictly needed for multiplyBigIntByInt
    // setBigIntFromInt(&base_bigint, base); // Convert base to BigInt once

    const char* current_char = str;
    while (*current_char != '\0') {
        // Skip whitespace if necessary (should be handled by read_line, but safe check)
        if (isspace((unsigned char)*current_char)) {
             current_char++;
             continue;
        }

        // Find the digit's value in the provided 'digits' string
        const char* ptr = strchr(digits, *current_char);
        char original_char = *current_char; // Store for error message

        // Case-insensitive matching logic (as before)
        if (!ptr) {
            char alt_char = original_char;
            if (islower((unsigned char)alt_char)) alt_char = toupper((unsigned char)alt_char);
            else if (isupper((unsigned char)alt_char)) alt_char = tolower((unsigned char)alt_char);

            if (alt_char != original_char) {
                 ptr = strchr(digits, alt_char);
            }
        }

        if (!ptr) { // If still not found, it's an invalid character
             fprintf(stderr, "Error: Invalid character '%c' in input string '%s' for the given base digits '%s'.\n", original_char, str, digits);
             initBigInt(result); // Return 0 on error
             return;
        }
        unsigned int digit_val = ptr - digits;

        // result = result * base
        BigInt temp_result;
        multiplyBigIntByInt(result, (int)base, &temp_result); // temp_result = result * base

        // Convert digit_val to a BigInt
        // ***** PREVIOUSLY ERROR LINE 169 *****
        setBigIntFromInt(&current_digit_bigint, digit_val); // Correct variable name and pass address

        // result = temp_result + current_digit_bigint
        // ***** PREVIOUSLY ERROR LINE 173 *****
        addBigInt(&temp_result, &current_digit_bigint, result); // Correct variable name and pass addresses

        current_char++;
    }
    // Sign is not handled here. Assumes positive input strings.
}


// --- Main Application Logic ---

// --- Define Character Sets ---
const char* digits2  = "01";
const char* digits8  = "01234567";
const char* digits10 = "0123456789";
const char* digits16_upper = "0123456789ABCDEF";
const char* digits16_lower = "0123456789abcdef";
const char* digits16_input = "0123456789abcdefABCDEF";
const char* digits26 = "abcdefghijklmnopqrstuvwxyz";
const char* digits32 = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
const char* digits36_lower = "0123456789abcdefghijklmnopqrstuvwxyz";
const char* digits36_input = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* digits52 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* digits58 = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
const char* digits62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* digits64_std = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// const char* digits64_url = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"; // Unused currently


int main(void)
{
    char base_input_buf[20];
    char num_input_buf[MAX_DIGITS * 4];
    char processed_input_buf[MAX_DIGITS * 4];
    char out_buf[MAX_DIGITS * 4];
    char num_str_decimal[MAX_DIGITS * 4];

    long input_base_num = 0;
    const char* input_validation_set = NULL;
    const char* conversion_digits_set = NULL;
    const char* input_base_name = "";
    char* endptr;
    BigInt num;
    initBigInt(&num);

    // --- Prompt for and Read Input Base ---
    while (conversion_digits_set == NULL) {
        printf("Enter the base of your input number (輸入進制) (e.g., 2, 8, 10, 16, 36, 64): ");
        if (!read_line(base_input_buf, sizeof(base_input_buf))) {
             fprintf(stderr, "Error or empty input while reading base. Exiting.\n");
             return 1;
        }

        errno = 0;
        input_base_num = strtol(base_input_buf, &endptr, 10);

        if (errno != 0 || *endptr != '\0' || base_input_buf == endptr || input_base_num < 2 || input_base_num > 64) {
             fprintf(stderr, "Error: Invalid base '%s'. Please enter a number between 2 and 64.\n", base_input_buf);
             conversion_digits_set = NULL;
             continue;
        }

        // --- Map Base Number to Input Validation Set and Conversion Digits Set ---
        switch (input_base_num) {
             case 2:  input_validation_set = digits2; conversion_digits_set = digits2; input_base_name="Base 2"; break;
             case 8:  input_validation_set = digits8; conversion_digits_set = digits8; input_base_name="Base 8"; break;
             case 10: input_validation_set = digits10; conversion_digits_set = digits10; input_base_name="Base 10"; break;
             case 16:
                  input_validation_set = digits16_input;
                  conversion_digits_set = digits16_upper; // Normalize to uppercase for conversion
                  input_base_name = "Base 16";
                  printf(" (Input accepts 0-9, a-f, A-F)\n");
                  break;
             case 26: input_validation_set = digits26; conversion_digits_set = digits26; input_base_name="Base 26 (a-z)"; break;
             case 32: input_validation_set = digits32; conversion_digits_set = digits32; input_base_name="Base 32 (Crockford)"; break;
             case 36:
                  input_validation_set = digits36_input;
                  conversion_digits_set = digits36_lower; // Normalize to lowercase for conversion
                  input_base_name="Base 36";
                  printf(" (Input accepts 0-9, a-z, A-Z; processed as lowercase)\n");
                  break;
             case 52: input_validation_set = digits52; conversion_digits_set = digits52; input_base_name="Base 52 (a-zA-Z)"; break;
             case 58: input_validation_set = digits58; conversion_digits_set = digits58; input_base_name="Base 58 (Bitcoin)"; break;
             case 62: input_validation_set = digits62; conversion_digits_set = digits62; input_base_name="Base 62 (0-9a-zA-Z)"; break;
             case 64:
                 input_validation_set = digits64_std; conversion_digits_set = digits64_std;
                 input_base_name="Base 64 (Standard)";
                 printf(" (Using standard Base64 alphabet: A-Z a-z 0-9 + /)\n");
                 break;
             default:
                 fprintf(stderr, "Error: Base %ld selection logic error or base not supported.\n", input_base_num);
                  conversion_digits_set = NULL;
        }
    }

    // --- Prompt for and Read Number String ---
    bool valid_num_input = false;
    while (!valid_num_input) {
        printf("Enter the number string in(輸入數值) %s: ", input_base_name);
        if (!read_line(num_input_buf, sizeof(num_input_buf))) {
            fprintf(stderr, "Error or empty input while reading number string. Exiting.\n");
            return 1;
        }

        // Validate the raw input against the allowed character set `input_validation_set`
        if (!is_valid_for_base(num_input_buf, input_validation_set)) {
            fprintf(stderr, "Error: Input string '%s' contains characters not allowed for %s.\n", num_input_buf, input_base_name);
            fprintf(stderr, "Allowed characters are: %s\n", input_validation_set);
            continue;
        }

        // Prepare the processed input buffer for conversion, adjusting case if necessary
        strncpy(processed_input_buf, num_input_buf, sizeof(processed_input_buf) - 1);
        processed_input_buf[sizeof(processed_input_buf) - 1] = '\0';

        if (input_base_num == 16) {
            for (char *p = processed_input_buf; *p; ++p) { *p = toupper((unsigned char)*p); }
        } else if (input_base_num == 36) {
             for (char *p = processed_input_buf; *p; ++p) { *p = tolower((unsigned char)*p); }
        }

        valid_num_input = true;
    }

    // --- Convert Processed Input String to BigInt Number ---
    convert_from_base_bigint(processed_input_buf, conversion_digits_set, &num);


    // Print the converted BigInt value (in base 10)
    bigIntToString(&num, num_str_decimal, sizeof(num_str_decimal));
    printf("\nInput '%s' (%s) converted to value %s (base 10).\n",
    num_input_buf, input_base_name, num_str_decimal);


    // --- Proceed with Conversions to Other Bases (OUTPUT Section) ---
    printf("\nConversion results for value %s (數值轉換結果):\n", num_str_decimal);

    convert_base_bigint(&num, digits2, out_buf, sizeof(out_buf));
    printf("Base  2 : %s\n", out_buf);

    convert_base_bigint(&num, digits8, out_buf, sizeof(out_buf));
    printf("Base  8 : %s\n", out_buf);

    printf("Base 10 : %s\n", num_str_decimal);

    convert_base_bigint(&num, digits16_lower, out_buf, sizeof(out_buf));
    printf("Base 16 : %s\n", out_buf);

    convert_base_bigint(&num, digits26, out_buf, sizeof(out_buf));
    printf("Base 26 : %s\n", out_buf);

    convert_base_bigint(&num, digits32, out_buf, sizeof(out_buf));
    printf("Base 32 : %s\n", out_buf);

    convert_base_bigint(&num, digits36_lower, out_buf, sizeof(out_buf));
    printf("Base 36 : %s\n", out_buf);

    convert_base_bigint(&num, digits52, out_buf, sizeof(out_buf));
    printf("Base 52 : %s\n", out_buf);

    convert_base_bigint(&num, digits58, out_buf, sizeof(out_buf));
    printf("Base 58 : %s\n", out_buf);

    convert_base_bigint(&num, digits62, out_buf, sizeof(out_buf));
    printf("Base 62 : %s\n", out_buf);

    convert_base_bigint(&num, digits64_std, out_buf, sizeof(out_buf));
    printf("Base 64 : %s\n", out_buf);

    // --- Restoration Checks ---
    BigInt restored_num;
    initBigInt(&restored_num);
    convert_from_base_bigint(out_buf, digits64_std, &restored_num);

    if (compareBigInt(&num, &restored_num) != 0) {
         char restored_num_str[MAX_DIGITS * 4];
         bigIntToString(&restored_num, restored_num_str, sizeof(restored_num_str));
         printf("\nWarning: Restoration check from Base 64 ('%s' -> %s) failed!\n", out_buf, restored_num_str);
         printf("Original (Base 10): %s\n", num_str_decimal);
    } else {
         printf("\nRestoration check from Base 64 OK.\n");
    }

    return 0;
} // End of main


// --- Helper Function Definitions ---

/*
 * Reads a line from stdin safely, removes newline, trims leading/trailing whitespace.
 * Returns true on success (non-empty line read), false otherwise.
 */
bool read_line(char* buffer, size_t size) {
    if (size == 0) return false;

    if (fgets(buffer, (int)size, stdin) == NULL) { // Cast size to int for fgets if needed, though size_t usually works
        buffer[0] = '\0';
        if (feof(stdin)) {
             return false; // EOF
        } else {
             perror("fgets error");
             return false; // Read error
        }
    }

    char *newline_pos = strchr(buffer, '\n');
    if (newline_pos != NULL) {
        *newline_pos = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF); // Consume rest of line
        fprintf(stderr, "Warning: Input line exceeded buffer size (%zu bytes).\n", size);
    }

    char* start = buffer;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    size_t current_len = strlen(start); // Use start here
    char* end = start + current_len - 1;
    while (end >= start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';

    if (start > buffer) {
        memmove(buffer, start, strlen(start) + 1);
    }

    return (buffer[0] != '\0');
}


/*
 * Checks if all characters in 'str' are present in the 'digits' character set.
 */
bool is_valid_for_base(const char* str, const char* digits) {
    if (!str || !digits) return false;
    if (*str == '\0') return false; // Empty string is invalid input number

    for (size_t i = 0; str[i] != '\0'; ++i) {
        if (strchr(digits, str[i]) == NULL) {
            return false;
        }
    }
    return true;
}
