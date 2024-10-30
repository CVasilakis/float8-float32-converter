#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "float8.h"

/*
 * Paths for csv test files.
 * The folder float8_to_float32_tests contains the tests for the float8_to_float32 function.
 * The folder float32_to_float8_tests contains the tests for the float32_to_float8 function.
 * Each file contains the binary representation of a float8_t number and the float value it represents.
 *
 * For the correct execution of the program remember to set correctly:
 * 1. The EXPONENT_BITS number in the float8.h file.
 * 2. The CSV filename to load the correct test.
 * 
 * For example, if EXPONENT_BITS is set to 4, the correct CSV file to load is "1-4-3.csv",
 * whereas if EXPONENT_BITS is set to 5, the correct CSV file to load is "1-5-2.csv", etc.
 */
char f8_to_f32_csv[] = "float8_to_float32_tests/1-4-3.csv";
char f32_to_f8_csv[] = "float32_to_float8_tests/1-4-3.csv";

/************************************************************
 *                   FUNCTION PROTOTYPES                    *
 ************************************************************/

void float8_to_float32_test();
void float32_to_float8_test();
unsigned char binaryStringToByte(const char *binaryString);

/************************************************************
 *                          MAIN                            *
 ************************************************************/

int main() {
    // Test the library for errors
    float8_to_float32_test();
    float32_to_float8_test();

    return 0;
}

/************************************************************
 *                    FUNCTION DEFINITIONS                  *
 ************************************************************/

/*
 * Test the conversion from float8 to float32. It validates the
 * correct functionality of the float8_to_float function.
 *
 * The function reads the binary representation of a float8_t number
 * and the float value it represents from a csv file. It then converts
 * the binary representation to float8. The function then calls the 
 * float8_to_float function and compares the result with the csv float
 * value. If the values are equal, the test passes.
 */
void float8_to_float32_test() {
    FILE *file = fopen(f8_to_f32_csv, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        // Remove carriage return character if present
        char *newline1 = strchr(line, '\r');
        if (newline1) *newline1 = '\0';
        // Remove newline character if present
        char *newline2 = strchr(line, '\n');
        if (newline2) *newline2 = '\0';

        // Split line by comma and get the first token
        char *token = strtok(line, ",");
        if (token == NULL) continue;

        // Get the binary string
        char binaryString[9];
        strcpy(binaryString, token);

        // Convert binary string to unsigned char
        unsigned char binaryValue = binaryStringToByte(token);

        // Get the second token (float value)
        token = strtok(NULL, ",");
        if (token == NULL) continue;

        // Get the floating point value
        float floatValue = strtof(token, NULL);

        // Print the current values that are about to be tested.
        // printf("Current test --> Binary string: %s, Unsigned Char: %u, Float: %.40f\n", 
        //         binaryString, binaryValue, floatValue);
        float f = float8_to_float(uchar_to_float8(binaryValue));
        assert(f == floatValue);
    }
    fclose(file);
    printf("\n#################### All float8 to float32 tests PASSED! ####################\n\n");
}

/*
 * Test the conversion from float32 to float8. It validates the
 * correct functionality of the float_to_float8 function.
 *
 * The function reads the binary representation of a float8_t number
 * and the float value it represents from a csv file. It then converts
 * the float value to float8. The function then calls the float8_to_uchar
 * function and compares the result with the csv binary value. If the values
 * are equal, the test passes.
 */
void float32_to_float8_test() {
    FILE *file = fopen(f32_to_f8_csv, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        // Remove carriage return character if present
        char *newline1 = strchr(line, '\r');
        if (newline1) *newline1 = '\0';
        // Remove newline character if present
        char *newline2 = strchr(line, '\n');
        if (newline2) *newline2 = '\0';

        // Split line by comma and get the first token
        char *token = strtok(line, ",");
        if (token == NULL) continue;

        // Get the floating point value
        float floatValue = strtof(token, NULL);

        // Get the second token (binary string)
        token = strtok(NULL, ",");
        if (token == NULL) continue;

        // Get the binary string
        char binaryString[9];
        strcpy(binaryString, token);

        // Convert binary string to unsigned char
        unsigned char binaryValue = binaryStringToByte(binaryString);

        // Print the current values that are about to be tested.
        // printf("Current test --> Binary string: %s, Unsigned Char: %u, Float: %.40f\n",
        //         binaryString, binaryValue, floatValue);
        unsigned char result = float8_to_uchar(float_to_float8(floatValue));
        assert(result == binaryValue);
    }
    fclose(file);
    printf("\n#################### All float32 to float8 tests PASSED! ####################\n\n");
}

/*
 * Convert a binary string to an unsigned char.
 *
 * @param binaryString: binary string to be converted
 * @return: unsigned char
 */
unsigned char binaryStringToByte(const char *binaryString) {
    unsigned char result = 0;
    while (*binaryString) {
        result = (result << 1) | (*binaryString++ - '0');
    }
    return result;
}
