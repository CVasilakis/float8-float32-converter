# Float8 - Float32 Converter

An extendable and lightweight C library designed to convert between 8-bit floating-point numbers (float8) and single-precision floating-point numbers (float32). This library supports all possible float8 formats (Sign-Exponent-Mantissa combinations). It also implements special cases such as subnormal numbers and infinity. Fully compatible with the C99 standard, this project is developed in conjunction with the research presented [here](https://doi.org/10.3390/software4020011).

## How to Build

To build and run the current project execute the included `run.sh` script:
```bash
chmod +x run.sh
./run.sh
```

## How to Use

To integrate the library into your project, just include the `float8.c` and `float8.h` files. Below is an overview of the key types and functions included.
> [!NOTE]
> The `main.c` file is provided to demonstrate the usage of the library.

### Float8 format selection

- `EXPONENT_BITS`: A value in `float8.h` that allow you to configure the format of the float8 number used by your application.
  ```c
    // Example 1-4-3 format (sign-exponent-mantissa)
    #define EXPONENT_BITS 4
    ```

### Data Structures

- **`float8_t`**: A struct representing an 8-bit floating-point number (float8), consisting of:
  - `sign`: 1 bit for the sign.
  - `exponent`: A customizable number of bits (defined by `EXPONENT_BITS`).
  - `fraction`: The remaining bits for the fraction (mantissa).
> [!NOTE]
> Bitfields used to ensure a total struct length of 8 bits.
  
### Key Functions

- **`float8_t float_to_float8(float f)`**  
  Converts a single-precision floating-point number (`float32`) to an 8-bit floating-point number (`float8_t`).

- **`float float8_to_float(float8_t f8)`**  
  Converts an 8-bit floating-point number (`float8_t`) to a single-precision floating-point number (`float32`).

- **`float8_t uchar_to_float8(unsigned char c)`**  
  Converts an unsigned char to a `float8_t`.  

- **`unsigned char float8_to_uchar(float8_t f8)`**  
  Converts a `float8_t` to an unsigned char.  

## How to Test

The `main.c` file also includes tests to validate the functionality of its conversion functions. Each test reads data from a CSV file containing binary representations of `float8_t` numbers and their corresponding `float32` values. All possible conversion for each float8 format are tested. The CSV test files are placed in the following directories:
- `float8_to_float32_tests` — Contains tests to validate the `float8_to_float` conversion function.
- `float32_to_float8_tests` — Contains tests to validate the `float_to_float8` conversion function.

### Important Configuration

Ensure that the `EXPONENT_BITS` value in the `float8.h` file matches the test files loaded in the `main.c` file.
> [!NOTE]
> For example, with `EXPONENT_BITS` set to 4, the loaded tests should be `"1-4-3.csv"`.

### Running the Tests

The `main` function in `main.c` calls two primary tests:
- `float8_to_float32_test()`: Verifies that `float8` to `float32` conversions are correct by comparing computed and expected float values.
- `float32_to_float8_test()`: Verifies that `float32` to `float8` conversions are correct by comparing computed and expected binary values.

On success, each test will print confirmation messages indicating that all tests have passed.

## How the Library Works

This library implements an 8-bit floating-point format based on the IEEE 754 standard. Although IEEE 754 does not standardize an 8-bit format, this library follows the IEEE 754 rules for floating-point numbers, adapting them to an 8-bit structure.

Based on IEEE 754 standard, a binary floating-point number consists of 3 parts:

- The sign bit
- The exponent bits
- The significant bits (also known as significand, mantissa, or fraction bits).

To convert the binary value to decimal, the following rules apply:

- If the sign bit is equal to zero (0), then the converted number is positive (+); otherwise (if sign bit = 1), it is negative (-).
- The exponent field is an unsigned integer in biased form. The bias is calculated using the formula $( (2^{N-1}) - 1 )$, where $` N `$ is the number of bits in the exponent field. For example, for an 8-bit exponent (used by the single-precision 32-bit floating-point format), the bias is 127, whereas for an 11-bit exponent (used by the double-precision 64-bit floating-point format), the bias is 1023. Based on the binary representation of the exponent, there are three different ways to interpret the floating-point number:
  - If the exponent field consists entirely of zeros (0), then the number represented by the floating-point number is either zero or a subnormal number (explained below).
  - If the exponent field consists entirely of ones (1), then the number represented by the floating-point number is either infinity or a NaN (Not a Number).
  - Otherwise, (if the exponent field is neither all ones nor all zeros), the decimal representation of the floating-point number will have a factor of $` 2^{\mathrm{exponent} - \mathrm{bias}} `$.

- Before analyzing the significant bits, we should pay attention to the "implicit leading bit," which is defined in the IEEE 754 standard as follows:
  - If the exponent field consists entirely of zeros $( \mathrm{exponent} = 0 )$, then the leading bit of a number is implicitly zero (0.) and the scale factor is considered fixed to the least possible value $` 2^{1 - \mathrm{bias}} `$. The number represented in such a case is called a "subnormal" number.
  - Otherwise (if $` \mathrm{exponent} \neq 0 `$), the leading bit of a number is one (1.) implicitly.

Up to this point, the distinction between a normal and a subnormal should be clear. The significant bits represent the bits following the implicit leading 1 in normalized floating-point numbers, whereas in a subnormal number, the leading bit is 0, and the significand stores the fraction directly without the implicit leading 1.

From the above description, it is clear that to calculate the decimal value of a normalized floating-point number, we can use the following formula:

```math
(-1)^{\mathrm{sign\_bit}} \times 2^{(\mathrm{exponent} - \mathrm{bias})} \times (1.\mathrm{significand})
```

whereas, to calculate the decimal value of a subnormal floating-point number, we can use the following formula:

```math
(-1)^{\mathrm{sign\_bit}} \times 2^{(1 - \mathrm{bias})} \times (0.\mathrm{significand})
```

This library adheres to the IEEE 754 rules to provide accurate conversions between custom `float8` and standard `float32` representations, handling special cases such as zero, subnormal numbers, and infinity with the same conventions as the IEEE standard.

## Limitations

This library does not implement `NaN` (Not a Number) values in its 8-bit floating-point format, due to hardware and platform-specific variations in `NaN` representation. Specifically, the different interpretations of quiet and signaling `NaN`s as well as `NaN` payloads, makes the creation of a portable `NaN` representation in a custom 8-bit format challenging. So for simplicity and portability, `NaN` is currently omitted in this implementation. It can be easily added for a spesific platform implementation.
