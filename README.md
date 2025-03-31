# Base Conversion Tool

## Description

This is a command-line C program for converting numbers between various big-integer number systems (radixes). It runs interactively, first prompting the user for a radix, then prompting the user for a string of digits represented in that radix. The program then converts the input value to a series of predefined target radixes (binary, octal, decimal, hexadecimal, Base26, Base32, Base36, Base52, Base58, Base62, Base64) and displays all the results.

The latest version supports conversion of large integers, and the large integer calculation library has been implemented separately.

## Features

*   **Interactive Interface**: Prompts the user for input via the command line.
*   **Flexible Input Base**: Allows the user to specify the base of the input number. Supports bases: `2`, `8`, `10`, `16`, `26`, `32`, `36`, `52`, `58`, `62`, `64`.
*   **Multiple Output Bases**: Simultaneously converts the input value into 11 different base representations.
*   **Specific Character Sets**: Uses specific, conventional, or defined character sets (alphabets) for different bases:
    *   **Hexadecimal (Base 16)**: Input accepts lowercase `a-f` (converted internally to uppercase for processing); Output is consistently **lowercase** `a-f`.
    *   **Base 32**: Uses the Crockford alphabet (excludes I, L, O, U).
    *   **Base 58**: Uses the alphabet commonly associated with cryptocurrencies (excludes 0, O, I, l).
    *   **Base 64**: Uses the standard Base64 alphabet (`A-Z`, `a-z`, `0-9`, `+`, `/`).
    *   Other bases also use specific defined alphabets (see code or list below).

## Supported Bases and Character Sets

### Input

The program prompts the user to enter one of the following bases:

`2`, `8`, `10`, `16`, `26`, `32`, `36`, `52`, `58`, `62`, `64`

*   **Base 16 Input**: Accepts `0-9`, `A-F`, and `a-f`. The program automatically converts lowercase `a-f` to uppercase `A-F` internally before validation. Validation uses the character set `0123456789ABCDEF`.
*   **Base 64 Input**: Accepts standard Base64 characters: `A-Z`, `a-z`, `0-9`, `+`, `/`.
*   Input for other bases must strictly match their defined character sets.

### Output

The program converts the input value to all the following bases for output, using these character sets:

*   **Base  2**: `01`
*   **Base  8**: `01234567`
*   **Base 10**: `0123456789`
*   **Base 16**: `0123456789abcdef` (lowercase)
*   **Base 26**: `abcdefghijklmnopqrstuvwxyz` (lowercase letters)
*   **Base 32**: `0123456789ABCDEFGHJKMNPQRSTVWXYZ` (Crockford, no ILOU)
*   **Base 36**: `0123456789abcdefghijklmnopqrstuvwxyz` (digits + lowercase)
*   **Base 52**: `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ` (lowercase + uppercase)
*   **Base 58**: `123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ` (digits + letters, no 0OIl)
*   **Base 62**: `0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ` (digits + lowercase + uppercase)
*   **Base 64**: `ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/` (Standard)

## Compilation

For example, there is `baseconv.c` (containing the main program logic and the implementation of the core conversion functions `convert_base` and `convert_from_base` in the interactive interface) and `bigint.c` (containing a separate implementation of big integer calculations).

Use the following GCC command to compile:

```bash
gcc -o baseconv baseconv.c bigint.c
```

-o baseconv: Specifies the output executable name as baseconv.

baseconv.c bigint.c : Specifies the source files to compile.



Usage

Run the compiled executable directly:

./baseconv

The program will then sequentially prompt you to enter:

The input base of the number (e.g., 10, 16, 62).

The number string in that base (e.g., 201314, FF, qN0).

The program validates if the input string conforms to the character set of the chosen base (after converting Base 16 input to uppercase). Upon successful validation, it calculates the numerical value and outputs the conversion results for all target bases.

# Example Interaction
```
./baseconv
Enter the base of your input number (輸入進制) (e.g., 2, 8, 10, 16, 36, 64): 16
 (Input accepts 0-9, a-f, A-F)
Enter the number string in(輸入數值) Base 16: a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3

Input 'a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3' (Base 16) converted to value 75263518707598184987916378021939673586055614731957507592904438851787542395619 (base 10).

Conversion results for value 75263518707598184987916378021939673586055614731957507592904438851787542395619 (數值轉換結果):
Base  2 : 1010011001100101101001000101100100100000010000100010111110011101010000010111111001001000011001111110111111011100010011111011100010100000010010100001111100111111111111110001111110100000011111101001100110001110100001101111011111110111101000100111101011100011
Base  8 : 12314551054440204276352027711031767734237342402241747777617640375146164157376750475343
Base 10 : 75263518707598184987916378021939673586055614731957507592904438851787542395619
Base 16 : a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3
Base 26 : cyjyytcpnwvukqtirxvglggjrutdmvtdbfkhwqfqovrgxyewainqcmt
Base 32 : 19K5MHCJ0GHFKN0QWJ37XZE4ZE5098FKZZRZM1Z9K3M6YZVT4YQ3
Base 36 : 45auvrm8e57zm8y2dyh0s3ows1n3bit9vrxsqcaikpyj7j7izn
Base 52 : xDSKwAzlrIDvehqDgMhNZnKOposGeaOAfMWTzQvXaSnGt
Base 58 : ccxz6dm7nUk4HoNrm4oVTxmqBhfYCjGd5NqrwX3JEXcr
Base 62 : Dso1ycSnGU39ov88mMUMI0kMFax21oJDQy9FYKVPR9F
Base 64 : KZlpFkgQi+dQX5IZ+/cT7igSh8//x+gfpmOhvf3onrj

Restoration check from Base 64 OK.

```
### ⚙️ Dependencies


Thanks for your help : gemini, ChatGPT, deepseek .

### Sponsorship
If this project has been helpful to you, please consider sponsoring. Your support is greatly appreciated. Thank you!
```
BTC: bc1qt3nh2e6gjsfkfacnkglt5uqghzvlrr6jahyj2k
ETH: 0xD6503e5994bF46052338a9286Bc43bC1c3811Fa1
DOGE: DTszb9cPALbG9ESNJMFJt4ECqWGRCgucky
TRX: TAHUmjyzg7B3Nndv264zWYUhQ9HUmX4Xu4
```
### 📜 Disclaimer

This tool is provided for learning and research purposes only. Please use it with an understanding of the relevant risks. The developers are not responsible for financial losses or legal liability -caused by the use of this tool.

