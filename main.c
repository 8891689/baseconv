//  gcc -o baseconv baseconv.c main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h> // For isspace

// Author: 8891689

// Forward declaration for helper function if defined below main
bool is_valid_for_base(const char* str, const char* digits);
// Forward declaration for conversion functions if defined elsewhere
void convert_base(unsigned long long value, const char* digits, char* out, size_t out_size);
unsigned long long convert_from_base(const char* str, const char* digits);

// --- Define Character Sets ---
// Define all alphabets needed for input selection AND output generation

// Base 2, 8, 10 (standard)
const char* digits2  = "01";
const char* digits8  = "01234567";
const char* digits10 = "0123456789";

// Base 16
const char* digits16_upper = "0123456789ABCDEF"; // For potential input validation if needed
const char* digits16_lower = "0123456789abcdef"; // **Use this for OUTPUT**

// Base 26 (lowercase letters) - Matches example
const char* digits26 = "abcdefghijklmnopqrstuvwxyz";

// Base 32 (Crockford) - Matches example
const char* digits32 = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

// Base 36 (digits + lowercase) - Matches example
const char* digits36 = "0123456789abcdefghijklmnopqrstuvwxyz";

// Base 52 (mixed case letters) - Matches example
const char* digits52 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Base 58 (no 0OIl) - Matches example
const char* digits58 = "123456789" "abcdefghijkmnopqrstuvwxyz" "ABCDEFGHJKLMNPQRSTUVWXYZ";

// Base 62 (numbers + mixed upper and lower case letters) - matching example
const char* digits62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";


// Base 64 (Standard Alphabet) - **Use this for INPUT and OUTPUT**
const char* digits64_std = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/*
 * Helper function to read a line safely from stdin, removing the newline.
 * Returns true on success, false on read error or empty input.
 */
bool read_line(char* buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return false;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    char* temp = buffer;
    while(*temp && isspace((unsigned char)*temp)) {
        temp++;
    }
    return (*temp != '\0');
}


/*
 * Main function for interactive base conversion.
 * Prompts user for input base and number string.
 */
int main(void)
{
    char base_input_buf[20];
    char num_input_buf[100];
    long input_base_num = 0;
    const char* input_digits = NULL; // Pointer to the DIGITS for the INPUT base
    const char* input_base_name = ""; // Store name for clarity
    char* endptr;
    unsigned long long num = 0;

    // --- Prompt for and Read Input Base ---
    while (input_digits == NULL) {
        //printf("Enter the base of your input number (e.g., 2, 8, 10, 16, 36, 64): ");
        printf("Enter the base of your input number(輸入進制)(e.g., 2, 8, 10, 16, 36, 64): ");
        if (!read_line(base_input_buf, sizeof(base_input_buf))) {
             fprintf(stderr, "Error or empty input while reading base. Exiting.\n");
             return 1;
        }

        errno = 0;
        input_base_num = strtol(base_input_buf, &endptr, 10);

        if (errno != 0 || *endptr != '\0' || base_input_buf == endptr) {
             fprintf(stderr, "Error: Invalid base '%s'. Please enter a number.\n", base_input_buf);
             continue;
        }

        // --- Map Base Number to Input Digits String ---
        switch (input_base_num) {
            case 2:  input_digits = digits2;  input_base_name="Base 2"; break;
            case 8:  input_digits = digits8;  input_base_name="Base 8"; break;
            case 10: input_digits = digits10; input_base_name="Base 10"; break;
            // Modify the input processing logic of Base 16
            case 16:
                 input_digits = digits16_upper;
                 input_base_name = "Base 16";
                 printf(" (Input accepts a-f in lowercase, converted to uppercase)\n"); // Update Tips
                 break;

            case 26: input_digits = digits26; input_base_name="Base 26"; break;
            case 32: input_digits = digits32; input_base_name="Base 32"; break;
            case 36: input_digits = digits36; input_base_name="Base 36"; break;
            case 52: input_digits = digits52; input_base_name="Base 52"; break;
            case 58: input_digits = digits58; input_base_name="Base 58"; break;
            case 62: input_digits = digits62; input_base_name="Base 62"; break;
            case 64:
                input_digits = digits64_std; // Use STANDARD Base64 for INPUT
                input_base_name="Base 64";
                printf(" (Using standard Base64 alphabet: A-Z a-z 0-9 + /)\n");
                break;
            default:
                fprintf(stderr, "Error: Unsupported input base %ld.\n", input_base_num);
                fprintf(stderr, "Supported bases are: 2, 8, 10, 16, 26, 32, 36, 52, 58, 62, 64.\n");
        }
    }

// --- Prompt for and Read Number String ---
bool valid_num_input = false;
while (!valid_num_input) {
    //printf("Enter the number string in %s (using digits: %s): ", input_base_name, input_digits);
    printf("Enter the number string in %s (輸入數值) : ", input_base_name, input_digits);
    if (!read_line(num_input_buf, sizeof(num_input_buf))) {
        fprintf(stderr, "Error or empty input while reading number string. Exiting.\n");
        return 1;
    }

    // New: Automatically convert lowercase letters to uppercase when inputting Base 16
    if (input_base_num == 16) {
        for (char *p = num_input_buf; *p; ++p) {
            if (*p >= 'a' && *p <= 'f') {
                *p = toupper((unsigned char)*p);
            }
        }
    }

    if (!is_valid_for_base(num_input_buf, input_digits)) {
        fprintf(stderr, "Error: Input string '%s' contains invalid characters for %s.\n", num_input_buf, input_base_name);
        fprintf(stderr, "Valid characters are: %s\n", input_digits);
    } else {
        valid_num_input = true;
    }
}


    // --- Convert Input String to Number ---
    num = convert_from_base(num_input_buf, input_digits);

    //printf("\nInput '%s' (%s) converted to value %llu.\n",
           //num_input_buf, input_base_name, num);

    // --- Proceed with Conversions to Other Bases (OUTPUT Section) ---
    char buf[100];

    printf("\nConversion results for value %llu (數值轉換結果):\n", num);

    convert_base(num, digits2, buf, sizeof(buf));
    printf("Base  2 : %s\n", buf);

    convert_base(num, digits8, buf, sizeof(buf));
    printf("Base  8 : %s\n", buf);

    convert_base(num, digits10, buf, sizeof(buf));
    printf("Base 10 : %s\n", buf);

    // Base 16: Use LOWERCASE digits for output, as requested
    convert_base(num, digits16_lower, buf, sizeof(buf));
    printf("Base 16 : %s\n", buf);

    // Base 26: Use lowercase digits (already correct)
    convert_base(num, digits26, buf, sizeof(buf));
    printf("Base 26 : %s\n", buf);

    // Base 32: Use Crockford digits (already correct)
    convert_base(num, digits32, buf, sizeof(buf));
    printf("Base 32 : %s\n", buf);

    // Base 36: Use digits + lowercase (already correct)
    convert_base(num, digits36, buf, sizeof(buf));
    printf("Base 36 : %s\n", buf);

    // Base 52: Use mixed case letters (already correct)
    convert_base(num, digits52, buf, sizeof(buf));
    printf("Base 52 : %s\n", buf);

    // Base 58: Use specific mixed case (already correct)
    convert_base(num, digits58, buf, sizeof(buf));
    printf("Base 58 : %s\n", buf);

    // Base 62: Use digits + mixed case (already correct)
    convert_base(num, digits62, buf, sizeof(buf));
    printf("Base 62 : %s\n", buf);

    // Base 64: Use STANDARD digits for output, as requested
    convert_base(num, digits64_std, buf, sizeof(buf));
    printf("Base 64 : %s\n", buf);

    // --- Restoration Checks ---
    // Restore from Base 64 result (using standard digits)
    unsigned long long restored_from_b64 = convert_from_base(buf, digits64_std);
    //printf("\nRestored number from Base 64 result ('%s'): %llu\n", buf, restored_from_b64);
    if (restored_from_b64 != num) {
         printf("Warning: Base 64 restoration check failed!\n");
    }

    // Restore from original input (using the input_digits selected earlier)
    unsigned long long restored_from_input = convert_from_base(num_input_buf, input_digits);
    //printf("Restored number from original input ('%s', %s): %llu\n", num_input_buf, input_base_name, restored_from_input);
     if (restored_from_input != num) {
         printf("Warning: Input restoration check failed!\n");
    }

    return 0; // Indicate successful execution
}

// Definition of the helper function (ensure this version is used)

bool is_valid_for_base(const char* str, const char* digits) {
    if (!str || !digits) return false;
    if (*str == '\0') return false; // Disallow empty string input
    while (*str) {
        if (strchr(digits, *str) == NULL) {
            return false; // Character not in the specified digits set
        }
        str++;
    }
    return true;
}


