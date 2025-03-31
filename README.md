# Base Conversion Tool

## Description

This is a command-line C program designed to convert numbers between various numeral systems (bases). It operates interactively, prompting the user first for the input base and then for the number string represented in that base. The program then converts the input value to a range of predefined target bases (Binary, Octal, Decimal, Hexadecimal, Base26, Base32, Base36, Base52, Base58, Base62, Base64) and displays all results.

Internally, the program uses `unsigned long long` to store the numerical value, allowing for the conversion of large integers.

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

Assuming you have `main.c` (containing the main program logic and interactive interface) and `baseconv.c` (containing the implementations of the core conversion functions `convert_base` and `convert_from_base`).

Use the following GCC command to compile:

```bash
gcc -o baseconv baseconv.c main.c
```

-o baseconv: Specifies the output executable name as baseconv.

baseconv.c main.c: Specifies the source files to compile.



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
Enter the base of your input number(輸入進制)(e.g., 2, 8, 10, 16, 36, 64): 10
Enter the number string in Base 10 (輸入數值) : 123456789

Conversion results for value 123456789 (數值轉換結果):
Base  2 : 111010110111100110100010101
Base  8 : 726746425
Base 10 : 123456789
Base 16 : 75bcd15
Base 26 : kkeekb
Base 32 : 3NQK8N
Base 36 : 21i3v9
Base 52 : qUbfb
Base 58 : bUKpk
Base 62 : 8m0Kx
Base 64 : HW80V


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

