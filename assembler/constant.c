#ifdef TEST
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "constant.h"
#include "processor.h"

// returns the value of a constant
int get_constant(char* line) {
    if (string_match(line, "0x"))
        return get_hex_constant(line + 2);
    else if (string_match(line, "h"))
        return get_hex_constant(line + 1);
    else if (string_match(line, "b"))
        return get_bin_constant(line + 1);
    else if (string_match(line, "-"))
        return get_dec_constant(line + 1) * -1;
    else if (isdigit(line[0]))
        return get_dec_constant(line);
    else
        return CONSTANT_ERROR;
}

// returns true if the character is a valid hex character
int is_hex_digit(char c) {
    return isdigit(c) || (tolower(c) >= 'a' && tolower(c) <= 'f');
}

int is_bin_digit(char c) {
    return (c == '0' || c == '1');
}

// returns the numerical value of a character
// this function works for all radixes
int get_num_value(char c) {
    c = tolower(c);
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return CONSTANT_ERROR;
}

int get_hex_constant(char* line) {
    int len = strlen(line);
    int number_length = 0;
    while (is_hex_digit(line[number_length]) && number_length < len)
        number_length++;
    int i, num = 0;
    for (i = 0; i < number_length; i++)
        num += get_num_value(line[i]) * pow(16, number_length - i - 1);
    return num;
}

int get_bin_constant(char* line) {
    int len = strlen(line);
    int number_length = 0;
    while (is_bin_digit(line[number_length]) && number_length < len)
        number_length++;
    int i, num = 0;
    for (i = 0; i < number_length; i++)
        num += get_num_value(line[i]) * pow(2, number_length - i - 1);
    return num;
}

int get_dec_constant(char* line) {
    int len = strlen(line);
    int number_length = 0;
    while (isdigit(line[number_length]) && number_length < len)
        number_length++;
    int i, num = 0;
    for (i = 0; i < number_length; i++)
        num += get_num_value(line[i]) * pow(10, number_length - i - 1);
    return num;
}

#ifdef TEST

int main() {
    char string[100];
    while (1) {
        scanf("%s" , string);
        printf("value = %d\n>",(int8_t)get_constant(string));
    }
}

#endif
